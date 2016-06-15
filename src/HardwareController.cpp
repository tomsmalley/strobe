#include "HardwareController.h"

#include "ADC.h"

const HardwareController* hardwareController = new HardwareController();

/**
 * Setup all the pins and ADC
 */
HardwareController::HardwareController() {

    // LED pin
    pinMode(PIN_LED, OUTPUT);
    // Analog in pin
    pinMode(PIN_ROW_READ, INPUT);
    // Digital multiplexer controller pins
    pinMode(PIN_MUX_CONTROL[0], OUTPUT);
    pinMode(PIN_MUX_CONTROL[1], OUTPUT);
    pinMode(PIN_MUX_CONTROL[2], OUTPUT);
    // Column pins
    for (int i = 0; i < NUM_COLS; i++) {
        pinMode(PIN_COL[i], OUTPUT);
    }

    // Row ADC
    adc = new ADC();
    adc->setAveraging(1);
    adc->setResolution(8); // returns a byte (0-255)
    adc->setConversionSpeed(ADC_HIGH_SPEED);
    adc->setSamplingSpeed(ADC_HIGH_SPEED);
    adc->setReference(ADC_REF_3V3);

}

/**
 * Select a row on the multiplexer (integer row number).
 * @param row row number
 */
void HardwareController::selectRow(uint8_t row) const {
    // Get binary representation using bitwise operations
    digitalWrite(PIN_MUX_CONTROL[0], (row) & 1);
    digitalWrite(PIN_MUX_CONTROL[1], (row >> 1) & 1);
    digitalWrite(PIN_MUX_CONTROL[2], (row >> 2) & 1);
}

uint8_t HardwareController::readRow() const {
    return adc->analogRead(PIN_ROW_READ, ADC_0);
}

void HardwareController::setColHigh(uint8_t col) const {
    digitalWrite(PIN_COL[col], HIGH);
}

void HardwareController::setColLow(uint8_t col) const {
    digitalWrite(PIN_COL[col], LOW);
}
