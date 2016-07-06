#ifndef PERSIST_H
#define PERSIST_H

#include <cstdint>

enum class Route : uint8_t;

#define SETTINGS_COUNT 6 // Must update this when adding settings
enum class Setting : uint8_t
    { DEADZONE      = 0
    , SENSITIVITY   = 1
    , MIN_THRESHOLD = 2
    , MAX_THRESHOLD = 3
    , NOISE_FLOOR   = 4
    , LAYER_COUNT   = 5
    };

/**
 * This class defines how EEPROM is allocated and provides accessor functions.
 * Memory here is small and we want to store keymaps and macros, so we need a
 * compact method of allocation. From byte 0 to 2048:
 *  size : allocation
 * ------:------------
 *  s_S  : User settings
 *  RxC  : Key user ID (addressed by hardware ID)
 *  RxC  : Key cal min (addressed by hardware ID)
 *  RxC  : Key cal max (addressed by hardware ID)
 *   N   : Keymap layer 0
 *  N/4  : Routes layer 0
 *   N   : Keymap layer 1
 *  N/4  : Routes layer 1
 *  ...  : ...
 *   N   : Keymap layer [LAYER_COUNT]
 *  N/4  : Routes layer [LAYER_COUNT]
 *   ?   : Empty space
 *   M   : Macros (in reverse)
 * ------:------------
 *
 * All key functions are addressed using the key user ID unless specified
 * otherwise.
 */
class Persist {

    public:

        /**
         * User settings
         */
        static uint8_t getSetting(Setting setting);
        static void setSetting(Setting setting, uint8_t value);

        /**
         * Check if a key is in this matrix.
         */
        static bool matrixPositionActive(uint8_t row, uint8_t col);
        static void setMatrixPositionActive(uint8_t row, uint8_t col, bool
                active);

        static uint8_t getUserID(uint8_t row, uint8_t col);
        static void setUserID(uint8_t row, uint8_t col, uint8_t userID);
        static uint8_t getHardwareID(uint8_t row, uint8_t col);

        static uint8_t getCalMin(uint8_t row, uint8_t col);
        static void setCalMin(uint8_t row, uint8_t col, uint8_t value);

        static uint8_t getCalMax(uint8_t row, uint8_t col);
        static void setCalMax(uint8_t row, uint8_t col, uint8_t value);

        /**
         * Get/set action for a key on a given layer.
         */
        static Route getRoute(uint8_t keyID, uint8_t layer);
        static void setRoute(uint8_t keyID, uint8_t layer, Route route);
        static uint8_t getPayload(uint8_t keyID, uint8_t layer);
        static void setPayload(uint8_t keyID, uint8_t layer, uint8_t payload);

    private:

        // Helper functions for input sanitization
        static uint8_t maskKeyID(uint8_t keyID);
        static uint8_t maskLayer(uint8_t maskLayer);

        static uint8_t routeShift(uint8_t keyID);
        static uint8_t routeOffset(uint8_t keyID);

};

#endif
