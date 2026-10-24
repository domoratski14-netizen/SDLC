#pragma once

#include <string>

class PlanetWeightModel;
class MainWindow;

// Контроллер: обрабатывает действия пользователя, проверяет введенные
// данные и изменяет модель. Представление контроллер не обновляет –
// при активной модели это делает сама модель через механизм уведомлений.
class WeightController {
public:
    WeightController(PlanetWeightModel& model, MainWindow& view);

    // Преобразует текст в число; при ошибке выбрасывает std::invalid_argument.
    static double parseWeight(const std::wstring& text);

private:
    void onInputData();  // обработчик кнопки «Ввести данные»

    PlanetWeightModel& model_;
    MainWindow& view_;
};
