#ifndef SERIALMENUFUNCTION_H
#define SERIALMENUFUNCTION_H

// Function pointer typedef, only consider void (void) functions
typedef void (*voidFunction)(void);

/**
 * Represents a function which has some char identity and a string name.
 */
class SerialMenuFunction {

    public:

        /**
         * Constructor just sets the identity, name, and function.
         * The function has to return void with no arguments. Pass in the
         * reference.
         * @param c char identity
         * @param n string name
         * @param f void function to call (pass reference)
         */
        SerialMenuFunction(const char c, const char* n, const voidFunction f);

        /**
         * Get the char identity
         * @return char identity
         */
        const char getCommand() const;

        /**
         * Get the function name as string
         * @return function name
         */
        const char* getName() const;

        /**
         * Call this to call the set function
         */
        void execute() const;

    private:

        // Char identity
        const char command;
        // String name
        const char* name;
        // Function to call
        const voidFunction function;

};

#endif
