#include "Persist.h"

#include <WProgram.h>

#include <EEPROM.h>

#include "Actions.h" // For Route enum
#include "HardwareController.h"

// Memory locations
const int MATRIX_SIZE = HardwareController::NUM_READS * HardwareController::NUM_STROBES;
const int ADDRESS_SETTINGS = 0;
const int ADDRESS_IDS = ADDRESS_SETTINGS + SETTINGS_COUNT;
const int ADDRESS_CAL_MIN = ADDRESS_IDS + MATRIX_SIZE;
const int ADDRESS_CAL_MAX = ADDRESS_CAL_MIN + MATRIX_SIZE;
const int ADDRESS_LAYERS = ADDRESS_CAL_MAX + MATRIX_SIZE;
const int ADDRESS_MACROS = 0x7FF; // Last byte, these are read in reverse

/* PRIVATE HELPER FUNCTIONS */

// Mask off bottom 7 bits
uint8_t Persist::maskKeyID(uint8_t keyID) {
    return (keyID & 0x7F);
}

// Make sure layer is reachable
uint8_t Persist::maskLayer(uint8_t layer) {
    // Fall back to layer 0 if attempting to access non existing layer
    return layer < getSetting(Setting::LAYER_COUNT) ? layer : 0;
}

/* USER SETTINGS */

uint8_t Persist::getSetting(Setting s) {
    return EEPROM.read(ADDRESS_SETTINGS + static_cast<uint8_t>(s));
}
void Persist::setSetting(Setting s, uint8_t value) {
    if (s == Setting::LAYER_COUNT) {} // Need to allocate memory TODO
    EEPROM.update(ADDRESS_SETTINGS + static_cast<uint8_t>(s), value);
}

/* MATRIX POSITION (ROW AND COLUMN) FUNCTIONS */

bool Persist::matrixPositionActive(uint8_t row, uint8_t col) {
    // Read the memory at key matrix address, sanitize input to 7 bit
    uint8_t keyID = EEPROM.read(ADDRESS_IDS + getHardwareID(row, col));
    // 0-127 are keyIDs, 128 (0x80) denotes it is in matrix but ID not set
    return (keyID <= 0x80);
}

void Persist::setMatrixPositionActive(uint8_t row, uint8_t col, bool active) {
    // 128 (0x80) denotes active in matrix
    uint8_t setting = active ? 0x80 : 0xFF;
    EEPROM.update(ADDRESS_IDS + getHardwareID(row, col), setting);
}

uint8_t Persist::getUserID(uint8_t row, uint8_t col) {
    // Read the memory at key matrix address, sanitize input to 7 bit
    return EEPROM.read(ADDRESS_IDS + getHardwareID(row, col));
}

void Persist::setUserID(uint8_t row, uint8_t col, uint8_t userID) {
    EEPROM.update(ADDRESS_IDS + getHardwareID(row, col),
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
    return EEPROM.read(ADDRESS_CAL_MIN + getHardwareID(row, col));
}

void Persist::setCalMin(uint8_t row, uint8_t col, uint8_t value) {
    EEPROM.update(ADDRESS_CAL_MIN + getHardwareID(row, col), value);
}

uint8_t Persist::getCalMax(uint8_t row, uint8_t col) {
    return EEPROM.read(ADDRESS_CAL_MAX + getHardwareID(row, col));
}

void Persist::setCalMax(uint8_t row, uint8_t col, uint8_t value) {
    EEPROM.update(ADDRESS_CAL_MAX + getHardwareID(row, col), value);
}

/* KEY ACTIONS */

#define NUM_LAYERS_MAX 16

uint8_t Persist::getPayload(uint8_t keyID, uint8_t layer) {
    uint16_t address = ADDRESS_LAYERS + maskLayer(layer) *
        HardwareController::NUM_KEYS + maskKeyID(keyID);
    return EEPROM.read(address);
}
void Persist::setPayload(uint8_t keyID, uint8_t layer, uint8_t payload) {
    uint16_t address = ADDRESS_LAYERS + maskLayer(layer) *
        HardwareController::NUM_KEYS + maskKeyID(keyID);
    EEPROM.update(address, payload);
}

#define ROUTE_BITS 2
#define ROUTE_PER_BYTE 8/ROUTE_BITS

uint8_t Persist::routeOffset(uint8_t keyID) {
    return keyID * ROUTE_BITS / 8;
}
uint8_t Persist::routeShift(uint8_t keyID) {
    return (keyID % (8/ROUTE_BITS)) * ROUTE_BITS;
}

Route Persist::getRoute(uint8_t keyID, uint8_t layer) {
    uint16_t address = ADDRESS_LAYERS + getSetting(Setting::LAYER_COUNT) *
        HardwareController::NUM_KEYS + routeOffset(keyID);
    // Bit shift to lowest two bits and mask
    uint8_t r = (EEPROM.read(address) >> (6 - routeShift(keyID))) & 0b11;
    return static_cast<Route>(r);
}

void Persist::setRoute(uint8_t keyID, uint8_t layer, Route route) {
    uint16_t address = ADDRESS_LAYERS + getSetting(Setting::LAYER_COUNT) *
        HardwareController::NUM_KEYS + routeOffset(keyID);
    uint8_t r = static_cast<uint8_t>(route);
    // Number of bits to shift by
    uint8_t shift = 6 - routeShift(keyID);
    // Clear bits of interest in the byte block
    uint8_t old = EEPROM.read(address) & ~((0b11) << shift);
    // Mask and shift the route into position
    uint8_t set = ((r & 0b11) << shift);
    // Set the new byte value
    EEPROM.update(address, old | set);
}
