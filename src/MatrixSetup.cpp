#include "MatrixSetup.h"

#include <WProgram.h>

#include "globals.h"
#include "Key.h"

void MatrixSetup::printValues() {
    Serial.println("Matrix rows/columns:");
    // For each key
    for (int i = 0; i < NUM_KEYS; i++) {
        Serial.print("Key: ");
        Serial.print(i);
        Serial.print(" Row: ");
        Serial.print(Key::getRow(i));
        Serial.print(" Col: ");
        Serial.println(Key::getCol(i));
    }
}

void MatrixSetup::setMatrix() {

    Serial.println("Setting up the matrix. Send 'q' when done.");
    for (int i = 0; i < NUM_KEYS; i++) {
        Serial.print("Key: ");
        Serial.println(i);
        Serial.println("Send row:");
        bool rowSet = false;
        while (!rowSet) {
            if (Serial.available()) {
                char incoming = Serial.read();
                if (incoming == 'q') {
                    return;
                }
                if (incoming >= '0' && incoming <= '7') {
                    Key::setRow(i, incoming - 48);
                    rowSet = true;
                } else {
                    Serial.println("Not a recognised row");
                }
            }
        }
        Serial.println("Send column:");
        bool colSet = false;
        while (!colSet) {
            if (Serial.available()) {
                char incoming = Serial.read();
                if (incoming == 'q') {
                    return;
                }
                if (incoming >= '0' && incoming <= '9') {
                    Key::setCol(i, incoming - 48);
                    colSet = true;
                } else if (incoming >= 'A' && incoming <= 'F') {
                    Key::setCol(i, incoming - 55);
                    colSet = true;
                } else {
                    Serial.println("Not a recognised column");
                }
            }
        }
    }
}

void MatrixSetup::resetValues() {
    // Reset sets to non existant column and row
    for (int i = 0; i < NUM_KEYS; i++) {
        Key::setRow(i, -1);
        Key::setCol(i, -1);
    }
    Serial.println("Memory reset.");
}
