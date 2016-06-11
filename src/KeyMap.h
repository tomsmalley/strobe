#ifndef KEYMAP_H
#define KEYMAP_H

#include <cstdint>

class State;

class KeyMap {

    public:
        static void handle(uint8_t mapID, uint8_t depth, State* state);
        static void pressEvent(uint8_t mapID, State* state);
        static void releaseEvent(uint8_t mapID, State* state);

};

#endif
