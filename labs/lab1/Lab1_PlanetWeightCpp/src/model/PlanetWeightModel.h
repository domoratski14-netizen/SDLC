#pragma once

#include <vector>

#include "IModelListener.h"
#include "Planet.h"
#include "WeightUnit.h"

// Активная модель: хранит вес человека на Земле, рассчитывает его вес
// на других планетах и сама уведомляет подписанные представления.
class PlanetWeightModel {
public:
    static constexpr double MIN_WEIGHT_KG = 1.0;
    static constexpr double MAX_WEIGHT_KG = 500.0;

    void addListener(IModelListener* listener);
    void removeListener(IModelListener* listener);

    // Проверяет вес на Земле, сохраняет его и оповещает наблюдателей.
    // При недопустимом значении выбрасывает std::invalid_argument.
    void calculate(double weight, WeightUnit unit);

    bool isCalculated() const { return calculated_; }
    double weight() const { return weight_; }
    WeightUnit unit() const { return unit_; }
    double weightKg() const { return weightKg_; }

    // Вес на планете в единицах, в которых он был введен.
    double weightOn(const Planet& planet) const;
    // Вес на планете в килограммах.
    double weightKgOn(const Planet& planet) const;
    // Сила тяжести, действующая на человека на планете, Н.
    double forceOn(const Planet& planet) const;

private:
    void fireModelChanged();

    std::vector<IModelListener*> listeners_;
    double weight_ = 0.0;
    WeightUnit unit_ = WeightUnit::Kilogram;
    double weightKg_ = 0.0;
    bool calculated_ = false;
};
