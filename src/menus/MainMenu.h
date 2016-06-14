#ifndef MAINMENU_H
#define MAINMENU_H

#include <WProgram.h>

#include "SerialMenu.h"

#include "Calibration.h"
#include "MatrixSetup.h"

class MainMenu: public SerialMenu {

    public:

        MainMenu(): SerialMenu ("Main menu.", FUNCTIONS, ARRAY_SIZE) {}

    private:

        static const int ARRAY_SIZE = 2;
        static const SerialMenuFunction FUNCTIONS[ARRAY_SIZE];

        static Calibration calibrationMenu;
        static MatrixSetup matrixSetupMenu;

        static void calibration();
        static void matrixSetup();

};

#endif
