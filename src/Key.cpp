#include "Key.h"

#include <WProgram.h>

#include "Persist.h"
#include "RowController.h"
#include "ColumnController.h"

Key::Key() {
    Serial.print("create");
}

uint8_t Key::strobeRead(int8_t keyID, RowController* row, ColumnController* col) {
    // Check the ID is positive
    if (keyID < 0) {
        return 0;
    }
    // Get the row and column IDs
    int8_t rowID = Persist::getRow(keyID);
    int8_t colID = Persist::getCol(keyID);
    // Check for invalid address
    if (rowID < 0 || colID < 0) {
        return 0;
    }

    uint8_t value;
    // Interrupts can affect delayMicroseconds
    noInterrupts();
    // Select the row on multiplexer
    row->select(rowID);
    // Set column high ("strobe")
    col->setHigh(colID);
    // Wait for amplifier to catch up
    delayMicroseconds(3);
    // Read the row value
    value = row->read();
    // Set column low
    col->setLow(colID);
    // Turn back on interrupts and wait for row to relax to 0V
    interrupts();
    return value;
}

uint8_t Key::normalise(int8_t keyID, uint8_t value) {
    uint16_t calMin = Persist::getCalMin(keyID);
    uint16_t calMax = Persist::getCalMax(keyID);
    // Clamp to max and min values
    if (value < calMin) {
        value = calMin;
    } else if (value > calMax) {
        value = calMax;
    }
    // Feature scaling, scale needs to be done before int division
    // Cast is okay because the fraction must be between 0 and 1 due to
    // clamping.
    uint16_t numerator = 255 * (value - calMin);
    uint8_t denominator = calMax - calMin;
    return (uint8_t) (numerator / denominator);
}
