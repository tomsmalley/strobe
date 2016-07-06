#include "JoystickState.h"

#include <cstdint>
#include <WProgram.h>
#include "usb_dev.h"
#include "core_pins.h"
#include "Actions.h"
#include "USBTimeout.h"

void JoystickState::resetAnalog() {

}

void JoystickState::send() {
    uint32_t waitCount = 0;
    usb_packet_t *txPacket;

    while(true) {
        if (!usb_configuration) {
            return;
        }
        if (usb_tx_packet_count(JOYSTICK_ENDPOINT) < TX_PACKET_LIMIT) {
            txPacket = usb_malloc();
            if (txPacket) break;
        }
        if (++waitCount > TX_TIMEOUT || transmitPreviousTimeout) {
            transmitPreviousTimeout = true;
            return;
        }
        yield();
    }
    transmitPreviousTimeout = false;
    // build packet
    usb_tx(JOYSTICK_ENDPOINT, txPacket);
}
