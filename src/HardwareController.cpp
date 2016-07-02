#include "HardwareController.h"

#include "ADC.h"

/*** Setup all the pins and ADC in constructor ***/

HardwareController::HardwareController() {

    // LED pin
    pinMode(PIN_LED, OUTPUT);
    // Analog in pin
    pinMode(PIN_READ, INPUT);
    // Digital multiplexer controller pins
    pinMode(PIN_MUX_CONTROL[0], OUTPUT);
    pinMode(PIN_MUX_CONTROL[1], OUTPUT);
    pinMode(PIN_MUX_CONTROL[2], OUTPUT);
    // Column pins
    for (int i = 0; i < NUM_STROBES; i++) {
        pinMode(PIN_STROBE[i], OUTPUT);
    }
    pinMode(PIN_DRAIN, OUTPUT_OPENDRAIN);

    // Row ADC
    adc = new ADC();
    adc->setAveraging(1);
    adc->setResolution(8); // returns a byte (0-255)
    adc->setConversionSpeed(ADC_HIGH_SPEED);
    adc->setSamplingSpeed(ADC_HIGH_SPEED);
    adc->setReference(ADC_REF_3V3);

}


/*** Reading functions ***/

uint8_t HardwareController::strobeRead(uint8_t strobeLine) {
    uint8_t value;
    // Make sure we have waited enough time
    long time = timeSinceLastStrobe; // Capture current value
    if (time < RELAX_TIME) {
        delayMicroseconds(RELAX_TIME - time);
    }
    // Float the drain pin
    setDrainFloating();
    // Time sensitive part - interrupts can affect delayMicroseconds
    noInterrupts();
    // Set column high ("strobe")
    setStrobeHigh(strobeLine);
    // Read the selected key value
    value = readSelected();
    // Set strobe line low
    setStrobeLow(strobeLine);
    // Turn back on interrupts and wait for row to relax to 0V
    interrupts();
    // Ground the drain pin
    setDrainGround();
    // Start timer for next call
    timeSinceLastStrobe = elapsedMicros();
    return value;
}

void HardwareController::selectReadLine(uint8_t line) const {
    // Get binary representation using bitwise operations
    digitalWrite(PIN_MUX_CONTROL[0], (line) & 1);
    digitalWrite(PIN_MUX_CONTROL[1], (line >> 1) & 1);
    digitalWrite(PIN_MUX_CONTROL[2], (line >> 2) & 1);
    delayMicroseconds(RELAX_TIME);
}


/*** Row functions ***/

uint8_t HardwareController::readSelected() const {
    return adc->analogRead(PIN_READ, ADC_0);
}


/*** Col functions ***/

void HardwareController::setStrobeHigh(uint8_t line) const {
    digitalWrite(PIN_STROBE[line], HIGH);
}

void HardwareController::setStrobeLow(uint8_t line) const {
    digitalWrite(PIN_STROBE[line], LOW);
}


/*** Drain functions ***/

void HardwareController::setDrainFloating() const {
    digitalWrite(PIN_DRAIN, HIGH);
}

void HardwareController::setDrainGround() const {
    digitalWrite(PIN_DRAIN, LOW);
}


/*** LED functions ***/

void HardwareController::turnOnLED() const {
    digitalWrite(PIN_LED, HIGH);
}

void HardwareController::turnOffLED() const {
    digitalWrite(PIN_LED, LOW);
}


// Global definition
HardwareController* controller = new HardwareController();
