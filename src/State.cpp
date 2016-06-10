#include "State.h"

#include "Key.h"

State::State() {
    for (int i = 0; i < NUM_KEYS; i++) {
        keys[i] = new Key();
    }
}

void State::setUSBSelector(uint8_t keyCode) {}
void State::unsetUSBSelector(uint8_t keyCode) {}
void State::setUSBModifier(uint8_t keyCode) {}
void State::unsetUSBModifier(uint8_t keyCode) {}
void State::unsetAllUSBKeys() {}
