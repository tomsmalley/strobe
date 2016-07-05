#include "LayerState.h"

#include <cstdint>
#include <WProgram.h>

void LayerState::setLayer(uint8_t layer) {
    layers |= 1 << layer;
}

void LayerState::unsetLayer(uint8_t layer) {
    layers &= ~(1 << layer);
}

void LayerState::toggleLayer(uint8_t layer) {
    layers ^= 1 << layer;
}

void LayerState::resetLayers() {
    layers = 1;
}

#define SET 0
#define UNSET 1
#define TOGGLE 2

void LayerState::update(uint8_t payload, uint8_t operation) {
    uint8_t layer = payload - 0xD0;
    if (layer > MAX_LAYERS || layer == 0) {
        resetLayers();
        return;
    }
    switch(operation) {
            case SET:
                setLayer(layer);
                break;
            case UNSET:
                unsetLayer(layer);
                break;
            case TOGGLE:
                toggleLayer(layer);
                break;
    }
}

