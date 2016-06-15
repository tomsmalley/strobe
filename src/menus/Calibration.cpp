#include "Calibration.h"

#include <WProgram.h>

#include "HardwareController.h"

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
    , SerialMenuFunction('a', "Auto detect keys",
            &Calibration::autoDetect)
    };

void Calibration::printValues() {
    Serial.println();
    Serial.println("Calibration data");
    uint8_t noise = Persist::getNoiseFloor();
    Serial.print("Noise floor: ");
    Serial.println(noise);
    Serial.println("Per-key data:");
    Serial.println("+-----+-----+-----+-----+");
    Serial.println("| Key | Min | Max | SNR |");
    Serial.println("+-----+-----+-----+-----+");
    // For each key
    for (int i = 0; i < State::NUM_KEYS; i++) {
        // Only show ones set in this matrix
        if (Persist::keyIsInMatrix(i)) {
            uint8_t min = Persist::getCalMin(i);
            uint8_t max = Persist::getCalMax(i);
            uint8_t snr = (max - min)/noise;
            Serial.printf( "| %3u | %3u | %3u | %3u |"
                         , i
                         , min
                         , max
                         , snr
                         );
            Serial.println();
        }
    }
    Serial.println("+-----+-----+-----+-----+");
}

// Determines the maximum peak to peak noise
void Calibration::determineNoiseFloor() {
    /*

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
                uint8_t value = Key::strobeRead(i);
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
    Serial.println("+-----+-------+-----+-----+");
    Serial.println("| Key | Noise | Min | Max |");
    Serial.println("+-----+-------+-----+-----+");
    // For each key
    for (int i = 0; i < State::NUM_KEYS; i++) {
        // Only show ones set in this matrix
        if (Persist::keyIsInMatrix(i)) {
            uint8_t noise = maxValue[i] - minValue[i];
            // Find the max
            if (noise > maxNoise) {
                maxNoise = noise;
            }
            Serial.printf( "| %3u | %3u | %3u | %3u |"
                         , i
                         , noise
                         , minValue[i]
                         , maxValue[i]
                         );
            Serial.println();
        }
    }
    Serial.println("+-----+-------+-----+-----+");

    Serial.println("Saving maximum value to memory...");
    Persist::setNoiseFloor(maxNoise);

    */
}

// Function finds the min/max values of ADC readings of each key
// (pressed/unpressed)
void Calibration::calibrate() {
/*
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
                uint8_t value = Key::strobeRead(i);
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
*/
}

void Calibration::autoDetect() {

    Serial.println();
    Serial.println("Do not press any keys!");
    Serial.print("Determining noise floor");
    uint8_t lowMin[controller->NUM_ROWS][controller->NUM_COLS];
    uint8_t lowMax[controller->NUM_ROWS][controller->NUM_COLS];
    for (int i = 0; i < controller->NUM_ROWS; i++) {
        for (int j = 0; j < controller->NUM_COLS; j++) {
            lowMin[i][j] = 0xFF;
            lowMax[i][j] = 0x00;
        }
    }
    elapsedMillis time;
    uint16_t lastTime = time;
    // Do for 5 seconds
    while (time < 5000) {
        // Read key states
        for (int i = 0; i < controller->NUM_ROWS; i++) {
            controller->selectRow(i);
            for (int j = 0; j < controller->NUM_COLS; j++) {
                uint8_t value = controller->strobeRead(j);
                // Update min and max values
                if (value < lowMin[i][j]) { lowMin[i][j] = value; }
                if (value > lowMax[i][j]) { lowMax[i][j] = value; }
            }
        }
        // Add a period every second
        if (lastTime / 1000 < time / 1000) {
            Serial.print(".");
        }
        lastTime = time;
    }
    Serial.println(" done.");

    Serial.println();
    Serial.println("Press and hold each key in turn. Send 'q' when done.");
    // Set min values to 255 and max values to 0 for comparison checking
    uint8_t highMax[controller->NUM_ROWS][controller->NUM_COLS];
    for (int i = 0; i < controller->NUM_ROWS; i++) {
        for (int j = 0; j < controller->NUM_COLS; j++) {
            highMax[i][j] = 0x00;
        }
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
        // Record the maxima of each key
        for (int i = 0; i < controller->NUM_ROWS; i++) {
            controller->selectRow(i);
            for (int j = 0; j < controller->NUM_COLS; j++) {
                uint8_t value = controller->strobeRead(j);
                // Update max values
                if (value > highMax[i][j]) { highMax[i][j] = value; }
            }
        }
    }
    // Determine SNR
    uint8_t snr[controller->NUM_ROWS][controller->NUM_COLS];
    for (int i = 0; i < controller->NUM_ROWS; i++) {
        Serial.println();
        Serial.println("+-----------------------+");
        Serial.printf( "|         Row %u         |", i);
        Serial.println();
        Serial.println("+-----+-----+-----+------+-----+");
        Serial.println("|     | Low | Low | High | SNR |");
        Serial.println("| Col | Min | Max | Max  | SNR |");
        Serial.println("+-----+-----+-----+------+-----+");
        for (int j = 0; j < controller->NUM_COLS; j++) {
            uint8_t noise = 2 * (lowMax[i][j] - lowMin[i][j]);
            if (noise == 0) { noise = 1; }
            snr[i][j] = (highMax[i][j] - lowMin[i][j]) // signal
                      / noise;
            Serial.printf( "| %3u | %3u | %3u | %3u  | %3u |"
                         , j
                         , lowMin[i][j]
                         , lowMax[i][j]
                         , highMax[i][j]
                         , snr[i][j]
                         );
            Serial.println();
        }
        Serial.println("+-----+-----+-----+-----+");
    }

}
