#ifndef MAINMENU_H
#define MAINMENU_H

#include <WProgram.h>

#include "SerialMenu.h"

#include "MatrixSetup.h"
#include "Calibration.h"
#include "KeyMapSetup.h"

class MainMenu: public SerialMenu {

    public:

        MainMenu(): SerialMenu ("Main menu", FUNCTIONS, ARRAY_SIZE) {}

    private:

        static const int ARRAY_SIZE = 3;
        static const SerialMenuFunction FUNCTIONS[ARRAY_SIZE];

        static MatrixSetup matrixSetupMenu;
        static Calibration calibrationMenu;
        static KeyMapSetup keyMapSetupMenu;

        static void matrixSetup();
        static void calibration();
        static void keyMapSetup();

};

#endif
