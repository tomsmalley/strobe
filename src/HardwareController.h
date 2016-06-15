#ifndef HARDWARE_CONTROLLER_H
#define HARDWARE_CONTROLLER_H

#include <cstdint>

#include "WProgram.h" // For pin definitions

class ADC;

class HardwareController {

    public:

        HardwareController();

        // Matrix
        static const uint8_t NUM_COLS = 16;
        static const uint8_t NUM_ROWS = 8; // 7051 spec

        // Pins
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

        // Row functions
        void selectRow(uint8_t row) const;
        uint8_t readRow() const;

        // Column functions
        void setColHigh(uint8_t col) const;
        void setColLow(uint8_t col) const;

    private:

        ADC* adc;

};

extern const HardwareController* hardwareController;

#endif
