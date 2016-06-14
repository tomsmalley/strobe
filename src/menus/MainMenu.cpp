#include "MainMenu.h"

const SerialMenuFunction MainMenu::FUNCTIONS[ARRAY_SIZE] =
                { SerialMenuFunction('c', "Calibration",
                        &MainMenu::calibration)
                , SerialMenuFunction('m', "Matrix setup",
                        &MainMenu::matrixSetup)
                };

Calibration MainMenu::calibrationMenu = Calibration();

MatrixSetup MainMenu::matrixSetupMenu = MatrixSetup();

void MainMenu::calibration() {
    calibrationMenu.start();
}

void MainMenu::matrixSetup() {
    matrixSetupMenu.start();
}
