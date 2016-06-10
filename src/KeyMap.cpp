#include "KeyMap.h"

#include "State.h"

void KeyMap::pressEvent(uint8_t mapID, State* state) {
    if (mapID < 0xE0) { // Selectors (USB HID Spec)
        state->setUSBSelector(mapID);
    } else if (mapID < 0xE8) { // Modifiers (USB HID Spec)
        state->setUSBModifier(mapID);
    } else if (mapID > 0xE8 && mapID <= 0xEF) {
        // Change the layer (0xE8 is 0, 0xEF is 7)
        state->layer = mapID - 0xE8;
        // All keys need to be unset so they don't get stuck on
        state->unsetAllUSBKeys();
        // Maybe increment/decrement layer??
    } else if (mapID >= 0xF0 && mapID <= 0xFA) {
        // Mouse key defs
        // 0xF0 Left movement
        // 0xF1 Down movement
        // 0xF2 Up movement
        // 0xF3 Right movement
        // maybe allocate 4 more for diagonals??
        // 0xF8 Left click
        // 0xF9 Middle click
        // 0xFA Right click
        // and maybe 2 for scroll ?
    } else if (mapID == 0xFF) {
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
