#pragma once

class PlanetWeightModel;

// Интерфейс наблюдателя активной модели. Представление реализует его,
// чтобы получать уведомления об изменении состояния модели.
class IModelListener {
public:
    virtual ~IModelListener() = default;
    virtual void modelChanged(const PlanetWeightModel& model) = 0;
};
