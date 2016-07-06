#include <WProgram.h>

#include "HardwareController.h"
#include "Key.h"
#include "Persist.h"
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
    state = new State();

    Serial.begin(0);

    // Flash LED to show it is working
    for (int i = 0; i < 5; i++) {
        controller->turnOnLED();
        delay(100);
        controller->turnOffLED();
        delay(100);
    }


    // TODO Determine if we are master
    bool master = true;

    // DEBUGGING TEMP SETUP TODO
    // Settings
    Persist::setMinThreshold(127);
    Persist::setMaxThreshold(153);
    Persist::setSensitivity(20);
    Persist::setDeadZone(10);

    // Mappings
    Persist::setLayerCount(6);

    Persist::setUserID(2, 0, 0);
    Persist::setUserID(2, 1, 1);

    // button 0
    Persist::setRoute(0, 0, 1);
    Persist::setPayload(0, 0, 0x04);
    Persist::setRoute(0, 1, 1);
    Persist::setPayload(0, 1, 0x05);
    Persist::setRoute(0, 2, 1);
    Persist::setPayload(0, 2, 0x06);

    // button 1
    Persist::setRoute(1, 0, 2);
    Persist::setPayload(1, 0, 0xD1); // toggle L1
    Persist::setRoute(1, 1, 2);
    Persist::setPayload(1, 1, 0xD2); // toggle L2
    Persist::setRoute(1, 2, 2);
    Persist::setPayload(1, 2, 0xD0); // reset layer

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
        if (Serial.available()) {
            if (Serial.read() == 'm') {
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
                // Make sure the message prints fully and isn't waiting for the
                // rest of the packet
                Serial.send_now();
                menu->start();
            }
        }
    }

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


            // Read (or attempt to get) the key depth
            if (Persist::matrixPositionActive(i, j)) {
                uint8_t keyID = Persist::getUserID(i, j);
                if (keyID > state->NUM_KEYS - 1) break;
                // Read key state, normalise, and store (16 us)
                uint8_t reading = controller->strobeRead(j);
                state->keys[keyID]->depth = Key::normalise(i, j, reading);
            }

            /*else if (master) {
                // If the key isn't in this matrix and this is the master, we can
                // check the slave. If no slave is connected then the reply is
                // always 0 which denotes no press
                state->keys[j]->depth = requestFromSlave(i);
            }
            */

        }
    }

    // The rest only done on master
    if (master) {
        state->updateState();
    }

}

extern "C" int main(void) {

    setup();
    while (1) {
        loop();
    }

}
