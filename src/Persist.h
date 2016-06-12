#ifndef PERSIST_H
#define PERSIST_H

#include <cstdint>

/**
 * This class defines how EEPROM is allocated and provides accessor functions.
 * Dividing the 2048 byte EEPROM of the teensy into blocks of 128 bytes, there
 * are 16 blocks. These are allocated as follows:
 * 0 -> User setting
 * 1 -> Key electronic index (row and column)
 * 2 -> Key Cal Min
 * 3 -> Key Cal Max
 * 4 -> Keymap Layer 0
 * 5 -> Keymap Layer 1
 * 6 -> Keymap Layer 2
 * 7 -> Keymap Layer 3
 * 8 -> Keymap Layer 4
 * 9 -> Keymap Layer 5
 * A -> Keymap Layer 6
 * B -> Keymap Layer 7
 * C -> Not used
 * D -> Not used
 * E -> Not used
 * F -> Not used
 */
class Persist {

    public:

        /**
         * Check if a key is in this matrix.
         * @param keyID key ID
         * @return true if the key is in the matrix
         */
        static bool keyIsInMatrix(uint8_t keyID);

        /**
         * Get the row index of a given key.
         * @param keyID key ID
         * @return row index
         */
        static uint8_t getRow(uint8_t keyID);

        /**
         * Get the column index of a given key.
         * @param keyID key ID
         * @return column index
         */
        static uint8_t getCol(uint8_t keyID);

        /**
         * Set the matrix position (row and column) of a given key.
         * @param keyID key ID
         * @param row row index
         * @param col column index
         */
        static void setMatrixPosition(uint8_t keyID, uint8_t row, uint8_t column);

        /**
         * Get calibration minimum value for a key.
         * @param keyID key ID
         * @return calibration minimum value
         */
        static uint8_t getCalMin(uint8_t keyID);

        /**
         * Set calibration minimum value for a key.
         * @param keyID key ID
         * @param value minimum ADC reading
         */
        static void setCalMin(uint8_t keyID, uint8_t value);

        /**
         * Get calibration maximum value for a key.
         * @param keyID key ID
         * @return calibration maximum value
         */
        static uint8_t getCalMax(uint8_t keyID);

        /**
         * Set calibration maximum value for a key.
         * @param keyID key ID
         * @param value maximum ADC reading
         */
        static void setCalMax(uint8_t keyID, uint8_t value);

        /**
         * Get mapping for a key on a given layer.
         * @param keyID key ID
         * @param layer layer to find
         * @return mapping
         */
        static uint8_t getMapping(uint8_t keyID, uint8_t layer);

        /**
         * Set mapping for a key on a given layer.
         * @param keyID key ID
         * @param layer layer to set
         * @param mapID action to set
         */
        static void setMapping(uint8_t keyID, uint8_t layer, uint8_t mapID);

        static uint8_t getDeadZone();
        static void setDeadZone(uint8_t deadZone);

        static uint8_t getSensitivity();
        static void setSensitivity(uint8_t sensitivity);

    private:

        // Helper functions for input sanitization
        static uint8_t maskKeyID(uint8_t keyID);
        static uint8_t maskLayer(uint8_t maskLayer);

        // Memory locations
        static const int BLOCK_SIZE = 128; // In bytes
        static const int MEM_USER_SETTINGS_BLOCK = 0x0;
        static const int MEM_KEY_MATRIX_BLOCK = 0x1;
        static const int MEM_KEY_CAL_MIN_BLOCK = 0x2;
        static const int MEM_KEY_CAL_MAX_BLOCK = 0x3;
        static const int MEM_KEYMAP_START_BLOCK = 0x4;

        // User settings memory offsets
        static const int MEM_SETTINGS_DEADZONE_OFFSET = 0;
        static const int MEM_SETTINGS_SENSITIVITY_OFFSET = 1;

};

#endif
