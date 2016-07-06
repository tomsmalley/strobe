#ifndef ACTIONS_H
#define ACTIONS_H

#include <cstdint>

#if 0
#define SET     0
#define UNSET   1
#define TOGGLE  2
#endif

enum class Operation : uint8_t { SET = 0, UNSET = 1, TOGGLE = 2 };

#if 0
#define ROUTE_ANALOG    0
#define ROUTE_MOMENTARY 1
#define ROUTE_TOGGLE    2
#define ROUTE_SPECIAL   3
#endif

enum class Route : uint8_t { ANALOG = 0, MOMENTARY = 1, TOGGLE = 2, SPECIAL = 3 };

#endif
