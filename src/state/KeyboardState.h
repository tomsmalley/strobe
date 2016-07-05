#ifndef KEYBOARD_STATE_H
#define KEYBOARD_STATE_H

#include "BiState.h"

class KeyboardState: public BiState {

    public:
        virtual void update(uint8_t payload, uint8_t operation);
        void send();

    private:
        uint8_t transmitPreviousTimeout = 0;

        void setSelector(uint8_t keyCode);
        void unsetSelector(uint8_t keyCode);
        void toggleSelector(uint8_t keyCode);
        void setModifier(uint8_t keyCode);
        void unsetModifier(uint8_t keyCode);
        void toggleModifier(uint8_t keyCode);

        uint8_t modifiers = 0;
        uint8_t selectors[6] = {0, 0, 0, 0, 0, 0};
        uint8_t idleConfig = 125;
        uint8_t idleCount = 0;

};

#endif
