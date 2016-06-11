#ifndef ROWCONTROLLER_H
#define ROWCONTROLLER_H

#include <cstdint>
#include "WProgram.h"

class ADC;

class RowController {

    public:
        RowController();

        // 7051 specification
        static const uint8_t NUM_ROWS = 8;

        void select(uint8_t rowID);

        uint8_t read();

    private:
        ADC* adc;
        // Row read pin, must be analog in on ADC_0
        static const int ROW_READ_PIN = A0;
        // 3 pins which control multiplexer, in order
        const int MUX_CONTROL_PIN[3] = { 3, 2, 1 };

};

#endif
