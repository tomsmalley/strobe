#include "State.h"

#include "WProgram.h"

#include "Key.h"
#include "Persist.h"

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
    for (int i = 0; i < 6; i++) {
        if (keyboard_keys[i] == keyCode) {
            return;
        }
    }
    for (int i = 0; i < 6; i++) {
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

uint8_t State::mouseVel(uint8_t value) {
    const uint16_t DEADZONE = Persist::getDeadZone();
    const uint16_t SENSITIVITY = Persist::getSensitivity();
    if (value < DEADZONE) {
        value = DEADZONE;
    }
    uint16_t numerator = SENSITIVITY * (value - DEADZONE);
    uint8_t denominator = 255 - DEADZONE;
    return (uint8_t) (numerator / denominator);
}

// TODO make sure they cant overflow.
void State::moveMouseUp(uint8_t depth) {
    uint8_t v = mouseVel(depth);
    if ((int)mouseY - v < -127) {
        mouseY = -127;
    } else {
        mouseY -= v;
    }
}
void State::moveMouseRight(uint8_t depth) {
    uint8_t v = mouseVel(depth);
    if ((int)mouseX + v > 127) {
        mouseX = 127;
    } else {
        mouseX += v;
    }
}
void State::moveMouseDown(uint8_t depth) {
    uint8_t v = mouseVel(depth);
    if ((int)mouseY + v > 127) {
        mouseY = 127;
    } else {
        mouseY += v;
    }
}
void State::moveMouseLeft(uint8_t depth) {
    uint8_t v = mouseVel(depth);
    if ((int)mouseX - v < -127) {
        mouseX = -127;
    } else {
        mouseX -= v;
    }
}

int8_t State::getMouseX() {
    return mouseX;
}
int8_t State::getMouseY() {
    return mouseY;
}
void State::resetMousePos() {
    mouseX = 0;
    mouseY = 0;
}
