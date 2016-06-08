#ifndef SERIALMENU_H
#define SERIALMENU_H

#include "SerialMenuFunction.h"

class SerialMenu {

    public:
        SerialMenu(const char* n, SerialMenuFunction* fs, int s);
        void start();

    private:
        const char* name;
        SerialMenuFunction* functions;
        // size of functions array
        int size;
        void printMenu();

};

#endif
