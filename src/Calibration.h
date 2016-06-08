#ifndef CALIBRATION_H
#define CALIBRATION_H

#include "SerialMenu.h"

class Calibration: public SerialMenu {
    public:
        Calibration(): SerialMenu ("Calibration", MENU_ITEMS, NUM_ITEMS) {}
        static void doThis();
        static void doThat();
        static void calibrate();
        static void printValues();
        static void resetValues();
    private:
        static const int NUM_ITEMS = 5;
        SerialMenuFunction MENU_ITEMS[NUM_ITEMS] =
            { SerialMenuFunction('t', "This", &Calibration::doThis)
            , SerialMenuFunction('h', "That", &Calibration::doThat)
            , SerialMenuFunction('c', "Calibrate keys", &Calibration::calibrate)
            , SerialMenuFunction('r', "Reset calibration data", &Calibration::resetValues)
            , SerialMenuFunction('p', "Print calibration data", &Calibration::printValues)
            };
};



#endif
