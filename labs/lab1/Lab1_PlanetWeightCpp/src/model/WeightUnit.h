#pragma once

#include <string>

// Единицы измерения веса, поддерживаемые приложением.
enum class WeightUnit {
    Kilogram,
    Pound
};

// Количество килограммов в одной единице измерения.
inline double kilogramsPerUnit(WeightUnit unit) {
    return unit == WeightUnit::Kilogram ? 1.0 : 0.45359237;
}

inline double toKilograms(double value, WeightUnit unit) {
    return value * kilogramsPerUnit(unit);
}

inline double fromKilograms(double kilograms, WeightUnit unit) {
    return kilograms / kilogramsPerUnit(unit);
}

inline std::wstring unitTitle(WeightUnit unit) {
    return unit == WeightUnit::Kilogram ? L"кг" : L"фунты";
}
