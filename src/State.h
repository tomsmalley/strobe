#ifndef STATE_H
#define STATE_H

#include <cstdint>

class Key;

class State {

    public:
        State();

        static const int NUM_KEYS = 128;

        bool fnPressed;
        uint8_t layer = 1;
        Key* keys[NUM_KEYS];
        uint8_t* usbSelectors[6];
        uint8_t* usbModifiers[8];

        // static??
        void setUSBSelector(uint8_t keyCode);
        void unsetUSBSelector(uint8_t keyCode);
        void setUSBModifier(uint8_t keyCode);
        void unsetUSBModifier(uint8_t keyCode);
        void unsetAllUSBKeys();

};

#endif
