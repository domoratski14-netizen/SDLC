#include "InputDialog.h"

#include "Ui.h"

namespace {
constexpr wchar_t CLASS_NAME[] = L"PlanetWeightInputDialog";
constexpr int ID_OK = 1;
constexpr int ID_CANCEL = 2;
}  // namespace

std::optional<InputData> InputDialog::show(HWND owner, const std::optional<InputData>& initial) {
    static bool registered = false;
    if (!registered) {
        WNDCLASSW wc{};
        wc.lpfnWndProc = windowProc;
        wc.hInstance = GetModuleHandleW(nullptr);
        wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
        wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BTNFACE + 1);
        wc.lpszClassName = CLASS_NAME;
        RegisterClassW(&wc);
        registered = true;
    }

    InputDialog dialog(owner, initial);
    RECT ownerRect;
    GetWindowRect(owner, &ownerRect);
    const int width = 400, height = 190;
    const int x = ownerRect.left + (ownerRect.right - ownerRect.left - width) / 2;
    const int y = ownerRect.top + (ownerRect.bottom - ownerRect.top - height) / 2;
    CreateWindowExW(WS_EX_DLGMODALFRAME, CLASS_NAME, L"Ввод данных",
                    WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE, x, y, width, height,
                    owner, nullptr, GetModuleHandleW(nullptr), &dialog);

    // Модальность: главное окно недоступно, пока открыто окно ввода.
    EnableWindow(owner, FALSE);
    MSG msg;
    while (!dialog.done_ && GetMessageW(&msg, nullptr, 0, 0)) {
        if (!IsDialogMessageW(dialog.hwnd_, &msg)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }
    EnableWindow(owner, TRUE);
    SetForegroundWindow(owner);
    DestroyWindow(dialog.hwnd_);
    return dialog.result_;
}

InputDialog::InputDialog(HWND owner, const std::optional<InputData>& initial)
    : owner_(owner), initial_(initial) {}

LRESULT CALLBACK InputDialog::windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    InputDialog* self = nullptr;
    if (msg == WM_NCCREATE) {
        self = static_cast<InputDialog*>(reinterpret_cast<CREATESTRUCTW*>(lParam)->lpCreateParams);
        self->hwnd_ = hwnd;
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
    } else {
        self = reinterpret_cast<InputDialog*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    }
    return self ? self->handleMessage(msg, wParam, lParam) : DefWindowProcW(hwnd, msg, wParam, lParam);
}

LRESULT InputDialog::handleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        createControls();
        return 0;
    case WM_COMMAND:
        if (LOWORD(wParam) == ID_OK || LOWORD(wParam) == IDOK) {
            accept();
        } else if (LOWORD(wParam) == ID_CANCEL || LOWORD(wParam) == IDCANCEL) {
            done_ = true;  // ввод отменен
        }
        return 0;
    case WM_CLOSE:
        done_ = true;
        return 0;
    }
    return DefWindowProcW(hwnd_, msg, wParam, lParam);
}

void InputDialog::createControls() {
    ui::label(hwnd_, L"Вес на Земле:", 16, 20, 170, 24);
    weightEdit_ = ui::edit(hwnd_, 200, 18, 170, 26);
    ui::label(hwnd_, L"Единицы измерения:", 16, 58, 180, 24);
    unitCombo_ = ui::combo(hwnd_, 200, 56, 170, 120);
    SendMessageW(unitCombo_, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"кг"));
    SendMessageW(unitCombo_, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"фунты"));
    ui::button(hwnd_, L"OK", ID_OK, 190, 104, 85, 28, true);
    ui::button(hwnd_, L"Отмена", ID_CANCEL, 285, 104, 85, 28, false);

    // Восстановление последних введенных данных.
    if (initial_) {
        SetWindowTextW(weightEdit_, initial_->weightText.c_str());
        SendMessageW(unitCombo_, CB_SETCURSEL, initial_->unit == WeightUnit::Kilogram ? 0 : 1, 0);
    } else {
        SendMessageW(unitCombo_, CB_SETCURSEL, 0, 0);
    }
    SendMessageW(weightEdit_, EM_SETSEL, 0, -1);
    SetFocus(weightEdit_);
}

void InputDialog::accept() {
    InputData data;
    data.weightText = ui::text(weightEdit_);
    data.unit = SendMessageW(unitCombo_, CB_GETCURSEL, 0, 0) == 1 ? WeightUnit::Pound
                                                                   : WeightUnit::Kilogram;
    result_ = data;
    done_ = true;
}
