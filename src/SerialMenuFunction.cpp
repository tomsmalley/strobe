#include "SerialMenuFunction.h"

SerialMenuFunction::SerialMenuFunction(const char c, const char* n,
        voidFunction f)
        : command(c), name(n), function(f) {}

const char* SerialMenuFunction::getName() {
    return name;
}

const char SerialMenuFunction::getCommand() {
    return command;
}

void SerialMenuFunction::execute() {
    function();
}
