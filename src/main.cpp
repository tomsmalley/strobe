#include <WProgram.h>

#include "controllers.h"
#include "Key.h"
#include "Persist.h"
#include "KeyMap.h"
#include "State.h"

// Escape codes for serial printing
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

const int LED_PIN = 13;

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

    pinMode(LED_PIN, OUTPUT);

    // Flash LED to show it is working
    for (int i = 0; i < 10; i++) {
        digitalWrite(LED_PIN, HIGH);
        delay(100);
        digitalWrite(LED_PIN, LOW);
        delay(100);
    }

    // print fancy intro
    Serial.println(ANSI_COLOR_RESET);
    Serial.println("          _   _   _        "
        ANSI_COLOR_CYAN "  _       " ANSI_COLOR_RESET
        "                     _     ");
    Serial.println("     __ _| |_| |_| |_  ___ "
        ANSI_COLOR_CYAN " (_)__ __ " ANSI_COLOR_RESET
        " __ _ _ _ __ __ _ __| |___ ");
    Serial.println("    / _` |  _|  _| ' \\/ -_)"
        ANSI_COLOR_CYAN "/ / _/ -_)" ANSI_COLOR_RESET
        "/ _` | '_/ _/ _` / _` / -_)");
    Serial.println("    \\__,_|\\__|\\__|_||_\\__/"
        ANSI_COLOR_CYAN "/_/\\__/\\_/ " ANSI_COLOR_RESET
        "\\__,_|_| \\__\\__,_\\__,_\\___|");
    Serial.println(ANSI_COLOR_RESET);
    Serial.println("          "
        "      Capacitive sensing keyboard firmware");
    Serial.println("          "
        "        --------------------------------     ");
    Serial.println("          "
        "          https://github.com/tomsmalley          ");
    Serial.println();
    Serial.println("          "
        "              Type 'm' for the menu              ");

    // Make sure the message prints fully and isn't waiting for the rest of the
    // packet
    Serial.send_now();

    // DEBUGGING TEMP SETUP TODO
    // Settings
    Persist::setMinThreshold(127);
    Persist::setMaxThreshold(153);
    Persist::setSensitivity(20);
    Persist::setDeadZone(10);

    // Mappings
    Persist::setMapping(0, 1, 0xA5);
    Persist::setMapping(1, 1, 0xA7);
    Persist::setMapping(2, 1, 0xAA);
    Persist::setMapping(3, 1, 0xAC);

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
    if (master && Serial.available()) {
        if (Serial.read() == 'm') {
            // TODO needs to actually be a menu
            menu->start();
        }
    }

    // Update keyboard state. Each loop takes less than 20 us without waiting
    for (int i = 0; i < State::NUM_KEYS; i++) {
        // Record time to make sure we wait long enough
        elapsedMicros time;

        // Get the mapping for this key
        uint8_t mapping = getMapping(i);

        // Read (or attempt to get) the key depth
        if (Persist::keyIsInMatrix(i)) {
            // Read key state, normalise, and store (16 us)
            uint8_t reading = Key::strobeRead(i, controllers::row,
                    controllers::column);
            state->keys[i]->depth = Key::normalise(i, reading);
        } else if (master) {
            // If the key isn't in this matrix and this is the master, we can
            // check the slave. If no slave is connected then the reply is
            // always 0 which denotes no press
            state->keys[i]->depth = requestFromSlave(i);
        }

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

        // Wait for 130 us
        if (time < 130) {
            delayMicroseconds(130 - time);
        }

    }

    if (master) {
        // TODO make this interrupt poll at 60Hz
        Keyboard.send_now();
        Mouse.move(state->getMouseX(), state->getMouseY());
        state->resetMousePos();
    }

}

extern "C" int main(void) {

    setup();
    while (1) {
        loop();
    }

}
