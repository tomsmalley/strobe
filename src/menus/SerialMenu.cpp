#include "SerialMenu.h"

#include <WProgram.h>

// Constructor just sets name, functions, and number of functions
SerialMenu::SerialMenu(const char* n, const SerialMenuFunction* fs, int s): name(n), functions(fs), size(s) {}

// Simple print function to show the menu
void SerialMenu::printMenu() {
    // Show the menu name
    Serial.println();
    Serial.print("*** ");
    Serial.println(name);
    // List the available functions with the required inputs
    for (int i = 0; i < size; i++) {
        Serial.print("'");
        Serial.print(functions[i].getCommand());
        Serial.print("' - ");
        Serial.println(functions[i].getName());
    }
    // Always list quit option
    Serial.println("'q' - Quit");
    Serial.print(": ");
}

// Main menu function
void SerialMenu::start() {
    printMenu();
    // Just constantly look for serial commands
    while (true) {
        char command = getSerialCommand();
        Serial.println(); // Clear line
        // q should always quit the menu
        if (command == 'q') {
            Serial.println("Quitting...");
            return;
        }
        // Go through all other function commands
        for (int i = 0; i < size; i++) {
            // Call the function if a match is found
            if (command == functions[i].getCommand()) {
                functions[i].execute();
            }
        }
        // Print the menu after the command
        printMenu();
    }
}

// Just wait for a character to be input
char SerialMenu::getSerialCommand() {
    while(true) {
        if (Serial.available()) {
            return Serial.read();
        }
    }
}
