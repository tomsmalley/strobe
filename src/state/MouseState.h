#ifndef MOUSE_STATE_H
#define MOUSE_STATE_H

#include "BiState.h"

class MouseState: public BiState {

    public:
        virtual void update(uint8_t payload, uint8_t operation);
        void updateAnalog(uint8_t payload, uint8_t depth);
        void send();
        void resetAnalog();
        void initState();

    private:
        uint8_t transmitPreviousTimeout = 0;

        uint8_t buttons;
        uint8_t mouseX;
        uint8_t mouseY;
        uint8_t scrollY;
        uint8_t scrollX;

        uint8_t mouseVel(uint8_t value);
        void moveMouseUp(uint8_t depth);
        void moveMouseDown(uint8_t depth);
        void moveMouseLeft(uint8_t depth);
        void moveMouseRight(uint8_t depth);

};

#endif
