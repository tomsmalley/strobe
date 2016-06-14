#ifndef CALIBRATION_H
#define CALIBRATION_H

#include "SerialMenu.h"

class Calibration: public SerialMenu {

    public:

        Calibration(): SerialMenu ("Calibration", FUNCTIONS, ARRAY_SIZE) {}

    private:

        static const int ARRAY_SIZE = 3;
        static const SerialMenuFunction FUNCTIONS[ARRAY_SIZE];

        static void calibrate();
        static void printValues();
        static void resetValues();

};

#endif
