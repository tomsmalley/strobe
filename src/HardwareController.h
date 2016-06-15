#ifndef HARDWARE_CONTROLLER_H
#define HARDWARE_CONTROLLER_H

#include <cstdint>

#include "WProgram.h" // For pin definitions

class ADC;

/**
 * Contains pin definitions and controls the matrix.
 */
class HardwareController {

    public:

        /**
         * Use non static methods so that this class has to be instantiated to
         * be used, that way pin setup can be done in constructor.
         */
        HardwareController();

        /*** Matrix ***/

        static const uint8_t NUM_COLS = 16;
        static const uint8_t NUM_ROWS = 8; // 7051 spec

        /*** Row functions ***/

        /**
         * Select a row on the multiplexer.
         * @param row number of row to select (0-7)
         */
        void selectRow(uint8_t row) const;

        /**
         * Read the selected row with the ADC.
         * @return ADC reading (0-255)
         */
        uint8_t readRow() const;

        /*** Col functions ***/

        /**
         * Set column to 3.3V (strobe).
         * @param col number of col to select (0-15)
         */
        void setColHigh(uint8_t col) const;

        /**
         * Set column to 0V (relax).
         * @param col number of col to select (0-15)
         */
        void setColLow(uint8_t col) const;

        /*** LED functions ***/

        /**
         * Turn on LED.
         */
        void turnOnLED() const;

        /**
         * Turn off LED.
         */
        void turnOffLED() const;

    private:

        /*** ADC object, initialised in constructor ***/

        ADC* adc;

        /*** Pin definitions, must be unique, init in constructor ***/

        const int PIN_LED = 13;
        // Pin from row line multiplexer
        const int PIN_ROW_READ = A0;
        // Control pins to address the 8 mux channels
        const int PIN_MUX_CONTROL[3] = { 3, 2, 1 };
        const int PIN_COL[16] =
        {  4,  5,  6,  7
        ,  8,  9, 10, 11
        , 12, 15, 16, 17
        , 18, 19, 20, 21
        };


};

// Global object
extern const HardwareController* controller;

#endif
