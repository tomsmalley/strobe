#include "MatrixSetup.h"

#include <WProgram.h>

#include "Persist.h"
#include "State.h"

void MatrixSetup::printValues() {
    Serial.println("Matrix rows/columns:");
    // For each key
    for (int i = 0; i < State::NUM_KEYS; i++) {
        Serial.print("Key: ");
        Serial.print(i);
        Serial.print(" Row: ");
        Serial.print(Persist::getRow(i));
        Serial.print(" Col: ");
        Serial.println(Persist::getCol(i));
    }
}

void MatrixSetup::setMatrix() {

    Serial.println("Setting up the matrix. Send 'q' when done.");
    for (int i = 0; i < State::NUM_KEYS; i++) {
        Serial.print("Key: ");
        Serial.println(i);
        Serial.println("Send row:");
        bool rowSet = false;
        uint8_t row;
        while (!rowSet) {
            if (Serial.available()) {
                char incoming = Serial.read();
                if (incoming == 'q') {
                    return;
                }
                if (incoming >= '0' && incoming <= '7') {
                    row = incoming - 48;
                    rowSet = true;
                } else {
                    Serial.println("Not a recognised row");
                }
            }
        }
        Serial.println("Send column:");
        bool colSet = false;
        uint8_t col;
        while (!colSet) {
            if (Serial.available()) {
                char incoming = Serial.read();
                if (incoming == 'q') {
                    return;
                }
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
        }
        Persist::setMatrixPosition(i, row, col);
    }
}

void MatrixSetup::resetValues() {
    // Reset sets to non existant column and row
    for (int i = 0; i < State::NUM_KEYS; i++) {
        Persist::setMatrixPosition(i, 0, 0);
    }
    Serial.println("Memory reset.");
}
