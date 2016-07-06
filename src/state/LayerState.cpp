#include "LayerState.h"

#include <cstdint>
#include <WProgram.h>
#include "Actions.h"

bool LayerState::isActive(uint8_t layer) {
    if (layer > MAX_LAYERS) return 0;
    if (layer == 0) return 1;
    return (layers >> layer) & 1;
}
void LayerState::setLayer(uint8_t layer) {
    layers |= 1 << layer;
}

void LayerState::unsetLayer(uint8_t layer) {
    layers &= ~(1 << layer);
}

void LayerState::toggleLayer(uint8_t layer) {
    layers ^= 1 << layer;
}

void LayerState::update(uint8_t layer, uint8_t operation) {
    if (layer > MAX_LAYERS) return; // Do nothing if invalid
    if (layer == 0) {
        layers = 1; // Reset layers
    } else {
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
}

