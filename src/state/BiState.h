#ifndef BI_STATE_H
#define BI_STATE_H

#include <cstdint>

class BiState {

    public:
        virtual void update(uint8_t payload, uint8_t operation) = 0;

};

#endif
