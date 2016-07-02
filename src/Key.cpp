#include "Key.h"

#include <WProgram.h>

#include "Persist.h"
#include "HardwareController.h"

Key::Key() { }

uint8_t Key::normalise(uint8_t row, uint8_t col, uint8_t value) {
    uint16_t calMin = Persist::getCalMin(row, col);
    uint16_t calMax = Persist::getCalMax(row, col);
    // Clamp to max and min values
    if (value < calMin) {
        value = calMin;
    } else if (value > calMax) {
        value = calMax;
    }
    // Feature scaling, scale needs to be done before int division
    // Cast is okay because the fraction must be between 0 and 1 due to
    // clamping.
    uint16_t numerator = 255 * (value - calMin);
    uint8_t denominator = calMax - calMin;
    return (uint8_t) (numerator / denominator);
}
