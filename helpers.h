#ifndef HELPERS_H
#define HELPERS_H

#include "mbed.h"

class ToggleButton{
    public:
    ToggleButton(PinName pin);                    // create the InterruptIn on the pin specified to Counter


    void toggle();

    bool getToggle();

    private:
    InterruptIn _interrupt;
    bool _toggleBool = true;
};

#endif