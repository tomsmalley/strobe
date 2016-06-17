#include "KeyMapSetup.h"

#include <WProgram.h>

#include "Persist.h"
#include "State.h"

const SerialMenuFunction KeyMapSetup::FUNCTIONS[ARRAY_SIZE] =
    { SerialMenuFunction('s', "Setup new key map",
            &KeyMapSetup::setKeyMap)
    , SerialMenuFunction('r', "Reset key map data",
            &KeyMapSetup::resetValues)
    , SerialMenuFunction('p', "Print key map data",
            &KeyMapSetup::printValues)
    };

void KeyMapSetup::printValues() {
    /*
    Serial.println();
    Serial.println("+-----+----+----+----+--LAYER--+----+----+----+");
    Serial.println("| Key | 0  | 1  | 2  | 3  | 4  | 5  | 6  | 7  |");
    Serial.println("+-----+----+----+----+----+----+----+----+----+");
    // Go through keys
    for (int i = 0; i < State::NUM_KEYS; i++) {
        // Only show ones set in this matrix
        if (Persist::keyIsInMatrix(i)) {
            Serial.printf("| %3u |", i);
            // Go through layers
            for (int layer = 0; layer < 8; layer++) {
                Serial.printf(" %02X |", Persist::getMapping(i, layer));
            }
            Serial.println();
        }
    }
    Serial.println("+-----+----+----+----+----+----+----+----+----+");
    // For each key
    */
}

// TODO this function is lazily implemented, needs rewriting
void KeyMapSetup::setKeyMap() {
    Serial.println();
    Serial.println("Setting up the matrix. Send 'q' when done.");

    Serial.println("+-----+-----+-----+-----+---LAYER---+-----+-----+-----+");
    Serial.println("| Key |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |");
    Serial.println("+-----+-----+-----+-----+-----+-----+-----+-----+-----+");

    for (int i = 0; i < State::NUM_KEYS; i++) {
        // Print key
        Serial.printf("| %3u |", i);
        // Layers
        for (int l = 0; l < 8; l++) {
            // Print current mapping
            Serial.printf("%02X-", Persist::getMapping(i, l));
            bool firstSet = false;
            bool secondSet = false;
            uint8_t first;
            uint8_t second;
            while (!firstSet && !secondSet) {
                firstSet = false;
                secondSet = false;
                char firstChar = getSerialCommand();
                char secondChar = getSerialCommand();
                if (firstChar == 'q' || secondChar == 'q') {
                    return;
                } else if (firstChar == 'n' && secondChar == 'n') {
                    break;
                } else {
                    // Convert input char to corresponding hex value
                    if (firstChar >= '0' && firstChar <= '9') {
                        first = firstChar - 48;
                        firstSet = true;
                    } else if (firstChar >= 'A' && firstChar <= 'F') {
                        first = firstChar - 55;
                        firstSet = true;
                    }
                    if (secondChar >= '0' && secondChar <= '9') {
                        second = secondChar - 48;
                        secondSet = true;
                    } else if (secondChar >= 'A' && secondChar <= 'F') {
                        second = secondChar - 55;
                        secondSet = true;
                    }
                }
            }
            if (firstSet && secondSet) {
                uint8_t mapping = (first << 4) + second;
                Persist::setMapping(i, l, mapping);
            }
            // Print final mapping
            Serial.printf("%02X|", Persist::getMapping(i, l));
        }
        Serial.println();
    }
    Serial.println("+-----+-----+-----+-----+-----+-----+-----+-----+-----+");
}

void KeyMapSetup::resetValues() {
    // Reset all keys
    for (int i = 0; i < State::NUM_KEYS; i++) {
        for (int l = 0; l < 8; l++) {
            Persist::setMapping(i, l, 0);
        }
    }
    Serial.println("Memory reset.");
}
