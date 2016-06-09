#include "globals.h"

#include "RowController.h"
#include "ColumnController.h"

RowController* controllers::row = new RowController();

ColumnController* controllers::column = new ColumnController();

uint8_t state::layer = 1;

Key* state::keys[NUM_KEYS];
