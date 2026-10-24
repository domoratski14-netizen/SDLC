#include "PlanetWeightModel.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

void PlanetWeightModel::addListener(IModelListener* listener) {
    listeners_.push_back(listener);
}

void PlanetWeightModel::removeListener(IModelListener* listener) {
    listeners_.erase(std::remove(listeners_.begin(), listeners_.end(), listener),
                     listeners_.end());
}

void PlanetWeightModel::calculate(double weight, WeightUnit unit) {
    const double kg = toKilograms(weight, unit);
    if (std::isnan(kg) || kg < MIN_WEIGHT_KG || kg > MAX_WEIGHT_KG) {
        throw std::invalid_argument(
            "Вес на Земле должен находиться в диапазоне от 1 до 500 кг "
            "(от 2,2 до 1102 фунтов).");
    }
    weight_ = weight;
    unit_ = unit;
    weightKg_ = kg;
    calculated_ = true;
    fireModelChanged();
}

void PlanetWeightModel::fireModelChanged() {
    for (IModelListener* listener : listeners_) {
        listener->modelChanged(*this);
    }
}

double PlanetWeightModel::weightOn(const Planet& planet) const {
    return weight_ * planet.gravity / EARTH_GRAVITY;
}

double PlanetWeightModel::weightKgOn(const Planet& planet) const {
    return weightKg_ * planet.gravity / EARTH_GRAVITY;
}

double PlanetWeightModel::forceOn(const Planet& planet) const {
    return weightKg_ * planet.gravity;
}
