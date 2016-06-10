#include <WProgram.h>

#include "globals.h"
#include "Key.h"

// Escape codes for serial printing
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


void printNormalMenu();
void loop();

const int LED_PIN = 13;

boolean debugging = false;

// this whole file is a mess for now
#include "Calibration.h"
#include "MatrixSetup.h"

Calibration cal = Calibration();
MatrixSetup mat = MatrixSetup();


// current layer (0-8) (default to 1) (fn key hold gives layer 0)
// this should never be 0, only holds user selected layers that aren't the
// function layer (layer 0)

/**
 * Setup function
 */
void setup() {

    for (int i = 0; i < NUM_KEYS; i++) {
        state::keys[i] = new Key();
    }

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

    printNormalMenu();

    //usb_init();

}



void printNormalMenu() {
    Serial.println("-----------------------------");
    Serial.println("Normal mode. Send:");
    Serial.println("'c' to enter calibration mode");
    Serial.println("'m' to enter matrix setup mode");
    Serial.println("'d' to toggle debugging");
    Serial.println("-----------------------------");
}

/**
 * Main program loop
 */
void loop() {

    // Check for serial commands
    if (Serial.available()) {
        char incoming = Serial.read();
        switch (incoming) {
            case 'c':
                cal.start();
                break;
            case 'm':
                mat.start();
                break;
            case 'd':
                debugging = !debugging;
                if (debugging) {
                    Serial.println("Debugging on");
                } else {
                    Serial.println("Debugging off");
                }
                break;
        }
        //printNormalMenu();
    }

    // Measure the key states
    for (int i = 0; i < NUM_KEYS; i++) {
        // Record the time for later
        elapsedMicros time;
        // this loop takes around 30 us

        // Only consider valid keys
        if (Key::getRow(i) >= 0 && Key::getCol(i) >= 0) {

            // Measure key reading and store it
            uint8_t reading = Key::strobeRead(i, controllers::row, controllers::column);
            uint8_t value = Key::normalise(i, reading);
            state::keys[i]->state = value;

            uint8_t mapping;
            // Get the mapping for the fn layer or current layer
            if (state::fnPressed) {
                mapping = Key::getMapping(i, 0);
            } else {
                mapping = Key::getMapping(i, state::layer);
            }
            // Fallback to default layer
            if (mapping == 0) {
                mapping = Key::getMapping(i, 1);
            }

            // Hysteresis for determining if key is pressed
            // If key was pressed last iteration
            if (state::keys[i]->pressed) {
                // and it has dropped below threshold, set to not pressed
                if (value < 0.5) {
                    state::keys[i]->pressed = false;
                    // hold key fn off TODO
                    if (mapping == 0xEF) {
                        state::fnPressed = false;
                    }
                }
            // Or if it wasn't pressed
            } else {
                // and it has risen above threshold, set to pressed
                if (value > 0.6) {
                    state::keys[i]->pressed = true;
                    // hold key fn
                    // this needs do be done in a seperate loop incase there
                    // are two function keys, pressing both and releasing only
                    // one would unset the pressed state! TODO
                    if (mapping == 0xEF) {
                        state::fnPressed = true;
                    }
                }
            }

            // Parse the mapping
            // Only consider mapped keys
            if (mapping > 0) { // conveniently 0 is reserved in USB spec
                // I am using it to denote no mapping
                if (mapping < 0xE8) { // this denotes something in USB spec
                    // Just send it using teensy keyboard!
                    // TODO Make function which takes USB code and captures the
                    // first 6 it is sent
                    //Keyboard.set_key1(mapping);
                } else if (mapping > 0xE8 && mapping <= 0xEF) {
                    // Layer switching
                    // 0xE8  Layer 0 (fn layer, ignore here)
                    // 0xE9  Layer 1
                    // 0xEA  Layer 2
                    // 0xEB  Layer 3
                    // 0xEC  Layer 4
                    // 0xED  Layer 5
                    // 0xEE  Layer 6
                    // 0xEF  Layer 7
                    // Maybe increment/decrement layer??
                    state::layer = mapping - 0xE8;
                } else if (mapping >= 0xF0 && mapping <= 0xFA) {
                    // Mouse key defs
                    // 0xF0 Left movement
                    // 0xF1 Down movement
                    // 0xF2 Up movement
                    // 0xF3 Right movement
                    // maybe allocate 4 more for diagonals??
                    // 0xF8 Left click
                    // 0xF9 Middle click
                    // 0xFA Right click
                    // and maybe 2 for scroll ?
                } else if (mapping == 0xFF) {
                    // 0xFF keyboard lock
                }
            }

        }

        // Make sure we have waited 150 us before reading the next key
        // changed to 130 us since 130 us * 128 keys * 60 Hz gives 1 second
        if (time < 130) {
            delayMicroseconds(130 - time);
        } else {
            Serial.print("Overrunning loop by ");
            Serial.print(time - 130);
            Serial.println(" us!");
        }

    }

    // send keyboard state
    Keyboard.send_now();

    /*
    //mousekeys playground
    float valueX = state::keys[2]->state - state::keys[1]->state;
    float valueY = state::keys[3]->state - state::keys[0]->state;
    int SENSITIVITY = 20; // max 127
    float DEADZONE = 0.05;
    float rescaleX = SENSITIVITY * (abs(valueX) - DEADZONE)
                    / (float)(1 - DEADZONE);
    float rescaleY = SENSITIVITY * (abs(valueY) - DEADZONE)
                    / (float)(1 - DEADZONE);
    if (rescaleX < 0) {
        rescaleX = 0;
    }
    if (rescaleY < 0) {
        rescaleY = 0;
    }
    if (valueX < 0) {
        rescaleX = -rescaleX;
    }
    if (valueY < 0) {
        rescaleY = -rescaleY;
    }
    Mouse.move((int)rescaleX, (int)rescaleY);
    */

    // Debugging loop
    // Turn off LED by default
    digitalWrite(LED_PIN, LOW);
    if (debugging) {
        unsigned long millis();
        for (int i = 0; i < NUM_KEYS; i++) {
            if (Key::getRow(i) >= 0 || Key::getCol(i) >= 0) {
                Serial.print("Key ");
                Serial.print(i);
                Serial.print(": ");
                if (state::keys[i]->pressed) {
                    Serial.print("P ");
                    // Turn on LED if a key is pressed
                    digitalWrite(LED_PIN, HIGH);
                } else {
                    Serial.print("  ");
                }
                Serial.print(state::keys[i]->state);
                Serial.print(" ");
                // Wait a little while
                delay(1);
            }
        }
        Serial.println();
    }

}

extern "C" int main(void) {

    setup();
    while (1) {
        loop();
    }

}
