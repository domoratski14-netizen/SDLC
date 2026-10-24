#include "MainWindow.h"

#include <cstdio>

#include "../model/PlanetWeightModel.h"
#include "Ui.h"

namespace {
constexpr wchar_t CLASS_NAME[] = L"PlanetWeightMainWindow";
constexpr int ID_INPUT = 100;

std::wstring format(const wchar_t* pattern, double value) {
    wchar_t buffer[64];
    swprintf(buffer, 64, pattern, value);
    std::wstring s(buffer);
    for (wchar_t& c : s) {
        if (c == L'.') c = L',';  // десятичная запятая
    }
    return s;
}
}  // namespace

bool MainWindow::create(HINSTANCE instance, int showCommand) {
    WNDCLASSW wc{};
    wc.lpfnWndProc = windowProc;
    wc.hInstance = instance;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BTNFACE + 1);
    wc.lpszClassName = CLASS_NAME;
    RegisterClassW(&wc);

    hwnd_ = CreateWindowExW(0, CLASS_NAME, L"Вес на планетах Солнечной системы",
                            WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                            CW_USEDEFAULT, CW_USEDEFAULT, 640, 420, nullptr, nullptr, instance,
                            this);
    if (!hwnd_) return false;
    ShowWindow(hwnd_, showCommand);
    UpdateWindow(hwnd_);
    return true;
}

LRESULT CALLBACK MainWindow::windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    MainWindow* self = nullptr;
    if (msg == WM_NCCREATE) {
        self = static_cast<MainWindow*>(reinterpret_cast<CREATESTRUCTW*>(lParam)->lpCreateParams);
        self->hwnd_ = hwnd;
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
    } else {
        self = reinterpret_cast<MainWindow*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    }
    return self ? self->handleMessage(msg, wParam, lParam) : DefWindowProcW(hwnd, msg, wParam, lParam);
}

LRESULT MainWindow::handleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        createControls();
        return 0;
    case WM_SETFOCUS:
        SetFocus(inputButton_);  // кнопка «Ввести данные» нажимается и с клавиатуры
        return 0;
    case WM_COMMAND:
        if ((LOWORD(wParam) == ID_INPUT || LOWORD(wParam) == IDOK) && inputAction_) {
            inputAction_();  // действие пользователя передается контроллеру
        }
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hwnd_, msg, wParam, lParam);
}

void MainWindow::createControls() {
    summaryLabel_ = ui::label(hwnd_, L"Данные ещё не введены", 16, 12, 600, 24);
    inputButton_ = ui::button(hwnd_, L"Ввести данные", ID_INPUT, 16, 40, 592, 30, true);

    const int columns[] = {16, 140, 240, 380, 490};
    const wchar_t* headers[] = {L"Планета", L"g, м/с²", L"Вес (в ед. ввода)", L"Вес, кг",
                                L"Сила тяжести, Н"};
    for (int c = 0; c < 5; ++c) {
        HWND h = ui::label(hwnd_, headers[c], columns[c], 86, 130, 24);
        SendMessageW(h, WM_SETFONT, reinterpret_cast<WPARAM>(ui::boldFont()), TRUE);
    }
    const auto& planets = solarSystem();
    for (size_t i = 0; i < planets.size(); ++i) {
        const int y = 114 + static_cast<int>(i) * 30;
        ui::label(hwnd_, planets[i].name.c_str(), columns[0], y, 120, 24);
        ui::label(hwnd_, format(L"%.2f", planets[i].gravity).c_str(), columns[1], y, 90, 24);
        weightLabels_[i] = ui::label(hwnd_, L"—", columns[2], y, 130, 24);
        weightKgLabels_[i] = ui::label(hwnd_, L"—", columns[3], y, 100, 24);
        forceLabels_[i] = ui::label(hwnd_, L"—", columns[4], y, 120, 24);
    }
}

void MainWindow::showError(const std::wstring& message) const {
    MessageBoxW(hwnd_, message.c_str(), L"Некорректные данные", MB_OK | MB_ICONERROR);
}

void MainWindow::modelChanged(const PlanetWeightModel& model) {
    if (!model.isCalculated()) return;
    const std::wstring unit = unitTitle(model.unit());
    SetWindowTextW(summaryLabel_, (L"Введено: вес на Земле " + format(L"%.2f", model.weight()) +
                                   L" " + unit + L" (" + format(L"%.2f", model.weightKg()) +
                                   L" кг)").c_str());
    const auto& planets = solarSystem();
    for (size_t i = 0; i < planets.size(); ++i) {
        SetWindowTextW(weightLabels_[i],
                       (format(L"%.2f", model.weightOn(planets[i])) + L" " + unit).c_str());
        SetWindowTextW(weightKgLabels_[i], format(L"%.2f", model.weightKgOn(planets[i])).c_str());
        SetWindowTextW(forceLabels_[i], format(L"%.1f", model.forceOn(planets[i])).c_str());
    }
}
