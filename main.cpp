#include "Grove_temperature.h"
#include "mbed.h"
#include "screen.h"
#include "helpers.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include <cstdio>
#include <string>

AnalogIn ain(A0);
AnalogIn lightSensor(A2);
AnalogIn soundSensor(A3);
Grove_temperature tempSensor(A1);
DigitalOut myLedD2(D2);
DigitalOut myLedD3(D3);
InterruptIn exButton(D4);
BufferedSerial pc(USBTX, USBRX);


float temperature;
float soundLevel;
float lightLevel;

uint8_t celciusString[25] = {0};
uint8_t fahrenheitString[25] = {0};
bool celciusOrFahrenheit;
char buffer[32] = {0};
string officeLocale = "";

// Decides if show temp in fahrenheit or celcius
void tempUnit() {
        sprintf((char *) celciusString,"%4.2fC",temperature);
        sprintf((char *) fahrenheitString,"%4.2fF",temperature*1.8+32);
    if(celciusOrFahrenheit) {
        TemperatureString(celciusString);
    } else {
        TemperatureString(fahrenheitString);
    }
}



// main() runs in its own thread in the OS
int main() {
  pc.set_baud(9600);
  pc.set_format(
      /* bits */ 8,
      /* parity */ BufferedSerial::None,
      /* stop bit */ 1);
  ToggleButton toggleButton(D4);
  
  
  
  ScreenInit();
  StartScreen();
  InputScreen();

  while (strcmp(buffer, "\r") != 0) {
    if (uint32_t num = pc.read(buffer, sizeof(buffer))) {

      // pc.write(buffer,sizeof(buffer));
      if (strcmp(buffer, "\r") == 0) {
        // nothing
      } else if (strcmp(buffer, "\b\b") == 0) {
        officeLocale.pop_back();
      } else {
        officeLocale.push_back(buffer[0]);
      }
    }
    printf("%s", (uint8_t *)buffer);
    InputLocaleText(officeLocale);
    printf("\n%s\n", officeLocale.c_str());
  }

  HomeScreen(officeLocale);
  while (true) {
    temperature = tempSensor.getTemperature();
    soundLevel = soundSensor.read();
    lightLevel = lightSensor.read();
    // printf("percentage: %3.3f%%\n", ain.read() * 100.0f);
    printf("Temp: %3.3f\n", temperature);
    // printf("Sound: %3.3f\n", soundLevel);
    /*        printf("Light: %3.3f\n", lightLevel);
            if (uint32_t num = pc.read(buffer, sizeof(buffer))) {
                //pc.write(buffer,sizeof(buffer));
                officeLocale.push_back(buffer[0]);
                if(strcmp(buffer,"\r") == 0) {
                    printf("const char *__restrict, ...");
                }
            }
            */
    //printf("\n%s\n", officeLocale.c_str());
    celciusOrFahrenheit = toggleButton.getToggle();
    tempUnit();
    wait_us(500000);
  }
}
