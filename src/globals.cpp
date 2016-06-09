#include "globals.h"

#include "RowController.h"
#include "ColumnController.h"

RowController* controllers::row = new RowController();

ColumnController* controllers::column = new ColumnController();

bool state::fnPressed = false;

uint8_t state::layer = 1;

Key* state::keys[NUM_KEYS];
