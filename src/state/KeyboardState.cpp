#include "KeyboardState.h"

#include <cstdint>
#include <WProgram.h>
#include "usb_dev.h"
#include "core_pins.h"

#include "USBTimeout.h"

void KeyboardState::setSelector(uint8_t keyCode) {
    // Go through keys array, check for keyCode
    for (int i = 0; i < 6; i++) {
        if (selectors[i] == keyCode) {
            return;
        }
    }
    for (int i = 0; i < 6; i++) {
        // If there is an empty space put the keyCode in
        if (selectors[i] == 0) {
            selectors[i] = keyCode;
            return;
        }
    }
}

void KeyboardState::unsetSelector(uint8_t keyCode) {
    // Go through selectors array
    for (int i = 0; i < 6; i++) {
        // If the keyCode is there remove it
        if (selectors[i] == keyCode) {
            selectors[i] = 0;
        }
    }
}

void KeyboardState::toggleSelector(uint8_t keyCode) {
    // Go through selectors array
    for (int i = 0; i < 6; i++) {
        // If the keyCode is there remove it and exit
        if (selectors[i] == keyCode) {
            selectors[i] = 0;
            return;
        }
    }
    // If the keycode isn't there it needs adding
    for (int i = 0; i < 6; i++) {
        // If there is an empty space put the keyCode in
        if (selectors[i] == 0) {
            selectors[i] = keyCode;
            return;
        }
    }
}

void KeyboardState::setModifier(uint8_t keyCode) {
    modifiers |= 1 << (keyCode - 0xE0);
}

void KeyboardState::unsetModifier(uint8_t keyCode) {
    modifiers &= ~(1 << (keyCode - 0xE0));
}

void KeyboardState::toggleModifier(uint8_t keyCode) {
    modifiers ^= 1 << (keyCode - 0xE0);
}

void KeyboardState::initState() {
    modifiers = 0;
    for (int i = 0; i < 6; i++) {
        selectors[i] = 0;
    }
}

#define SET 0
#define UNSET 1
#define TOGGLE 2

void KeyboardState::update(uint8_t keyCode, uint8_t operation) {
    if (keyCode <= 0xA4) {
        switch(operation) {
            case SET:
                setSelector(keyCode);
                break;
            case UNSET:
                unsetSelector(keyCode);
                break;
            case TOGGLE:
                toggleSelector(keyCode);
                break;
        }
    } else if (keyCode >= 0xE0 && keyCode <= 0xE7) {
        switch(operation) {
            case SET:
                setModifier(keyCode);
                break;
            case UNSET:
                unsetModifier(keyCode);
                break;
            case TOGGLE:
                toggleModifier(keyCode);
                break;
        }
    }
}

void KeyboardState::send() {
    uint32_t waitCount = 0;
    usb_packet_t *txPacket;

    while(true) {
        if (!usb_configuration) {
            return;
        }
        if (usb_tx_packet_count(KEYBOARD_ENDPOINT) < TX_PACKET_LIMIT) {
            txPacket = usb_malloc();
            if (txPacket) break;
        }
        if (++waitCount > TX_TIMEOUT || transmitPreviousTimeout) {
            transmitPreviousTimeout = true;
            return;
        }
        yield();
    }
    transmitPreviousTimeout = false;
    *(txPacket->buf) = modifiers;
    *(txPacket->buf + 1) = 0;
    memcpy(txPacket->buf + 2, selectors, 6);
    txPacket->len = 8;
    usb_tx(KEYBOARD_ENDPOINT, txPacket);
}
