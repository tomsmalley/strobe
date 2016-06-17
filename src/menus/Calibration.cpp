#include "Calibration.h"

#include <WProgram.h>

#include "HardwareController.h"

#include "Persist.h"
#include "Key.h"
#include "State.h"

const SerialMenuFunction Calibration::FUNCTIONS[ARRAY_SIZE] =
    { SerialMenuFunction('c', "Calibrate keys",
            &Calibration::calibrate)
    , SerialMenuFunction('p', "Print calibration data",
            &Calibration::printValues)
    , SerialMenuFunction('a', "Auto detect keys",
            &Calibration::autoDetectKeys)
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
    for (int i = 0; i < controller->NUM_ROWS; i++) {
        for (int j = 0; j < controller->NUM_COLS; j++) {
            // Only show ones set in this matrix
            if (Persist::matrixPositionActive(i, j)) {
                uint8_t min = Persist::getCalMin(i, j);
                uint8_t max = Persist::getCalMax(i, j);
                uint8_t snr = (max - min)/noise;
                Serial.printf( "| %3u | %3u | %3u | %3u |"
                             , Persist::getUserID(i, j)
                             , min
                             , max
                             , snr
                             );
                Serial.println();
            }
        }
    }
    Serial.println("+-----+-----+-----+-----+");

}

// Function finds the min/max values of ADC readings of each key
// (pressed/unpressed)
void Calibration::calibrate() {
    fullRoutine(false);
}

void Calibration::autoDetectKeys() {
    fullRoutine(true);
}

void Calibration::fullRoutine(bool autoDetect) {

    // Noise floor
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

    // Peak values
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
    Serial.println("Done.");
    Serial.println();

    // Determine SNR
    uint8_t snr[controller->NUM_ROWS][controller->NUM_COLS];
    for (int i = 0; i < controller->NUM_ROWS; i++) {
        for (int j = 0; j < controller->NUM_COLS; j++) {
            uint16_t noise = lowMax[i][j] - lowMin[i][j];
            uint16_t signal = highMax[i][j] - lowMin[i][j] - noise;
            if (noise == 0) { noise = 1; }
            // Ignore low measurements seeming like they have high SNR
            if (highMax[i][j] < 50) {
                snr[i][j] = 0;
            } else {
                snr[i][j] = signal / noise;
            }
        }
    }

    if (autoDetect) {
        // Save keys with SNR above threshold as active
        const uint8_t SNR_THRESHOLD = 5;
        uint8_t keyCount = 0;
        for (int i = 0; i < controller->NUM_ROWS; i++) {
            for (int j = 0; j < controller->NUM_COLS; j++) {
                if (snr[i][j] > SNR_THRESHOLD) {
                    Persist::setMatrixPositionActive(i, j, true);
                    keyCount++;
                } else {
                    Persist::setMatrixPositionActive(i, j, false);
                }
            }
        }
        // Tell the user
        Serial.printf("Detected %u keys.", keyCount);
        Serial.println();
    }

    // Key table with SNR
    Serial.println();
    Serial.println("+-----+-----+-----+--------+--------+---------+");
    Serial.println("| ROW | COL | SNR | LOWMIN | LOWMAX | HIGHMAX |");
    Serial.println("+-----+-----+-----+--------+--------+---------+");
    for (int i = 0; i < controller->NUM_ROWS; i++) {
        uint8_t colCount = 0;
        for(int j = 0; j < controller->NUM_COLS; j++) {
            if (Persist::matrixPositionActive(i, j)) {
                Serial.print("| ");
                if (colCount == 0) {
                    Serial.printf("%3u", i);
                } else {
                    Serial.print("   ");
                }
                Serial.printf( " | %3u | %3u | %6u | %6u | %7u |"
                             , j
                             , snr[i][j]
                             , lowMin[i][j]
                             , lowMax[i][j]
                             , highMax[i][j]
                             );
                Serial.println();
                colCount++;
            }
        }
        if (colCount > 0) {
            Serial.println("+-----+-----+-----+--------+--------+---------+");
        }
    }

    // Noise floor
    Serial.println();
    uint8_t maxNoise = 0;
    for (int i = 0; i < controller->NUM_ROWS; i++) {
        for(int j = 0; j < controller->NUM_COLS; j++) {
            // Only show ones set in this matrix
            if (Persist::matrixPositionActive(i, j)) {
                uint8_t noise = lowMax[i][j] - lowMin[i][j];
                // Find the max
                if (noise > maxNoise) { maxNoise = noise; }
            }
        }
    }
    Serial.printf("Saving noise floor (%u) to memory...", maxNoise);
    Serial.println();
    Persist::setNoiseFloor(maxNoise);

    // Calibration
    Serial.println();
    uint8_t noiseFloor = Persist::getNoiseFloor();
    for (int i = 0; i < controller->NUM_ROWS; i++) {
        for(int j = 0; j < controller->NUM_COLS; j++) {
            // Only show ones set in this matrix
            if (Persist::matrixPositionActive(i, j)) {
                uint8_t min = lowMin[i][j] + noiseFloor/2;
                uint8_t max = highMax[i][j] - noiseFloor/2;

                Persist::setCalMin(i, j, min);
                Persist::setCalMax(i, j, max);
            }
        }
    }

}
