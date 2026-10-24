#include "WeightController.h"

#include <windows.h>

#include <cwchar>
#include <stdexcept>

#include "../model/PlanetWeightModel.h"
#include "../view/InputDialog.h"
#include "../view/MainWindow.h"

namespace {
std::wstring fromUtf8(const char* text) {
    const int size = MultiByteToWideChar(CP_UTF8, 0, text, -1, nullptr, 0);
    std::wstring result(size, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, text, -1, result.data(), size);
    result.resize(size - 1);
    return result;
}
}  // namespace

WeightController::WeightController(PlanetWeightModel& model, MainWindow& view)
    : model_(model), view_(view) {
    view_.setInputAction([this] { onInputData(); });
}

void WeightController::onInputData() {
    std::optional<InputData> input = view_.lastInput();
    while (true) {
        input = InputDialog::show(view_.handle(), input);
        if (!input) {
            return;  // пользователь отменил ввод
        }
        view_.setLastInput(*input);  // запоминаем последние данные в главной форме
        try {
            const double weight = parseWeight(input->weightText);
            model_.calculate(weight, input->unit);
            return;
        } catch (const std::invalid_argument& ex) {
            view_.showError(fromUtf8(ex.what()));
        }
    }
}

double WeightController::parseWeight(const std::wstring& text) {
    std::wstring s = text;
    for (wchar_t& c : s) {
        if (c == L',') c = L'.';
    }
    const size_t first = s.find_first_not_of(L" \t");
    if (first == std::wstring::npos) {
        throw std::invalid_argument("Поле «Вес на Земле» не заполнено.");
    }
    const wchar_t* begin = s.c_str() + first;
    wchar_t* end = nullptr;
    const double value = std::wcstod(begin, &end);
    while (end && (*end == L' ' || *end == L'\t')) ++end;
    if (end == begin || *end != L'\0') {
        throw std::invalid_argument("Вес должен быть числом, например 72,5.");
    }
    return value;
}
