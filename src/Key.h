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
         * Normalise a 0-255 ADC reading using calibration values (feature
         * scaling). The return value denotes keydepth from 0 (unpressed) to
         * 255 (fully pressed).
         * @param keyID key id
         * @param value ADC reading
         * @return normalised reading
         */
        static uint8_t normalise(uint8_t row, uint8_t col, uint8_t val);

        // Probably should make these private
        uint8_t depth = 0;
        bool pressed = false;

};

#endif
