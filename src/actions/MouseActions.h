#ifndef ACTION_MOUSE_H
#define ACTION_MOUSE_H

#include <cstdint>

class State;

class MouseActions {

    public:
        static void handle(uint8_t action, uint8_t depth, State* state);
        static void pressEvent(uint8_t action, State* state);
        static void releaseEvent(uint8_t action, State* state);

        void sendState();

    private:
        uint8_t cursorX;
        uint8_t cursorY;
        uint8_t scrollX;
        uint8_t scrollY;
        uint8_t buttons;

};

#endif
