#include "helpers.h"
#include "mbed.h"
/**
* This method is used to create a toggle button
* @author Christian Nick Rasmussen
* @param[in] pin The pin where the button is connected on the discovery board
* @date 2021/01/19
*/
ToggleButton::ToggleButton(PinName pin)
    : _interrupt(pin) // create the InterruptIn on the pin specified to Counter
{
  _interrupt.rise(
      callback(this, &ToggleButton::toggle)); // attach increment function of
                                              // this counter instance
}
/**
* This method is used to create a toggle effect when clicking a button
* @author Christian Nick Rasmussen
* @param[in] locale The location string
* @date 2021/01/19
*/
void ToggleButton::toggle() {
  _toggleBool = !_toggleBool;
  _interrupt.disable_irq();
  wait_us(250000);
  _interrupt.enable_irq();
}
/**
* This method is used to show input when entering the location from the keyboard using serial
* @author Christian Nick Rasmussen
* @param[out] _toggleBool Whether the toggle is true or false
* @date 2021/01/19
*/
bool ToggleButton::getToggle() { return _toggleBool; }
