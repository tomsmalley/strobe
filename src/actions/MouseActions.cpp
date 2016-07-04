#include "MouseActions.h"

#include <WProgram.h>
#include "usb_dev.h"
#include "core_pins.h" // for yield()

#include "USBTimeout.h"
#include "State.h"

// Mouse key defs
#define CURSOR_UP           0x00
#define CURSOR_DOWN         0x01
#define CURSOR_LEFT         0x02
#define CURSOR_RIGHT        0x03

#define CURSOR_UP_LEFT      0x04
#define CURSOR_UP_RIGHT     0x05
#define CURSOR_DOWN_LEFT    0x06
#define CURSOR_DOWN_RIGHT   0x07

#define BUTTON_1            0x11
#define BUTTON_2            0x12
#define BUTTON_3            0x13
#define BUTTON_4            0x14
#define BUTTON_5            0x15

#define SCROLL_UP           0x20
#define SCROLL_DOWN         0x21
#define SCROLL_LEFT         0x22
#define SCROLL_RIGHT        0x23

#define SCROLL_UP_LEFT      0x24
#define SCROLL_UP_RIGHT     0x25
#define SCROLL_DOWN_LEFT    0x26
#define SCROLL_DOWN_RIGHT   0x27

void MouseActions::handle(uint8_t action, uint8_t depth, State* state) {

    if ((action == CURSOR_UP_LEFT) || (action == CURSOR_UP)
            || (action == CURSOR_UP_RIGHT)) {
        state->moveMouseUp(depth);
    }
    if ((action == CURSOR_DOWN_LEFT) || (action == CURSOR_DOWN)
            || (action == CURSOR_DOWN_RIGHT)) {
        state->moveMouseDown(depth);
    }
    if ((action == CURSOR_UP_RIGHT) || (action == CURSOR_RIGHT)
            || (action == CURSOR_DOWN_RIGHT)) {
        state->moveMouseRight(depth);
    }
    if ((action == CURSOR_UP_LEFT) || (action == CURSOR_LEFT)
            || (action == CURSOR_DOWN_LEFT)) {
        state->moveMouseLeft(depth);
    }

}


static uint8_t transmit_previous_timeout=0;

void MouseActions::sendState() {
    // From Teensy usb_mouse.c
    uint32_t wait_count=0;
    usb_packet_t *tx_packet;

    if (cursorX == -128) cursorX = -127;
    if (cursorY == -128) cursorY = -127;
    if (scrollX == -128) scrollX = -127;
    if (scrollY == -128) scrollY = -127;

    while (1) {
            if (!usb_configuration) {
                    return;
            }
            if (usb_tx_packet_count(MOUSE_ENDPOINT) < TX_PACKET_LIMIT) {
                    tx_packet = usb_malloc();
                    if (tx_packet) break;
            }
            if (++wait_count > TX_TIMEOUT || transmit_previous_timeout) {
                    transmit_previous_timeout = 1;
                    return;
            }
            yield();
    }
    transmit_previous_timeout = 0;
    *(tx_packet->buf + 0) = 1;
    *(tx_packet->buf + 1) = buttons;
    *(tx_packet->buf + 2) = cursorX;
    *(tx_packet->buf + 3) = cursorY;
    *(tx_packet->buf + 4) = scrollY;
    *(tx_packet->buf + 5) = scrollX; // horizontal scroll
    tx_packet->len = 6;
    usb_tx(MOUSE_ENDPOINT, tx_packet);
    return;
}
