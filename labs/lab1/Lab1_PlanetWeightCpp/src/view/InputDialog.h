#pragma once

#include <windows.h>

#include <optional>
#include <string>

#include "../model/WeightUnit.h"

// Данные, введенные пользователем. Вес хранится как текст, чтобы
// восстановить в окне даже ошибочный ввод.
struct InputData {
    std::wstring weightText;
    WeightUnit unit = WeightUnit::Kilogram;
};

// Модальное окно ввода веса на Земле.
class InputDialog {
public:
    // Показывает окно и возвращает введенные данные либо пустое значение,
    // если ввод отменен.
    static std::optional<InputData> show(HWND owner, const std::optional<InputData>& initial);

private:
    InputDialog(HWND owner, const std::optional<InputData>& initial);
    static LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT handleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
    void createControls();
    void accept();

    HWND owner_;
    HWND hwnd_ = nullptr;
    HWND weightEdit_ = nullptr;
    HWND unitCombo_ = nullptr;
    std::optional<InputData> initial_;
    std::optional<InputData> result_;
    bool done_ = false;
};
