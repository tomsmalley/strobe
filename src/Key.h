#ifndef __KEY_H__
#define __KEY_H__

#include <cstdint>

class RowController;
class ColumnController;

/**
 * This class defines a keyswitch which is on a certain row and column. It
 * keeps track of state in RAM, and has accessors to EEPROM memory for row,
 * column, and calibration data. The EEPROM will store keymaps soon. Key state
 * can be read using strobeRead.
 */
class Key {

    public:

        Key();

        static uint8_t strobeRead(int8_t keyID, RowController* row,
                ColumnController* col);
        static uint8_t normalise(int8_t keyID, uint8_t val);

        static bool isInMatrix(int8_t keyID);

        static int8_t getRow(int8_t keyID);
        static void setRow(int8_t keyID, int8_t row);

        static int8_t getCol(int8_t keyID);
        static void setCol(int8_t keyID, int8_t column);

        static uint8_t getCalMin(int8_t keyID);
        static void setCalMin(int8_t keyID, uint8_t value);

        static uint8_t getCalMax(int8_t keyID);
        static void setCalMax(int8_t keyID, uint8_t value);

        static uint8_t getMapping(int8_t keyID, uint8_t layer);

        static void setMapping(int8_t keyID, uint8_t layer, uint8_t mapping);

        // Probably should make these private
        uint8_t depth;
        bool pressed;

    private:

        // How many bytes of EEPROM does a key use?
        static const int EEPROM_KEY_SIZE;
        static const int MEM_ROW_OFFSET;
        static const int MEM_COL_OFFSET;
        static const int MEM_CAL_MIN_OFFSET;
        static const int MEM_CAL_MAX_OFFSET;
        static const int MEM_BASE_LAYER_OFFSET;

        static int getKeyAddress(int8_t keyID);

};

#endif
