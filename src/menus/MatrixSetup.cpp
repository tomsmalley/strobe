#include "MatrixSetup.h"

#include <WProgram.h>

#include "Persist.h"
#include "State.h"

const SerialMenuFunction MatrixSetup::FUNCTIONS[ARRAY_SIZE] =
    { SerialMenuFunction('s', "Setup new matrix",
            &MatrixSetup::setMatrix)
    , SerialMenuFunction('r', "Reset matrix data",
            &MatrixSetup::resetValues)
    , SerialMenuFunction('p', "Print matrix data",
            &MatrixSetup::printValues)
    };

void MatrixSetup::printValues() {
    Serial.println();
    Serial.println("+-----+-----+-----+");
    Serial.println("| Key | Row | Col |");
    Serial.println("+-----+-----+-----+");
    // For each key
    for (int i = 0; i < State::NUM_KEYS; i++) {
        // Only show ones set in this matrix
        if (Persist::keyIsInMatrix(i)) {
            Serial.printf( "| %-3u | %02X  | %02X |"
                         , i
                         , Persist::getRow(i)
                         , Persist::getCol(i)
                         );
            Serial.println();
        }
    }
    Serial.println("+-----+-----+-----+");
}

void MatrixSetup::setMatrix() {
    Serial.println();
    Serial.println("Setting up the matrix. Send 'q' when done.");
    for (int i = 0; i < State::NUM_KEYS; i++) {
        Serial.print("Key: ");
        Serial.println(i);
        Serial.println("Send row:");
        bool rowSet = false;
        uint8_t row;
        while (!rowSet) {
            char incoming = getSerialCommand();
            if (incoming == 'q') {
                return;
            }
            // Convert input char to corresponding hex value
            if (incoming >= '0' && incoming <= '7') {
                row = incoming - 48;
                rowSet = true;
            } else {
                Serial.println("Not a recognised row");
            }
        }
        Serial.println("Send column:");
        bool colSet = false;
        uint8_t col;
        while (!colSet) {
            char incoming = getSerialCommand();
            if (incoming == 'q') {
                return;
            }
            // Convert input char to corresponding hex value
            if (incoming >= '0' && incoming <= '9') {
                col = incoming - 48;
                colSet = true;
            } else if (incoming >= 'A' && incoming <= 'F') {
                col = incoming - 55;
                colSet = true;
            } else {
                Serial.println("Not a recognised column");
            }
        }
        Persist::setMatrixPosition(i, row, col);
    }
}

void MatrixSetup::resetValues() {
    // Reset all keys
    for (int i = 0; i < State::NUM_KEYS; i++) {
        Persist::setKeyNotInMatrix(i);
    }
    Serial.println("Memory reset.");
}
