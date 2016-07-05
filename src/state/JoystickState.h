#ifndef JOYSTICK_STATE_H
#define JOYSTICK_STATE_H

#include "BiState.h"

class JoystickState: public BiState {

    public:
        virtual void update(uint8_t payload, uint8_t operation) {}
        void updateAnalog(uint8_t payload, uint8_t depth) {}
        void resetAnalog();
        void send();

    private:
        uint8_t transmitPreviousTimeout = 0;

};

#endif
