#include "helpers.h"
#include "mbed.h"

ToggleButton::ToggleButton(PinName pin)
    : _interrupt(pin) // create the InterruptIn on the pin specified to Counter
{
  _interrupt.rise(
      callback(this, &ToggleButton::toggle)); // attach increment function of
                                              // this counter instance
}

void ToggleButton::toggle() {
  _toggleBool = !_toggleBool;
  _interrupt.disable_irq();
  wait_us(250000);
  _interrupt.enable_irq();
}

bool ToggleButton::getToggle() { return _toggleBool; }
