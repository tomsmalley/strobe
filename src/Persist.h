#ifndef PERSIST_H
#define PERSIST_H

#include <cstdint>


/**
 * This class defines how EEPROM is allocated and provides accessor functions.
 * Dividing the 2048 byte EEPROM of the mk20 into blocks of 128 bytes, there
 * are 16 blocks. These are allocated as follows:
 *
 * 0 -> User settings / persistent macros
 * 1 -> Key user ID (addressed by hardware ID)
 * 2 -> Key Cal Min (addressed by hardware ID)
 * 3 -> Key Cal Max (addressed by hardware ID)
 * 4 -> Keymap Layer 0
 * 6 -> Keymap Layer 1
 * 8 -> Keymap Layer 2
 * A -> Keymap Layer 3
 * C -> Keymap Layer 4
 * E -> Keymap Layer 5
 * All key functions are addressed using the key user ID unless specified
 * otherwise.
 */
class Persist {

    public:

        /**
         * Check if a key is in this matrix.
         * TODO update comments
         * @param keyID key ID
         * @return true if the key is in the matrix
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
        static uint8_t getRoute(uint8_t keyID, uint8_t layer);
        static void setRoute(uint8_t keyID, uint8_t layer, uint8_t route);
        static uint8_t getPayload(uint8_t keyID, uint8_t layer);
        static void setPayload(uint8_t keyID, uint8_t layer, uint8_t payload);

        /**
         * User settings
         */
        static uint8_t getDeadZone();
        static void setDeadZone(uint8_t deadZone);

        static uint8_t getSensitivity();
        static void setSensitivity(uint8_t sensitivity);

        static uint8_t getMinThreshold();
        static void setMinThreshold(uint8_t threshold);

        static uint8_t getMaxThreshold();
        static void setMaxThreshold(uint8_t threshold);

        static uint8_t getNoiseFloor();
        static void setNoiseFloor(uint8_t noise);

        static uint8_t getLayerCount();
        static void setLayerCount(uint8_t layers);

    private:

        // Helper functions for input sanitization
        static uint8_t maskKeyID(uint8_t keyID);
        static uint8_t maskLayer(uint8_t maskLayer);

        static uint8_t routeShift(uint8_t keyID);
        static uint8_t routeOffset(uint8_t keyID);

        /*
        // Memory locations
        static const int BLOCK_SIZE = 0x80; // In bytes
        static const int MEM_USER_SETTINGS = 0x0 * BLOCK_SIZE;
        static const int MEM_KEY_MATRIX    = 0x1 * BLOCK_SIZE;
        static const int MEM_KEY_CAL_MIN   = 0x2 * BLOCK_SIZE;
        static const int MEM_KEY_CAL_MAX   = 0x3 * BLOCK_SIZE;
        static const int MEM_KEYMAPS       = 0x4 * BLOCK_SIZE;

        // User settings memory offsets
        static const int MEM_SETTINGS_DEADZONE_OFFSET = 0;
        static const int MEM_SETTINGS_SENSITIVITY_OFFSET = 1;
        static const int MEM_SETTINGS_MIN_THRESHOLD_OFFSET = 2;
        static const int MEM_SETTINGS_MAX_THRESHOLD_OFFSET = 3;
        static const int MEM_SETTINGS_NOISE_FLOOR_OFFSET = 4;
        */

};

#endif
