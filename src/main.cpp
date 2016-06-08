#include <WProgram.h>

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


// needs initialising
Key* keys[Key::NUM_KEYS];


/**
 * Setup function
 */
void setup() {

    for (int i = 0; i < Key::NUM_KEYS; i++) {
        keys[i] = new Key();
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
        printNormalMenu();
    }

    // Loop over all the keys
    for (int i = 0; i < Key::NUM_KEYS; i++) {

        // Record the time for later
        elapsedMicros time;

        // Measure key reading and store it
        uint8_t reading = Key::strobeRead(i);
        float value = Key::normalise(i, reading);

        keys[i]->state = value;

        // Hysteresis for determining if key is pressed
        // If key was pressed last iteration
        if (keys[i]->pressed) {
            // and it has dropped below threshold, set to not pressed
            if (value < 0.5) {
                keys[i]->pressed = false;
            }
        // Or if it wasn't pressed
        } else {
            // and it has risen above threshold, set to pressed
            if (value > 0.6) {
                keys[i]->pressed = true;
            }
        }

        // Make sure we have waited 150 us before reading the next key
        if (time < 150) {
            delayMicroseconds(150 - time);
        }

    }

    // Debugging loop
    // Turn off LED by default
    digitalWrite(LED_PIN, LOW);
    if (debugging) {
        unsigned long millis();
        for (int i = 0; i < Key::NUM_KEYS; i++) {
            Serial.print("Key ");
            Serial.print(i);
            Serial.print(": ");
            if (keys[i]->pressed) {
                Serial.print("P ");
                // Turn on LED if a key is pressed
                digitalWrite(LED_PIN, HIGH);
            } else {
                Serial.print("  ");
            }
            Serial.print(keys[i]->state);
            Serial.print(" ");
            // Wait a little while
            delay(1);
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
