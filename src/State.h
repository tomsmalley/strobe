#ifndef STATE_H
#define STATE_H

#include <cstdint>

class Key;

class State {

    public:
        State();

        static const int NUM_KEYS = 128;

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

};

#endif
