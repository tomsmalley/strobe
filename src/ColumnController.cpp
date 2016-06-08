#include "ColumnController.h"

#include <WProgram.h>

ColumnController::ColumnController() {
    // Set pinmodes
    for (int i = 0; i < NUM_COLUMNS; i++) {
        pinMode(columnPin(i), OUTPUT);
    }
}

void ColumnController::setHigh(uint8_t columnID) {
    digitalWrite(columnPin(columnID), HIGH);
}

void ColumnController::setLow(uint8_t columnID) {
    digitalWrite(columnPin(columnID), LOW);
}

// Digital out pins (don't use LED_PIN or MUX_CONTROL_PINs)
// defined in function so we can control bad access
int ColumnController::columnPin(uint8_t columnID) {
    switch (columnID) {
        case 0:
            return 9;
            break;
        case 1:
            return 10;
            break;
        case 2:
            return 11;
            break;
        case 3:
            return 12;
            break;
        default:
            return 8;
            break;
    }
}
