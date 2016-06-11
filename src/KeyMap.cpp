#include "KeyMap.h"

#include <WProgram.h>

#include "State.h"

/**
 * 0x00 .. 0xA4  USB Selectors (Part 1)
 * 0xA5 .. 0xAF  Mouse keys
 * 0xB0 .. 0xDF  USB Selectors (Part 2)
 * 0xE0 .. 0xE7  USB Modifiers
 * 0xE8 .. 0xEF  8 Layer toggle
 * 0xF0 .. 0xF7  8 Layer while held // might get rid of this
 * 0xF8 .. 0xFE  Unused (7 spaces, maybe good for controller)
 * 0xFF          Keyboard lock
 */
void KeyMap::handle(uint8_t mapID, uint8_t depth, State* state) {

    if ((mapID >= 0xA5 && mapID <= 0xAF)
     || (mapID >= 0xF8 && mapID <= 0xF9)) { // Mouse keys


        if ((mapID == 0xA5) || (mapID == 0xA6) || (mapID == 0xA7)) {
            state->moveMouseUp(depth);
        }

        if ((mapID == 0xA7) || (mapID == 0xA9) || (mapID == 0xAC)) {
            state->moveMouseRight(depth);
        }

        if ((mapID == 0xAA) || (mapID == 0xAB) || (mapID == 0xAC)) {
            state->moveMouseDown(depth);
        }

        if ((mapID == 0xA5) || (mapID == 0xA8) || (mapID == 0xAA)) {
            state->moveMouseLeft(depth);
        }

    }

}

void KeyMap::pressEvent(uint8_t mapID, State* state) {

    if (mapID <= 0xA4 || (mapID >= 0xB0 && mapID <= 0xDF)) { // Selectors
        state->setUSBSelector(mapID);
    } else if (mapID >= 0xE0 && mapID <= 0xE7) { // Modifiers
        state->setUSBModifier(mapID);
    } else if ((mapID >= 0xA5 && mapID <= 0xAF)
            || (mapID >= 0xF8 && mapID <= 0xF9)) { // Mouse keys
        // Mouse key defs
        //
        // Pointer:       UP
        //      +------+------+------+
        //      | 0xA5 | 0xA6 | 0xA7 |
        //      +------+------+------+
        // LEFT | 0xA8 |      | 0xA9 | RIGHT
        //      +------+------+------+
        //      | 0xAA | 0xAB | 0xAC |
        //      +------+------+------+
        //               DOWN
        //
        // Click:
        //     LEFT : 0xAD
        //   MIDDLE : 0xAE
        //    RIGHT : 0xAF
        //
        // Scroll:
        //       UP : 0xF8
        //     DOWN : 0xF9

    } else if (mapID >= 0xE8 && mapID <= 0xEF) { // Toggle layer
        // Change the layer (0xE8 is 0, 0xEF is 7)
        state->layer = mapID - 0xE8;
        // All keys need to be unset so they don't get stuck on
        // TODO do this in changeLayer function?
        state->unsetAllUSBKeys();
        // increment decrement can be set up through setting different
        // functions for a single key for each layer (eg: layer 1 function is
        // to go to layer 2, layer 2 function is to go to layer 3... etc).
    } else if (mapID >= 0xF0 && mapID <= 0xF7) { // Hold layer
        // Change layer while held
    } else if (mapID == 0xFF) { // Keyboard lock
        // 0xFF keyboard lock
    }
}

void KeyMap::releaseEvent(uint8_t mapID, State* state) {
    if (mapID < 0xE0) { // Selectors (USB HID Spec)
        state->unsetUSBSelector(mapID);
    } else if (mapID < 0xE8) { // Modifiers (USB HID Spec)
        state->unsetUSBModifier(mapID);
    } else if (mapID > 0xE8 && mapID <= 0xEF) {
        // Do nothing
    } else if (mapID >= 0xF0 && mapID <= 0xFA) {
        // Nothing
    } else if (mapID == 0xFF) {
        // 0xFF keyboard lock
    }
}
