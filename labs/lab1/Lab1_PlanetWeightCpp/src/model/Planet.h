#pragma once

#include <array>
#include <string>

// Планета Солнечной системы и ускорение свободного падения
// на ее поверхности, м/с2.
struct Planet {
    std::wstring name;
    double gravity;
};

constexpr double EARTH_GRAVITY = 9.81;

// Планеты Солнечной системы от Меркурия до Нептуна.
inline const std::array<Planet, 8>& solarSystem() {
    static const std::array<Planet, 8> planets = {{
        {L"Меркурий", 3.70},
        {L"Венера", 8.87},
        {L"Земля", 9.81},
        {L"Марс", 3.72},
        {L"Юпитер", 24.79},
        {L"Сатурн", 10.44},
        {L"Уран", 8.69},
        {L"Нептун", 11.15},
    }};
    return planets;
}
