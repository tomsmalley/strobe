#ifndef COLUMNCONTROLLER_H
#define COLUMNCONTROLLER_H

#include <cstdint>

class ColumnController {

    public:
        ColumnController();

        static const uint8_t NUM_COLUMNS = 16;

        void setHigh(uint8_t col);
        void setLow(uint8_t col);

    private:
        int columnPin(uint8_t columnID);

};

#endif
