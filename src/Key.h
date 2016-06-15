#ifndef __KEY_H__
#define __KEY_H__

#include <cstdint>

/**
 * Holds key state and provides functions to measure key state.
 */
class Key {

    public:

        Key();

        /**
         * Strobe a column and read the row voltage. Must wait long enough for
         * rows to relax between function calls! Returns 0-255 denoting row
         * voltage (not key depth!).
         * @param keyID key id number
         * @param row controller for the row
         * @param col controller for the column
         * @return ADC reading for row voltage
         */
        static uint8_t strobeRead(int8_t keyID);

        /**
         * Normalise a 0-255 ADC reading using calibration values (feature
         * scaling). The return value denotes keydepth from 0 (unpressed) to
         * 255 (fully pressed).
         * @param keyID key id
         * @param value ADC reading
         * @return normalised reading
         */
        static uint8_t normalise(int8_t keyID, uint8_t val);

        // Probably should make these private
        uint8_t depth = 0;
        bool pressed = false;

};

#endif
