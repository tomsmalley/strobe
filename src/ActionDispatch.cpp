#include "ActionDispatch.h"

#include <WProgram.h>

#include "State.h"
#include "Persist.h"
#include "actions/MouseActions.h"

#define ROUTE_ANALOG    0
#define ROUTE_MOMENTARY 1
#define ROUTE_TOGGLE    2
#define ROUTE_SPECIAL   3

#define SET 0
#define UNSET 1
#define TOGGLE 2

ActionDispatch::ActionDispatch() {
}

void ActionDispatch::handle(uint8_t route, uint8_t payload, uint8_t depth = 0,
        bool up = false, bool down = false) {
    switch (route) {
        case ROUTE_ANALOG:
            analogHandle(payload, depth);
            break;
        case ROUTE_MOMENTARY:
            if (down) {
                schedule(payload, SET, 0);
            } else if (up) {
                schedule(payload, UNSET, 0);
            }
            break;
        case ROUTE_TOGGLE:
            if (down) {
                schedule(payload, TOGGLE, 0);
            }
            break;
        case ROUTE_SPECIAL:
            specialHandle(payload, depth);
            break;
    }
}

void ActionDispatch::analogHandle(uint8_t payload, uint8_t depth) {
}

void ActionDispatch::schedule(uint8_t payload, uint8_t operation, uint16_t time) {
    uint16_t longestTime = 0;
    uint8_t longestID = 0;
    for (int i = 0; i < SCHEDULE_LENGTH; i++) {
        if (schedulePayload[i] == 0) {
            schedulePayload[i] = payload;
            scheduleOperation[i] = operation;
            scheduleTime[i] = time;
            return;
        } else {
            // Log the longest time incase the schedule is full
            if (scheduleTime[i] > longestTime) {
                longestID = i;
                longestTime = scheduleTime[i];
            }
        }
    }
    // If the schedule is full and this should come earlier than the longest
    // scheduled item, just replace it with this
    if (time < longestTime) {
        schedulePayload[longestID] = payload;
        scheduleOperation[longestID] = operation;
        scheduleTime[longestID] = time;
    }
}

void ActionDispatch::updateState() {
    // march time, do using user setting TODO
    uint16_t timeStep = sinceLastUpdate;
    for (int i = 0; i < SCHEDULE_LENGTH; i++) {
        if (schedulePayload[i] != 0) {
            if (scheduleTime[i] <= timeStep) {
                dispatchPayload(schedulePayload[i], scheduleOperation[i]);
                schedulePayload[i] = 0;
            } else {
                scheduleTime[i] -= timeStep;
            }
        }
    }
    sinceLastUpdate = elapsedMillis();
}


void ActionDispatch::specialHandle(uint8_t address, uint8_t depth) {
    // read the address and get the actions
}

void ActionDispatch::dispatchPayload(uint8_t payload, uint8_t operation) {
    if (payload <= 0xA4) { // Selectors
        keyboardState.execute(payload, operation);
    } else if (payload >= 0xE0 && payload <= 0xE7) { // Modifiers
        keyboardState.execute(payload, operation);
    } else if (payload >= 0xD0 && payload <= 0xDF) { // Toggle layer
        // Change the layer (0xE8 is 0, 0xEF is 7)
        uint8_t layer = payload - 0xD0;
        if (layer == 0) {
            // reset all layers
        } else {
            // operation that layer
        }
        // All keys need to be unset so they don't get stuck on
        // TODO do this in changeLayer function?
        //state->unsetAllUSBKeys();
        // increment decrement can be set up through setting different
        // functions for a single key for each layer (eg: layer 1 function is
        // to go to layer 2, layer 2 function is to go to layer 3... etc).
    } else if (payload >= 0xB0 && payload <= 0xCF) { // Joystick keys
        joystickState.execute(payload - 0xB0, operation);
    } else if (payload >= 0xA5 && payload <= 0xAA) { // Mouse buttons
        mouseState.execute(payload - 0xA5, operation);
    } else if (payload == 0xFF) { // Keyboard lock
        // 0xFF keyboard lock
    }
}
