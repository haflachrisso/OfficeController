#include "mbed.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include "screen.h"
#include "Grove_temperature.h"


AnalogIn ain(A0);
AnalogIn lightSensor(A2);
AnalogIn soundSensor(A3);
Grove_temperature tempSensor(A1);
DigitalOut myLedD2(D2);
DigitalOut myLedD3(D3);
InterruptIn exButton(D4);


float temperature;
float soundLevel;
float lightLevel;

// main() runs in its own thread in the OS
int main()
{
    ScreenInit();
    while (true) {
        temperature =  tempSensor.getTemperature();
        soundLevel = soundSensor.read();
        lightLevel = lightSensor.read();
        //printf("percentage: %3.3f%%\n", ain.read() * 100.0f);
        printf("Temp: %3.3f\n", temperature);
        printf("Sound: %3.3f\n", soundLevel);
        printf("Light: %3.3f\n", lightLevel);

        wait_us(1000000);
    }
}

