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
 * Selects row as per 4051 binary table
 * @param s2 third binary bit
 * @param s1 second binary bit
 * @param s0 first binary bit
 */
void RowController::muxSelect(uint8_t s2, uint8_t s1, uint8_t s0) {
    digitalWrite(MUX_CONTROL_PIN[0], s0);
    digitalWrite(MUX_CONTROL_PIN[1], s1);
    digitalWrite(MUX_CONTROL_PIN[2], s2);
}

/**
 * Select a row on the multiplexer (integer row number)
 * TODO make these functions nicer -- maybe hexadecimal
 * @param row row number
 */
void RowController::select(uint8_t rowID) {
    switch (rowID) {
        // Converts decimal row to binary for multiplexer
        case 1:
            muxSelect(0, 0, 1);
            break;
        case 2:
            muxSelect(0, 1, 0);
            break;
        case 3:
            muxSelect(0, 1, 1);
            break;
        case 4:
            muxSelect(1, 0, 0);
            break;
        case 5:
            muxSelect(1, 0, 1);
            break;
        case 6:
            muxSelect(1, 1, 0);
            break;
        case 7:
            muxSelect(1, 1, 1);
            break;
        default:
            muxSelect(0, 0, 0);
            break;
    }
}

uint8_t RowController::read() {
    return adc->analogRead(ROW_READ_PIN, ADC_0);
}
