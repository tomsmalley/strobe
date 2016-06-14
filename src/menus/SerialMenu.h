#ifndef SERIALMENU_H
#define SERIALMENU_H

#include "SerialMenuFunction.h"

/**
 * This class helps defining serial interactions. Extend with the functions a
 * menu should implement and pass them to the constructor. Call start() to use
 * the menu.
 */
class SerialMenu {

    public:

        /**
         * SerialMenu constructor
         * @param n string name of menu
         * @param fs array of SerialMenuFunctions
         * @param s size of array
         */
        SerialMenu(const char* n, const SerialMenuFunction* fs, int s);

        /**
         * Call this to show the menu and watch for inputs.
         */
        void start();

    protected:

        /**
         * Just waits for a character to be sent to serial input.
         * @return command from serial
         */
        static char getSerialCommand();

    private:

        // Menu name
        const char* name;
        // Array of functions
        const SerialMenuFunction* functions;
        // Size of functions array
        const int size;
        // Used by start(), prints menu to Serial
        void printMenu();

};

#endif
