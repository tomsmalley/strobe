#include "Persist.h"

#include <EEPROM.h>

/* PRIVATE HELPER FUNCTIONS */

// Mask off bottom 7 bits
uint8_t Persist::maskKeyID(uint8_t keyID) {
    return (keyID & 0x7F);
}

// Mask off bottom 3 bits
uint8_t Persist::maskLayer(uint8_t layer) {
    return (layer & 0x07);
}

/* USER SETTINGS */

uint8_t Persist::getDeadZone() {
    return EEPROM.read(BLOCK_SIZE * MEM_USER_SETTINGS_BLOCK +
            MEM_SETTINGS_DEADZONE_OFFSET);
}

void Persist::setDeadZone(uint8_t deadZone) {
    EEPROM.update(BLOCK_SIZE * MEM_USER_SETTINGS_BLOCK +
            MEM_SETTINGS_DEADZONE_OFFSET, deadZone);
}

uint8_t Persist::getSensitivity() {
    return EEPROM.read(BLOCK_SIZE * MEM_USER_SETTINGS_BLOCK +
            MEM_SETTINGS_SENSITIVITY_OFFSET);
}

void Persist::setSensitivity(uint8_t sensitivity) {
    EEPROM.update(BLOCK_SIZE * MEM_USER_SETTINGS_BLOCK +
            MEM_SETTINGS_SENSITIVITY_OFFSET, sensitivity);
}

/* MATRIX POSITION (ROW AND COLUMN) FUNCTIONS */

bool Persist::keyIsInMatrix(uint8_t keyID) {
    uint8_t matrix = EEPROM.read(BLOCK_SIZE * MEM_KEY_MATRIX_BLOCK +
            maskKeyID(keyID));
    // Get the bit
    return (matrix >> 7);
}

// Row bits (RRR) and column (CCCC) are stored in a single byte: 1RRR CCCC
void Persist::setMatrixPosition(uint8_t keyID, uint8_t row, uint8_t col) {
    uint8_t rowMask = 0x07;
    uint8_t colMask = 0x0F;
    uint8_t matrixPosition = 0;
    // Set left bit to 1 to indicate the key is used
    matrixPosition |= 1 << 7;
    // Set row bits
    matrixPosition |= (row & rowMask) << 4;
    // Set column bits
    matrixPosition |= col & colMask;
    EEPROM.update(BLOCK_SIZE * MEM_KEY_MATRIX_BLOCK + maskKeyID(keyID),
            matrixPosition);
}

uint8_t Persist::getRow(uint8_t keyID) {
    uint8_t matrix = EEPROM.read(BLOCK_SIZE * MEM_KEY_MATRIX_BLOCK +
            maskKeyID(keyID));
    // Shift the three row bits and mask them
    uint8_t mask = 0x07;
    return (matrix >> 4) & mask;
}

uint8_t Persist::getCol(uint8_t keyID) {
    uint8_t matrix = EEPROM.read(BLOCK_SIZE * MEM_KEY_MATRIX_BLOCK +
            maskKeyID(keyID));
    // Mask off the four column bits
    uint8_t mask = 0x0F;
    return matrix & mask;
}

/* CALIBRATION FUNCTIONS */

uint8_t Persist::getCalMin(uint8_t keyID) {
    return EEPROM.read(BLOCK_SIZE * MEM_KEY_CAL_MIN_BLOCK + maskKeyID(keyID));
}

void Persist::setCalMin(uint8_t keyID, uint8_t value) {
    EEPROM.update(BLOCK_SIZE * MEM_KEY_CAL_MIN_BLOCK + maskKeyID(keyID),
            value);
}

uint8_t Persist::getCalMax(uint8_t keyID) {
    return EEPROM.read(BLOCK_SIZE * MEM_KEY_CAL_MAX_BLOCK + maskKeyID(keyID));
}

void Persist::setCalMax(uint8_t keyID, uint8_t value) {
    EEPROM.update(BLOCK_SIZE * MEM_KEY_CAL_MAX_BLOCK + maskKeyID(keyID),
            value);
}

/* KEYMAP FUNCTIONS */

uint8_t Persist::getMapping(uint8_t keyID, uint8_t layer) {
    return EEPROM.read(BLOCK_SIZE * MEM_KEYMAP_START_BLOCK * maskLayer(layer) +
            maskKeyID(keyID));
}

void Persist::setMapping(uint8_t keyID, uint8_t layer, uint8_t mapID) {
    EEPROM.update(BLOCK_SIZE * MEM_KEYMAP_START_BLOCK * maskLayer(layer) +
            maskKeyID(keyID), mapID);
}
