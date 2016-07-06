#ifndef STATE_H
#define STATE_H

#include <cstdint>

#include <WProgram.h>

#include "state/KeyboardState.h"
#include "state/MouseState.h"
#include "state/JoystickState.h"
#include "state/LayerState.h"

#define SCHEDULE_LENGTH 128

class Key;

class State {

    public:
        State();
        static const int NUM_KEYS = 128;
        Key* keys[NUM_KEYS];
        void handle(uint8_t route, uint8_t payload, uint8_t depth, bool up, bool down);
        void updateState();

    private:
        uint8_t schedulePayload[SCHEDULE_LENGTH];
        uint16_t scheduleTime[SCHEDULE_LENGTH];
        uint8_t scheduleOperation[SCHEDULE_LENGTH];

        elapsedMillis sinceLastUpdate;

        KeyboardState keyboardState;
        MouseState mouseState;
        JoystickState joystickState;
        LayerState layerState;

        void scanKeys();
        void analogHandle(uint8_t payload, uint8_t depth);
        void dispatchPayload(uint8_t payload, uint8_t operation);
        void schedule(uint8_t payload, uint8_t operation, uint16_t time);
        void specialHandle(uint8_t address, uint8_t depth);

};

#endif
