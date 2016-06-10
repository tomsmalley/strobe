#ifndef KEYMAP_H
#define KEYMAP_H

#include <cstdint>

class State;

class KeyMap {

    public:
        static void pressEvent(uint8_t mapID, State* state);
        static void releaseEvent(uint8_t mapID, State* state);

};

#endif
