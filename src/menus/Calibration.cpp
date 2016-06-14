#include "Calibration.h"

#include <WProgram.h>

#include "controllers.h"
#include "Persist.h"
#include "Key.h"
#include "State.h"

const SerialMenuFunction Calibration::FUNCTIONS[ARRAY_SIZE] =
    { SerialMenuFunction('n', "Determine noise floor",
            &Calibration::determineNoiseFloor)
    , SerialMenuFunction('c', "Calibrate keys",
            &Calibration::calibrate)
    , SerialMenuFunction('p', "Print calibration data",
            &Calibration::printValues)
    };

void Calibration::printValues() {
    Serial.println();
    Serial.println("Calibration data");
    uint8_t noise = Persist::getNoiseFloor();
    Serial.print("Noise floor: ");
    Serial.println(noise);
    Serial.println("Per-key data:");
    // For each key
    for (int i = 0; i < State::NUM_KEYS; i++) {
        if (Persist::keyIsInMatrix(i)) {
            uint8_t min = Persist::getCalMin(i);
            uint8_t max = Persist::getCalMax(i);
            uint8_t snr = (max - min)/noise;
            Serial.print("Key: ");
            Serial.print(i);
            Serial.print(" Min: ");
            Serial.print(min);
            Serial.print(" Max: ");
            Serial.print(max);
            Serial.print(" *** Signal/noise ratio: ");
            Serial.println(snr);
        }
    }
}

// Determines the maximum peak to peak noise
void Calibration::determineNoiseFloor() {

    Serial.println();
    Serial.print("Determining noise floor: do not press any keys");

    uint8_t minValue[128];
    uint8_t maxValue[128];
    for (int i = 0; i < State::NUM_KEYS; i++) {
        minValue[i] = 0xFF;
        maxValue[i] = 0x00;
    }

    elapsedMillis time;
    uint16_t lastTime = time;
    uint32_t measurements = 0;
    // Do for 5 seconds
    while (time < 5000) {
        measurements += 1;
        // Read key states
        for (int i = 0; i < State::NUM_KEYS; i++) {
            if (Persist::keyIsInMatrix(i)) {
                uint8_t value = Key::strobeRead(i, controllers::row,
                        controllers::column);
                delayMicroseconds(150);
                // Update min and max values
                if (value < minValue[i]) {
                    minValue[i] = value;
                }
                if (value > maxValue[i]) {
                    maxValue[i] = value;
                }
            }
        }
        // Add a period every second
        if (lastTime / 1000 < time / 1000) {
            Serial.print(".");
        }
        lastTime = time;
    }
    Serial.print(" done. Took ");
    Serial.print(measurements);
    Serial.println(" measurements on each key.");

    uint8_t maxNoise = 0;
    for (int i = 0; i < State::NUM_KEYS; i++) {
        if (Persist::keyIsInMatrix(i)) {
            uint8_t noise = maxValue[i] - minValue[i];
            // Find the max
            if (noise > maxNoise) {
                maxNoise = noise;
            }
            // Print the measurements
            Serial.print("Key: ");
            Serial.print(i);
            Serial.print(" Noise: ");
            Serial.print(noise);
            Serial.print(" *** Measured (min/max): ");
            Serial.print(minValue[i]);
            Serial.print("/");
            Serial.println(maxValue[i]);
        }
    }
    Serial.println("Saving maximum value to memory...");
    Persist::setNoiseFloor(maxNoise);

}

// Function finds the min/max values of ADC readings of each key
// (pressed/unpressed)
void Calibration::calibrate() {

    Serial.println();
    Serial.println("Calibrating... Press and hold each key in turn for 1"
            " second, send 'q' when done.");

    // Set min values to 255 and max values to 0 for comparison checking
    uint8_t minValue[128];
    uint8_t maxValue[128];
    for (int i = 0; i < State::NUM_KEYS; i++) {
        minValue[i] = 0xFF;
        maxValue[i] = 0x00;
    }

    // Loop until done (user controlled)
    bool done = false;
    while (!done) {
        // check for serial commands
        if (Serial.available()) {
            char incoming = Serial.read();
            if (incoming == 'q') {
                done = true;
            }
        }

        // Poll each key checking for minima and maxima
        for (int i = 0; i < State::NUM_KEYS; i++) {
            if (Persist::keyIsInMatrix(i)) {
                uint8_t value = Key::strobeRead(i, controllers::row,
                        controllers::column);
                delayMicroseconds(150);
                // Update min and max values
                if (value < minValue[i]) {
                    minValue[i] = value;
                }
                if (value > maxValue[i]) {
                    maxValue[i] = value;
                }
            }
        }
    }

    uint8_t noiseFloor = Persist::getNoiseFloor();
    for (int i = 0; i < State::NUM_KEYS; i++) {
        if (Persist::keyIsInMatrix(i)) {
            uint8_t min = minValue[i] + noiseFloor/2;
            uint8_t max = maxValue[i] - noiseFloor/2;

            Persist::setCalMin(i, min);
            Persist::setCalMax(i, max);
        }
    }

    printValues();

}
