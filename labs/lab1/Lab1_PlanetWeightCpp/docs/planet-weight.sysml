package PlanetWeightCalculator {

    doc /*
     * Модель системы «Калькулятор веса на разных планетах» (вариант 4)
     * в текстовой нотации SysML v2.
     */

    private import ScalarValues::*;

    enum def WeightUnit {
        enum kilogram;
        enum pound;
    }

    attribute def Planet {
        attribute name : String;
        attribute gravity : Real;   // ускорение свободного падения, м/с2
    }

    attribute def InputData {
        attribute weightText : String;
        attribute unit : WeightUnit;
    }

    port def ModelChangedPort {
        out item modelChanged;
    }

    part def PlanetWeightModel {
        attribute weightKg : Real;
        attribute planets : Planet[8];
        port notify : ModelChangedPort;
    }

    part def MainWindow {
        attribute lastInput : InputData;
        port observe : ~ModelChangedPort;
    }

    part def InputDialog;

    part def WeightController;

    part def Application {
        part weightModel : PlanetWeightModel;
        part mainWindow : MainWindow;
        part inputDialog : InputDialog;
        part weightController : WeightController;

        // Активная модель: уведомление идет от модели к представлению.
        connect weightModel.notify to mainWindow.observe;
    }

    action def CalculatePlanetWeight {
        in input : InputData;

        first start;
        then action parse;        // преобразовать текст веса в число
        then action validate;     // проверить диапазон 1–500 кг
        then action recalculate;  // рассчитать вес на каждой планете
        then action notify;       // оповестить наблюдателей
        then done;
    }

    part variant4 : Application;
}
