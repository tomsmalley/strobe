#ifndef GLOBALS_H
#define GLOBALS_H

#include <cstdint>

class RowController;
class ColumnController;
class Key;

const int NUM_KEYS = 128;

namespace controllers {

    extern RowController* row;
    extern ColumnController* column;

}

namespace state {

    extern bool fnPressed;
    extern uint8_t layer;
    extern Key* keys[NUM_KEYS];

}

#endif
