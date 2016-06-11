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
void State::printUSBModifiers() {
    Serial.print("Mods: ");
    Serial.println(keyboard_modifier_keys);
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

/**
 * Sets the USB modifier sent in keyCode.
 * Modifier packet is one byte with each bit denoting if a modifier is pressed.
 * Denoting the bits in the byte packet as 76543210,
 * Let (HEX) BIT denote the USB HID spec hex input code and the bit number
 * respectively, the codes for each key are:
 *        Control   Shift    Alt      GUI
 * Left   (E0) 0    (E1) 1   (E2) 2   (E3) 3
 * Right  (E4) 4    (E5) 5   (E6) 6   (E7) 7
 * @param keyCode USB HID modifier key code.
 */
void State::setUSBModifier(uint8_t keyCode) {
    keyboard_modifier_keys |= (1 << (keyCode - 0xE0));
}

/**
 * Unsets the USB modifier sent in keyCode. See setUSBModifier for explanation.
 * @param keyCode USB HID modifier key code.
 */
void State::unsetUSBModifier(uint8_t keyCode) {
    keyboard_modifier_keys &= ~(1 << (keyCode - 0xE0));
}

void State::unsetAllUSBKeys() {
    // Selectors
    for (int i = 0; i < 6; i++) {
        keyboard_keys[i] = 0;
    }
    // Modifiers
    keyboard_modifier_keys = 0;
}
