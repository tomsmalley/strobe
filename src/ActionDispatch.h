#ifndef ACTION_DISPATCH_H
#define ACTION_DISPATCH_H

#include <cstdint>

#include <WProgram.h>

#include "state/KeyboardState.h"
#include "state/MouseState.h"
#include "state/JoystickState.h"

#define SCHEDULE_LENGTH 128

class ActionDispatch {

    public:
        ActionDispatch();
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

        void analogHandle(uint8_t payload, uint8_t depth);
        void dispatchPayload(uint8_t payload, uint8_t operation);
        void schedule(uint8_t payload, uint8_t operation, uint16_t time);
        void specialHandle(uint8_t address, uint8_t depth);

};

#endif
