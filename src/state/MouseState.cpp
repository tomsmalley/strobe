#include "MouseState.h"

#include <cstdint>
#include <WProgram.h>
#include "usb_dev.h"
#include "core_pins.h"
#include "Actions.h"
#include "USBTimeout.h"
#include "Persist.h"

void MouseState::resetAnalog() {
    mouseX = 0;
    mouseY = 0;
    scrollX = 0;
    scrollY = 0;
}

void MouseState::initState() {
    resetAnalog();
    buttons = 0;
}

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

void MouseState::updateAnalog(uint8_t payload, uint8_t depth) {
    if ((payload == CURSOR_UP_LEFT) || (payload == CURSOR_UP)
            || (payload == CURSOR_UP_RIGHT)) {
        moveMouseUp(depth);
    }
    if ((payload == CURSOR_DOWN_LEFT) || (payload == CURSOR_DOWN)
            || (payload == CURSOR_DOWN_RIGHT)) {
        moveMouseDown(depth);
    }
    if ((payload == CURSOR_UP_RIGHT) || (payload == CURSOR_RIGHT)
            || (payload == CURSOR_DOWN_RIGHT)) {
        moveMouseRight(depth);
    }
    if ((payload == CURSOR_UP_LEFT) || (payload == CURSOR_LEFT)
            || (payload == CURSOR_DOWN_LEFT)) {
        moveMouseLeft(depth);
    }
    // wheel TODO
}

void MouseState::update(uint8_t payload, uint8_t operation) {

}

uint8_t MouseState::mouseVel(uint8_t value) {
    const uint16_t DEADZONE = Persist::getDeadZone();
    const uint16_t SENSITIVITY = Persist::getSensitivity();
    if (value < DEADZONE) value = DEADZONE;
    uint16_t numerator = SENSITIVITY * (value - DEADZONE);
    uint8_t denominator = 255 - DEADZONE;
    return (uint8_t) (numerator / denominator);
}

// TODO make sure they cant overflow.
void MouseState::moveMouseUp(uint8_t depth) {
    uint8_t v = mouseVel(depth);
    if ((int)mouseY - v < -127) {
        mouseY = -127;
    } else {
        mouseY -= v;
    }
}
void MouseState::moveMouseRight(uint8_t depth) {
    uint8_t v = mouseVel(depth);
    if ((int)mouseX + v > 127) {
        mouseX = 127;
    } else {
        mouseX += v;
    }
}
void MouseState::moveMouseDown(uint8_t depth) {
    uint8_t v = mouseVel(depth);
    if ((int)mouseY + v > 127) {
        mouseY = 127;
    } else {
        mouseY += v;
    }
}
void MouseState::moveMouseLeft(uint8_t depth) {
    uint8_t v = mouseVel(depth);
    if ((int)mouseX - v < -127) {
        mouseX = -127;
    } else {
        mouseX -= v;
    }
}

void MouseState::send() {
    uint32_t waitCount = 0;
    usb_packet_t *txPacket;

    while(true) {
        if (!usb_configuration) {
            return;
        }
        if (usb_tx_packet_count(MOUSE_ENDPOINT) < TX_PACKET_LIMIT) {
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
    *(txPacket->buf + 0) = 1;
    *(txPacket->buf + 1) = buttons;
    *(txPacket->buf + 2) = mouseX;
    *(txPacket->buf + 3) = mouseY;
    *(txPacket->buf + 4) = scrollY;
    *(txPacket->buf + 5) = scrollX;
    txPacket->len = 6;
    usb_tx(MOUSE_ENDPOINT, txPacket);
}
