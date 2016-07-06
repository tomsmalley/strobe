#ifndef JOYSTICK_STATE_H
#define JOYSTICK_STATE_H

#include <cstdint>

enum class Operation : uint8_t;

class JoystickState {

    public:
        void update(uint8_t payload, Operation operation) {}
        void updateAnalog(uint8_t payload, uint8_t depth) {}
        void resetAnalog();
        void send();
        void initState() {}

    private:
        uint8_t transmitPreviousTimeout = 0;

};

#endif
