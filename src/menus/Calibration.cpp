#include "Calibration.h"

#include <WProgram.h>

#include "controllers.h"
#include "Persist.h"
#include "Key.h"
#include "State.h"

const SerialMenuFunction Calibration::FUNCTIONS[ARRAY_SIZE] =
    { SerialMenuFunction('c', "Calibrate keys",
            &Calibration::calibrate)
    , SerialMenuFunction('r', "Reset calibration data",
            &Calibration::resetValues)
    , SerialMenuFunction('p', "Print calibration data",
            &Calibration::printValues)
    };

void Calibration::printValues() {
    Serial.println("Calibration values:");
    // For each key
    for (int i = 0; i < State::NUM_KEYS; i++) {
        if (Persist::keyIsInMatrix(i)) {
            Serial.print("Key: ");
            Serial.print(i);
            Serial.print(" Min: ");
            Serial.print(Persist::getCalMin(i));
            Serial.print(" Max: ");
            Serial.println(Persist::getCalMax(i));
        }
    }
}

void Calibration::resetValues() {
    for (int i = 0; i < State::NUM_KEYS; i++) {
        Persist::setCalMin(i, 0x00);
        Persist::setCalMax(i, 0xFF);
    }
    Serial.println("Memory reset.");
}

void Calibration::calibrate() {

    Serial.println("Calibrating... Press and hold each key in turn for 1 second, send 'q' when done.");

    // Set min values to 255 and max values to 0 for comparison checking
    for (int i = 0; i < State::NUM_KEYS; i++) {
        Persist::setCalMin(i, 0xFF);
        Persist::setCalMax(i, 0x00);
    }

    while (true) {

        // check for serial commands
        if (Serial.available()) {
            char incoming = Serial.read();
            if (incoming == 'q') {
                Serial.println("Done.");
                Calibration::printValues();
                return;
            }
        }

        // Poll each key checking for minima and maxima
        for (int i = 0; i < State::NUM_KEYS; i++) {
            uint8_t value = Key::strobeRead(i, controllers::row, controllers::column);
            delayMicroseconds(150);
            if (value < Persist::getCalMin(i)) {
                Persist::setCalMin(i, value);
            }
            if (value > Persist::getCalMax(i)) {
                Persist::setCalMax(i, value);
            }
        }
    }
}
