#ifndef LAYER_STATE_H
#define LAYER_STATE_H

#include <cstdint>
#include "Actions.h"

class LayerState {

    public:
        void update(uint8_t payload, Operation operation);
        bool isActive(uint8_t layer);

    private:
        void setLayer(uint8_t layer);
        void unsetLayer(uint8_t layer);
        void toggleLayer(uint8_t layer);

        uint16_t layers = 1;
        uint8_t MAX_LAYERS = 16;

};

#endif
