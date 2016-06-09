#include "Key.h"

#include <EEPROM.h>
#include <WProgram.h>

#include "controllers.h"

const int Key::EEPROM_KEY_SIZE = 9;
const int Key::MEM_ROW_OFFSET = 0;
const int Key::MEM_COL_OFFSET = 1;
const int Key::MEM_CAL_MIN_OFFSET = 2;
const int Key::MEM_CAL_MAX_OFFSET = 3;
const int Key::MEM_BASE_LAYER_OFFSET = 4;

Key::Key() {
    Serial.print("create");
}

int Key::getKeyAddress(int8_t keyID) {
    return keyID * EEPROM_KEY_SIZE;
}

int8_t Key::getRow(int8_t keyID) {
    return EEPROM.read(getKeyAddress(keyID));
}

void Key::setRow(int8_t keyID, int8_t row) {
    EEPROM.write(getKeyAddress(keyID) + MEM_ROW_OFFSET, row);
}

int8_t Key::getCol(int8_t keyID) {
    return EEPROM.read(getKeyAddress(keyID) + MEM_COL_OFFSET);
}

void Key::setCol(int8_t keyID, int8_t column) {
    EEPROM.write(getKeyAddress(keyID) + MEM_COL_OFFSET, column);
}


uint8_t Key::getCalMin(int8_t keyID) {
    return EEPROM.read(getKeyAddress(keyID) + MEM_CAL_MIN_OFFSET);
}

void Key::setCalMin(int8_t keyID, uint8_t value) {
    EEPROM.write(getKeyAddress(keyID) + MEM_CAL_MIN_OFFSET, value);
}

uint8_t Key::getCalMax(int8_t keyID) {
    return EEPROM.read(getKeyAddress(keyID) + MEM_CAL_MAX_OFFSET);
}

void Key::setCalMax(int8_t keyID, uint8_t value) {
    EEPROM.write(getKeyAddress(keyID) + MEM_CAL_MAX_OFFSET, value);
}

// Gets what the key should actually do given the current layer
uint8_t Key::getMapping(int8_t keyID, uint8_t layer) {
    return EEPROM.read(getKeyAddress(keyID) + MEM_BASE_LAYER_OFFSET + layer);
}

void Key::setMapping(int8_t keyID, uint8_t layer, uint8_t mapping) {
    EEPROM.write(getKeyAddress(keyID) + MEM_BASE_LAYER_OFFSET + layer, mapping);
}

/**
 * Strobe a column and read the row voltage. Must wait long enough for rows to
 * relax between function calls!
 * @param keyID key id number
 * @return ADC reading for row voltage
 */
uint8_t Key::strobeRead(int8_t keyID) {
    // Check the ID is positive
    if (keyID < 0) {
        return 0;
    }
    // Get the row and column IDs
    int8_t rowID = getRow(keyID);
    int8_t colID = getCol(keyID);
    // Check for invalid address
    if (rowID < 0 || colID < 0) {
        return 0;
    }

    uint8_t value;
    // Interrupts can affect delayMicroseconds
    noInterrupts();
    // Select the row on multiplexer
    controllers::row.select(rowID);
    // Set column high ("strobe")
    controllers::column.setHigh(colID);
    // Wait for amplifier to catch up
    delayMicroseconds(3);
    // Read the row value
    value = controllers::row.read();
    // Set column low
    controllers::column.setLow(colID);
    // Turn back on interrupts and wait for row to relax to 0V
    interrupts();
    return value;
}

/**
 * Normalise a 0-255 ADC reading using calibration values (feature scaling)
 * @param keyID key id
 * @param value ADC reading
 * @return normalised reading
 */
float Key::normalise(int8_t keyID, uint8_t value) {
    float f = (value - getCalMin(keyID))
            / (float)(getCalMax(keyID) - getCalMin(keyID));
    // Clamp to between 0 and 1
    if (f > 1) { f = 1; }
    if (f < 0) { f = 0; }
    return f;
}
