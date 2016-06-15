Capacitive Sensing Keyboard Firmware
====================================

Rudimentary firmware for capacitive sensing. Work in progress.
See the [deskthority
thread](https://deskthority.net/workshop-f7/designing-a-custom-topre-board-t11734.html)
for how the keyboard is coming along.


What is done
------------

- Sensing key state
- Keymap storage in EEPROM
- Basic layer system (partial)
- USB functions
  - USB HID Keyboard functions
  - Some mouse controls
- Calibration
  - Noise floor measurement
  - Basic calibration routine


Still to do
-----------

- Complete layer system
- USB functions
  - The rest of the mouse controls
  - Analog controller? (like joystick)
- Calibration
  - Set hysteresis window accordingly
  - Adjustable activation point
- On the fly macro recording
- Implement master/slave mode and communication
- Refactor main loop, learn how to use interrupts
- Tidy it all up
- Nice setup method


Setup and usage
---------------

This project uses
[teensy-template](https://github.com/apmorton/teensy-template).
