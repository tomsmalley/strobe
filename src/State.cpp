#include "State.h"

#include "WProgram.h"

#include "Key.h"

State::State() {
    for (int i = 0; i < NUM_KEYS; i++) {
        keys[i] = new Key();
    }
}

// Testing
void State::printUSBSelectors() {
    for (int i = 0; i < 6; i++) {
        Serial.print(keyboard_keys[i]);
        Serial.print(" ");
    }
    Serial.println();
}

void State::setUSBSelector(uint8_t keyCode) {
    // Go through selectors array, check for the keyCode
    for (int i = 0; i < 2; i++) {
        if (keyboard_keys[i] == keyCode) {
            return;
        }
    }
    for (int i = 0; i < 2; i++) {
        // If there is an empty space put the keyCode in
        if (keyboard_keys[i] == 0) {
            keyboard_keys[i] = keyCode;
            return;
        }
    }
}

void State::unsetUSBSelector(uint8_t keyCode) {
    // Go through selectors array
    for (int i = 0; i < 6; i++) {
        // If the keyCode is there remove it
        if (keyboard_keys[i] == keyCode) {
            keyboard_keys[i] = 0;
            return;
        }
    }
}

// Modifiers work differently - there is a space for any of them!!
// TODO
void State::setUSBModifier(uint8_t keyCode) {

}

void State::unsetUSBModifier(uint8_t keyCode) {

}

void State::unsetAllUSBKeys() {
    // Selectors
    for (int i = 0; i < 6; i++) {
        keyboard_keys[i] = 0;
    }
}
