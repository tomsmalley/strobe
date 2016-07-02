#include <WProgram.h>

#include "HardwareController.h"
#include "Key.h"
#include "Persist.h"
#include "KeyMap.h"
#include "State.h"
#include "EscapeCodes.h"

#include "MainMenu.h"
MainMenu* menu;
State* state;

/**
 * Setup function
 */
void setup() {

    state = new State();
    menu = new MainMenu();

    Serial.begin(0);

    // Flash LED to show it is working
    for (int i = 0; i < 5; i++) {
        controller->turnOnLED();
        delay(100);
        controller->turnOffLED();
        delay(100);
    }

    // DEBUGGING TEMP SETUP TODO
    // Settings
    /*
    Persist::setMinThreshold(127);
    Persist::setMaxThreshold(153);
    Persist::setSensitivity(20);
    Persist::setDeadZone(10);

    // Mappings
    Persist::setMapping(0, 1, 0xA5);
    Persist::setMapping(1, 1, 0xA7);
    Persist::setMapping(2, 1, 0xAA);
    Persist::setMapping(3, 1, 0xAC);
    */

}

uint8_t getMapping(int8_t keyID) {
    uint8_t mapping;
    // Get the mapping for the fn layer or current layer
    if (state->fnPressed) {
        mapping = Persist::getMapping(keyID, 0);
    } else {
        mapping = Persist::getMapping(keyID, state->layer);
    }
    // Fallback to default layer if there is no function declared
    if (mapping == 0) {
        mapping = Persist::getMapping(keyID, 1);
    }
    return mapping;
}

/**
 * TODO when 2nd teensy arrives
 */
uint8_t requestFromSlave(int8_t keyID) {
    // Interrupt slave over serial1 with keyID
    // Return slave response
    return 0;
}

void loop() {
    // Check if USB is connected and hence determine if master or slave
    // TODO
    bool master = true;
    // If this is the master we also need to do serial interaction stuff
    if (Serial.dtr()) {
        // Print fancy intro
        Serial.println(ANSI_COLOR_RED);
        Serial.println("       __           __      ");
        Serial.println("  ___ / /________  / /  ___ ");
        Serial.println(" (_-</ __/ __/ _ \\/ _ \\/ -_)");
        Serial.println("/___/\\__/_/  \\___/_.__/\\__/ ");
        Serial.println(ANSI_COLOR_RESET);
        Serial.println("Capacitive sensing keyboard firmware");
        Serial.println("*** https://github.com/tomsmalley/strobe");
        Serial.println();
        // Make sure the message prints fully and isn't waiting for the rest of the
        // packet
        Serial.send_now();
        menu->start();
    }
    /*
    if (master && Serial.available()) {
        if (Serial.read() == 'm') {
            // TODO needs to actually be a menu
            menu->start();
        }
    }
    */

    /*
    if (Serial.dtr()) {
        Serial.println();
        Serial.println("+-----+-----+-----+-----+-----+-----+-----+-----+-----+");
        Serial.println("|     |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |");
        Serial.println("| Row |  8  |  9  |  10 |  11 |  12 |  13 |  14 |  15 |");
        Serial.println("+-----+-----+-----+-----+-----+-----+-----+-----+-----+");
    }
    */
    // Update keyboard state.
    for (int i = 0; i < controller->NUM_READS; i++) {
        controller->selectReadLine(i);
        for(int j = 0; j < controller->NUM_STROBES; j++) {

            // Check in matrix
            if (Persist::matrixPositionActive(i, j)) {
                uint8_t reading = Key::normalise(i, j, controller->strobeRead(j));
            }

            // Get the mapping for this key
            //uint8_t mapping = getMapping(i);

            /*
            // Read (or attempt to get) the key depth
            if (Persist::keyIsInMatrix(i)) {
                // Read key state, normalise, and store (16 us)
                uint8_t reading = Key::strobeRead(i);
                state->keys[i]->depth = Key::normalise(i, reading);
            } else if (master) {
                // If the key isn't in this matrix and this is the master, we can
                // check the slave. If no slave is connected then the reply is
                // always 0 which denotes no press
                state->keys[i]->depth = requestFromSlave(i);
            }*/

            /*
        // The rest only done on master
        if (master) {

            // Analog handler
            KeyMap::handle(mapping, state->keys[i]->depth, state);

            // Hysteresis for determining if key is pressed
            // If key was pressed last iteration
            if (state->keys[i]->pressed) {
                // and it has dropped below threshold, set to not pressed
                if (state->keys[i]->depth < Persist::getMinThreshold()) {
                    state->keys[i]->pressed = false;
                    KeyMap::releaseEvent(mapping, state);
                }
            // Or if it wasn't pressed
            } else {
                // and it has risen above threshold, set to pressed
                if (state->keys[i]->depth > Persist::getMaxThreshold()) {
                    state->keys[i]->pressed = true;
                    KeyMap::pressEvent(mapping, state);
                }
            }

        }

        */
        }
    }

    if (master) {
        // TODO make this interrupt poll at 60Hz
        /*
        Keyboard.send_now();
        Mouse.move(state->getMouseX(), state->getMouseY());
        */
        state->resetMousePos();
    }

}

extern "C" int main(void) {

    setup();
    while (1) {
        loop();
    }

}
