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


        // Matrix definitions, public for loop limits
        static const uint8_t NUM_STROBES = 6;
        static const uint8_t NUM_READS = 7;


        /*** Reading functions ***/

        /**
         * Strobe a column and read the (preselected) row voltage.
         * Must wait long enough for rows to relax between function calls!
         * Returns 0-255 denoting row voltage (not key depth!).
         * @param strobeLine strobe line number
         * @return ADC reading for row voltage
         */
        uint8_t strobeRead(uint8_t strobeLine);

        /**
         * Select a row on the multiplexer.
         * @param line number of read line to select (0-7)
         */
        void selectReadLine(uint8_t line) const;


        /*** LED functions ***/

        void turnOnLED() const;
        void turnOffLED() const;

    private:

        /*** Row functions ***/

        /**
         * Read the selected line with the ADC.
         * @return ADC reading (0-255)
         */
        uint8_t readSelected() const;


        /*** Col functions ***/

        /**
         * Set strobe line to 3.3V
         * @param col number of col to select (0-15)
         */
        void setStrobeHigh(uint8_t line) const;

        /**
         * Set strobe line to 0V
         * @param col number of col to select (0-15)
         */
        void setStrobeLow(uint8_t line) const;


        /*** Drain functions ***/

        /**
         * Set drain to float.
         */
        void setDrainFloating() const;

        /**
         * Set drain to ground.
         */
        void setDrainGround() const;


        // ADC object, setup in constructor
        ADC* adc;

        // Time since last read, reset every strobeRead
        elapsedMicros timeSinceLastStrobe;

        /*** Pin definitions, must be unique, init in constructor ***/

        const int PIN_LED = 13;
        // Pin from row line multiplexer
        const int PIN_READ = A0;
        // Control pins to address the 8 mux channels
        const int PIN_MUX_CONTROL[3] = { 3, 2, 1 };
        const int PIN_STROBE[NUM_STROBES] = { 11, 12, 27, 28, 29, 30 };
        // Fast drain pin
        const int PIN_DRAIN = 15;

        // How long it takes the matrix to relax fully in microseconds
        // Determined by RC circuit time constant * 5
        const uint8_t RELAX_TIME = 5;


};

// Global object
extern HardwareController* controller;

#endif
