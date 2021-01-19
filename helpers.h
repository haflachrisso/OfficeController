/**
* @file helpers.h
* @brief This header file contains all helper functions
*
* @author Christian Nick Rasmussen
*
* @date 2021/01/19
*/

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