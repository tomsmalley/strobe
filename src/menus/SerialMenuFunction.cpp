#include "SerialMenuFunction.h"

// Constructor to set up required variables
SerialMenuFunction::SerialMenuFunction(const char c, const char* n,
        voidFunction f)
        : command(c), name(n), function(f) {}

const char* SerialMenuFunction::getName() const {
    return name;
}

const char SerialMenuFunction::getCommand() const {
    return command;
}

// Just call the function
void SerialMenuFunction::execute() const {
    function();
}
