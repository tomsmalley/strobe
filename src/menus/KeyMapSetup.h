#ifndef KEYMAPSETUP_H
#define KEYMAPSETUP_H

#include "SerialMenu.h"

class KeyMapSetup: public SerialMenu {

    public:

        KeyMapSetup(): SerialMenu ("Key map setup", FUNCTIONS, ARRAY_SIZE) {}

    private:

        static const int ARRAY_SIZE = 3;
        static const SerialMenuFunction FUNCTIONS[ARRAY_SIZE];

        static void setKeyMap();
        static void printValues();
        static void resetValues();

};

#endif
