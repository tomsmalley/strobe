Capacitive Sensing Keyboard Firmware
====================================

Rudimentary firmware for capacitive sensing. Work in progress.
See the [deskthority
thread](https://deskthority.net/workshop-f7/designing-a-custom-topre-board-t11734.html)
for how the keyboard is coming along.


What is done
------------

- Sensing key state
- Basic calibration routine
- Keymap storage in EEPROM
- Basic layer system
- USB keyboard 'selector' IDs now sending


Still to do
-----------

- Nicer layer system
- Full USB HID keyboard functions
- Analog mouse controls
- Refactor main loop, learn how to use interrupts
- Implement master/slave mode and communication
- Analog controller? (like joystick)
- Tidy it all up
- Nice setup method


Setup and usage
---------------

This project uses
[teensy-template](https://github.com/apmorton/teensy-template).
