#include "RowController.h"


#include "ADC.h"

/**
 * Setup pins
 */
RowController::RowController() {
    // Analog in pin
    pinMode(ROW_READ_PIN, INPUT);
    // Digital multiplexer controller pins
    pinMode(MUX_CONTROL_PIN[0], OUTPUT);
    pinMode(MUX_CONTROL_PIN[1], OUTPUT);
    pinMode(MUX_CONTROL_PIN[2], OUTPUT);
    // ADC for reading
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
void RowController::select(uint8_t rowID) {
    // Get binary representation using bitwise operations
    digitalWrite(MUX_CONTROL_PIN[0], (rowID) & 1);
    digitalWrite(MUX_CONTROL_PIN[1], (rowID >> 1) & 1);
    digitalWrite(MUX_CONTROL_PIN[2], (rowID >> 2) & 1);
}

uint8_t RowController::read() {
    return adc->analogRead(ROW_READ_PIN, ADC_0);
}
