#ifndef MATRIXSETUP_H
#define MATRIXSETUP_H

#include "SerialMenu.h"

class MatrixSetup: public SerialMenu {
    public:
        MatrixSetup(): SerialMenu ("Matrix setup", MENU_ITEMS, NUM_ITEMS) {}
        static void setMatrix();
        static void printValues();
        static void resetValues();
    private:
        static const int NUM_ITEMS = 3;
        SerialMenuFunction MENU_ITEMS[NUM_ITEMS] =
            { SerialMenuFunction('s', "Setup new matrix", &MatrixSetup::setMatrix)
            , SerialMenuFunction('r', "Reset matrix data", &MatrixSetup::resetValues)
            , SerialMenuFunction('p', "Print matrix data", &MatrixSetup::printValues)
            };
};



#endif
