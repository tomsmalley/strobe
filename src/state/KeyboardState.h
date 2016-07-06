#ifndef KEYBOARD_STATE_H
#define KEYBOARD_STATE_H

#include <cstdint>
#include "Actions.h"

class KeyboardState {

    public:
        void update(uint8_t payload, Operation operation);
        void initState();
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

};

#endif
