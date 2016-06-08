#ifndef SERIALMENUFUNCTION_H
#define SERIALMENUFUNCTION_H

// Function pointer typedef
typedef void (*voidFunction)(void);

// SerialMenuFunction.h
class SerialMenuFunction {

    public:
        SerialMenuFunction(const char c, const char* n, voidFunction f);
        const char getCommand();
        const char* getName();
        void execute();
    private:
        const char command;
        const char* name;
        voidFunction function;

};

#endif
