#ifndef STATE_H
#define STATE_H

#include <cstdint>

class Key;

class State {

    public:
        State();

        static const int NUM_KEYS = 128;

        // reset USB state array when changing layer or key could get stuck on!
        bool fnPressed = false;
        uint8_t layer = 1;
        Key* keys[NUM_KEYS];

        // static?? move to different class
        void setUSBSelector(uint8_t keyCode);
        void unsetUSBSelector(uint8_t keyCode);
        void setUSBModifier(uint8_t keyCode);
        void unsetUSBModifier(uint8_t keyCode);
        void unsetAllUSBKeys();
        void printUSBSelectors();
        void printUSBModifiers();

        //
        void moveMouseUp(uint8_t depth);
        void moveMouseRight(uint8_t depth);
        void moveMouseDown(uint8_t depth);
        void moveMouseLeft(uint8_t depth);

        int8_t getMouseX();
        int8_t getMouseY();
        void resetMousePos();

    private:

        uint8_t mouseVel(uint8_t value);
        int8_t mouseX = 0;
        int8_t mouseY = 0;

};

#endif
