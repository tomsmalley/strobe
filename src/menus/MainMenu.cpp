#include "MainMenu.h"

const SerialMenuFunction MainMenu::FUNCTIONS[ARRAY_SIZE] =
                { SerialMenuFunction('c', "Calibration",
                        &MainMenu::calibration)
                , SerialMenuFunction('m', "Matrix setup",
                        &MainMenu::matrixSetup)
                , SerialMenuFunction('k', "Key map setup",
                        &MainMenu::keyMapSetup)
                };

MatrixSetup MainMenu::matrixSetupMenu = MatrixSetup();
Calibration MainMenu::calibrationMenu = Calibration();
KeyMapSetup MainMenu::keyMapSetupMenu = KeyMapSetup();

void MainMenu::matrixSetup() {
    matrixSetupMenu.start();
}

void MainMenu::calibration() {
    calibrationMenu.start();
}

void MainMenu::keyMapSetup() {
    keyMapSetupMenu.start();
}
