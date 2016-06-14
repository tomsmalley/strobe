#ifndef MATRIXSETUP_H
#define MATRIXSETUP_H

#include "SerialMenu.h"

class MatrixSetup: public SerialMenu {

    public:

        MatrixSetup(): SerialMenu ("Matrix setup", FUNCTIONS, ARRAY_SIZE) {}

    private:

        static const int ARRAY_SIZE = 3;
        static const SerialMenuFunction FUNCTIONS[ARRAY_SIZE];

        static void setMatrix();
        static void printValues();
        static void resetValues();

};

#endif
