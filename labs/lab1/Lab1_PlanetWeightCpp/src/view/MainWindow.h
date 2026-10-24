#pragma once

#include <windows.h>

#include <array>
#include <functional>
#include <optional>
#include <string>

#include "../model/IModelListener.h"
#include "InputDialog.h"

// Главная форма. Является наблюдателем активной модели: при изменении
// модели сама считывает из нее данные и обновляет метки.
class MainWindow : public IModelListener {
public:
    bool create(HINSTANCE instance, int showCommand);
    HWND handle() const { return hwnd_; }

    void setInputAction(std::function<void()> action) { inputAction_ = std::move(action); }
    const std::optional<InputData>& lastInput() const { return lastInput_; }
    void setLastInput(const InputData& input) { lastInput_ = input; }
    void showError(const std::wstring& message) const;

    void modelChanged(const PlanetWeightModel& model) override;

private:
    static LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT handleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
    void createControls();

    HWND hwnd_ = nullptr;
    HWND summaryLabel_ = nullptr;
    HWND inputButton_ = nullptr;
    std::array<HWND, 8> weightLabels_{};
    std::array<HWND, 8> weightKgLabels_{};
    std::array<HWND, 8> forceLabels_{};
    std::function<void()> inputAction_;
    // Последние введенные данные хранятся в главной форме.
    std::optional<InputData> lastInput_;
};
