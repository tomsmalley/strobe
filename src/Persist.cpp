#include "Persist.h"

#include <EEPROM.h>

/* PRIVATE HELPER FUNCTIONS */

// Mask off bottom 7 bits
uint8_t Persist::maskKeyID(uint8_t keyID) {
    return (keyID & 0x7F);
}

// Mask off bottom 3 bits
uint8_t Persist::maskLayer(uint8_t layer) {
    return layer < NUM_LAYERS ? layer : 0;
}

/* USER SETTINGS */

uint8_t Persist::getDeadZone() {
    return EEPROM.read(MEM_USER_SETTINGS + MEM_SETTINGS_DEADZONE_OFFSET);
}

void Persist::setDeadZone(uint8_t deadZone) {
    EEPROM.update(MEM_USER_SETTINGS + MEM_SETTINGS_DEADZONE_OFFSET, deadZone);
}

uint8_t Persist::getSensitivity() {
    return EEPROM.read(MEM_USER_SETTINGS + MEM_SETTINGS_SENSITIVITY_OFFSET);
}

void Persist::setSensitivity(uint8_t sensitivity) {
    EEPROM.update(MEM_USER_SETTINGS + MEM_SETTINGS_SENSITIVITY_OFFSET,
            sensitivity);
}

uint8_t Persist::getMinThreshold() {
    return EEPROM.read(MEM_USER_SETTINGS + MEM_SETTINGS_MIN_THRESHOLD_OFFSET);
}

void Persist::setMinThreshold(uint8_t threshold) {
    EEPROM.update(MEM_USER_SETTINGS + MEM_SETTINGS_MIN_THRESHOLD_OFFSET,
            threshold);
}

uint8_t Persist::getMaxThreshold() {
    return EEPROM.read(MEM_USER_SETTINGS + MEM_SETTINGS_MAX_THRESHOLD_OFFSET);
}

void Persist::setMaxThreshold(uint8_t threshold) {
    EEPROM.update(MEM_USER_SETTINGS + MEM_SETTINGS_MAX_THRESHOLD_OFFSET,
            threshold);
}

uint8_t Persist::getNoiseFloor() {
    return EEPROM.read(MEM_USER_SETTINGS + MEM_SETTINGS_NOISE_FLOOR_OFFSET);
}

void Persist::setNoiseFloor(uint8_t noise) {
    EEPROM.update(MEM_USER_SETTINGS + MEM_SETTINGS_NOISE_FLOOR_OFFSET, noise);
}

/* MATRIX POSITION (ROW AND COLUMN) FUNCTIONS */

bool Persist::matrixPositionActive(uint8_t row, uint8_t col) {
    // Read the memory at key matrix address, sanitize input to 7 bit
    uint8_t keyID = EEPROM.read(MEM_KEY_MATRIX + getHardwareID(row, col));
    // 0-127 are keyIDs, 128 (0x80) denotes it is in matrix but ID not set
    return (keyID <= 0x80);
}

void Persist::setMatrixPositionActive(uint8_t row, uint8_t col, bool active) {
    // 128 (0x80) denotes active in matrix
    uint8_t setting = active ? 0x80 : 0xFF;
    EEPROM.update(MEM_KEY_MATRIX + getHardwareID(row, col), setting);
}

uint8_t Persist::getUserID(uint8_t row, uint8_t col) {
    // Read the memory at key matrix address, sanitize input to 7 bit
    return EEPROM.read(MEM_KEY_MATRIX + getHardwareID(row, col));
}

void Persist::setUserID(uint8_t row, uint8_t col, uint8_t userID) {
    EEPROM.update(MEM_KEY_MATRIX + getHardwareID(row, col),
            // Set bits to the masked keyID
            (userID & 0x7F)
    );
}

// Row bits (RRR) and column (CCCC) in unique 0-127 value: 0RRR CCCC
uint8_t Persist::getHardwareID(uint8_t row, uint8_t col) {
    uint8_t rowMask = 0x07;
    uint8_t colMask = 0x0F;
    return ((row & rowMask) << 4) | (col & colMask);
}

/* CALIBRATION FUNCTIONS */

uint8_t Persist::getCalMin(uint8_t row, uint8_t col) {
    return EEPROM.read(MEM_KEY_CAL_MIN + getHardwareID(row, col));
}

void Persist::setCalMin(uint8_t row, uint8_t col, uint8_t value) {
    EEPROM.update(MEM_KEY_CAL_MIN + getHardwareID(row, col), value);
}

uint8_t Persist::getCalMax(uint8_t row, uint8_t col) {
    return EEPROM.read(MEM_KEY_CAL_MAX + getHardwareID(row, col));
}

void Persist::setCalMax(uint8_t row, uint8_t col, uint8_t value) {
    EEPROM.update(MEM_KEY_CAL_MAX + getHardwareID(row, col), value);
}

/* KEYMAP FUNCTIONS */

uint16_t Persist::getMapping(uint8_t keyID, uint8_t layer) {
    uint16_t address = MEM_KEYMAPS + BLOCK_SIZE * maskLayer(layer) +
        2 * maskKeyID(keyID);
    return (EEPROM.read(address) << 8) | EEPROM.read(address + 1);
}

void Persist::setMapping(uint8_t keyID, uint8_t layer, uint16_t mapID) {
    uint16_t address = MEM_KEYMAPS + BLOCK_SIZE * maskLayer(layer) +
        2 * maskKeyID(keyID);
    EEPROM.update(address, (mapID >> 8));
    EEPROM.update(address + 1, mapID);
}
