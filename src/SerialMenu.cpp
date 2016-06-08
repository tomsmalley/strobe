#include "SerialMenu.h"

#include <WProgram.h>

SerialMenu::SerialMenu(const char* n, SerialMenuFunction* fs, int s): name(n), functions(fs), size(s) {
}

void SerialMenu::printMenu() {
    Serial.println("----------------------------");
    Serial.print(name);
    Serial.println(" mode.");

    for (int i = 0; i < size; i++) {
        Serial.print("'");
        Serial.print(functions[i].getCommand());
        Serial.print("' - ");
        Serial.println(functions[i].getName());
    }

    Serial.println("'q' - Quit");
    Serial.println("----------------------------");
}

void SerialMenu::start() {
    printMenu();
    while(true) {
        if (Serial.available()) {
            char command = Serial.read();
            if (command == 'q') {
                return;
            }
            for (int i = 0; i < size; i++) {
                if (command == functions[i].getCommand()) {
                    functions[i].execute();
                }
            }
            printMenu();
        }
    }
}
