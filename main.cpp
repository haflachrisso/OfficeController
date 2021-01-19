/**
* @file main.cpp
*/

#include "Grove_temperature.h"
#include "helpers.h"
#include "mbed.h"
#include "screen.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <string>

AnalogIn ain(A0);
AnalogIn lightSensor(A2);
AnalogIn soundSensor(A3);
TS_StateTypeDef TS_State; // Based on MBED Template

Grove_temperature tempSensor(A1);

DigitalOut RedLed(D2);
DigitalOut GreenLed(D3);
DigitalOut BlueLED(D5);

InterruptIn exButton(D4);
BufferedSerial pc(USBTX, USBRX);

float temperature;
float soundLevel;
float lightLevel;
float warningCounter = 0;

uint16_t x, y; ///< Xpos and Ypos of press on touch screen
uint8_t idx;
uint8_t cleared = 0;
uint8_t prev_nb_touches = 0;

uint8_t celciusString[25] = {0};
uint8_t fahrenheitString[25] = {0};
bool celciusOrFahrenheit;

char buffer[32] = {0}; ///< Serial Input Buffer
string officeLocale = "";
int alarmInt = 2;

enum Screen { home, sound, temp }; ///< Enum for which screen to show on the LCD
bool graphOutlineDrawn = false;
Screen currentScreen = home;
Thread thread2; ///< Creates a second thread for not blocking serial input

int soundValues[100];
int soundValuesArrayCounter = 0;

float tempFloatValues[100];
int tempValues[100];
int tempValuesArrayCounter = 0;

/**
* This method is used to decide whether to show temperature in Celcius or Fahrenheit
* @author Christian Nick Rasmussen
* @date 2021/01/19
*/
void tempUnit() {
  sprintf((char *)celciusString, "%4.2fC", temperature);
  sprintf((char *)fahrenheitString, "%4.2fF", temperature * 1.8 + 32);
  if (celciusOrFahrenheit) {
    TemperatureString(celciusString);
  } else {
    TemperatureString(fahrenheitString);
  }
}
/**
* This method is used to determined if it is dark or light in the room
* @author Christian Nick Rasmussen
* @param[in] light Light level from sensor
* @date 2021/01/19
*/
void NightOrDay(float light) {
  if (light > 0.150) {
    NightOrDaytText("Daily Routine");
  } else {
    NightOrDaytText("Night Routine");
  }
}
/**
* This method controls if the temperature is okay. If not "turns on" the aircondition or heater
* @author Christian Nick Rasmussen
* @param[in] temp Temperature in celcius
* @date 2021/01/19
*/
void TempController(float temp) {
  tempUnit();
  if (temp < 25) {
    TempControllerText("HEATER ON", LCD_COLOR_DARKRED, 50);
    BlueLED = 0;
    RedLed = 1;
  } else if (temp > 26) {
    BlueLED = 1;
    RedLed = 0;
    TempControllerText("AIRCONDITION On", LCD_COLOR_DARKBLUE, 20);
  } else {
    BlueLED = 0;
    RedLed = 0;
    TempControllerText("Idle", LCD_COLOR_WHITE, 80);
  }
}
/**
* This method controls if the sound is okay. If not and it is dark, an alarm goes off
* @author Christian Nick Rasmussen
* @param[in] noise Sound level from sensor
* @param[in] light Light level from sensor
* @date 2021/01/19
*/
void SoundController(float noise, float light) {
  if (noise > 0.9) {
    SoundControllerText("NOISE WARNING", LCD_COLOR_RED, 20);

    warningCounter += 0.25;
  } else {
    SoundControllerText("Noise OK", LCD_COLOR_WHITE, 60);
  }
  SoundControllerString(warningCounter, 50);
}
/**
* This method is run on Thread2 and checks for serial keyboard input and print the chosen value to the console
* @author Christian Nick Rasmussen
* @date 2021/01/19
*/
void valueReply() {
  while (true) {
    pc.set_blocking(false);
    if (uint32_t num = pc.read(buffer, sizeof(buffer))) {
      if (strcmp(buffer, "1") == 0) {
        printf("%s:Temp: %3.3f\n", officeLocale.c_str(), temperature);
      } else if (strcmp(buffer, "2") == 0) {
        printf("%s: Sound: %3.3f\n", officeLocale.c_str(), soundLevel);
      } else if (strcmp(buffer, "3") == 0) {
        printf("%s: Light: %3.3f\n", officeLocale.c_str(), lightLevel);
      }
    }
    wait_us(1000000);
  }
}
/**
* This method adds values to the soundValues array and overrides from the begining once the array is "full"
* @author Christian Nick Rasmussen
* @param[in] sound Sound level from sensor
* @param[in] counter A counter running through the index og the sound level array
* @date 2021/01/19
*/
void addSoundLevelToArray(float sound, int counter) {
  int index = counter % 100;
  int soundInPercent = sound * 100;
  soundValues[index] = soundInPercent;
}

/**
* This method changes temperature in celcius to an int from 1-100, to use the values as Y on the temp graph
* @author Christian Nick Rasmussen
* @param[in] temp Temperature in celcius
* @date 2021/01/19
*/

int tempToPercent(float temp) {
    float tempPercent;
    if(temp > 30.00) {
        tempPercent = 100;
    } else if (temp < 20.00) {
        tempPercent = 0;
    } else {
       tempPercent = (temp-20.00)*10;
    }
    
    return (int)tempPercent;
}
/**
* This method adds values to the tempValues and tempFloatValues array and overrides from the begining once the array is "full"
* @author Christian Nick Rasmussen
* @param[in] temp Temperature in celcius
* @param[in] counter A counter running through the index og the temperature arrays
* @date 2021/01/19
*/
void addTemperatureToArray(float temp, int counter) {
    int index = counter % 100;
    tempFloatValues[index] = temp;

    tempValues[index] = tempToPercent(temp);
}
/**
* This method gets the current and former sound level in the soundValues array and calculates average sound level of the last 100 measurements and uses these values to call the DrawSoundGraph
* @author Christian Nick Rasmussen
* @param[in] counter A counter running through the index og the sound level array
* @date 2021/01/19
*/
void drawCompleteSoundGraph(int counter) {
  int index = counter % 100;
  int soundTotal = 0;
  int soundsAboveZero = 0;
  string averageSound = "Avg. Sound Lvl: ";
  for(int x: soundValues) {
      if(x > 0) {
          soundTotal+=x;
          soundsAboveZero++;
      }
  }
  string avrString = to_string(soundTotal/soundsAboveZero);
  averageSound+=avrString+"%";
  DrawSoundGraph(soundValues[max(index - 1, 0)],max(index - 1, 0),soundValues[index], index,averageSound);
}
/**
* This method gets the current and former temperature in the tempValues array and calculates average temperature of the last 100 measurements and uses these values to call the DrawTempGraph
* @author Christian Nick Rasmussen
* @param[in] counter A counter running through the index og the temperature array
* @date 2021/01/19
*/
void drawCompleteTempGraph(int counter) {
  int index = counter % 100;
  float tempTotal = 0;
  int tempsAboveZero = 0;
  string averageTemp = "Avg. Temperature: ";
  for(float x:  tempFloatValues) {
      if(x > 0) {
          tempTotal+=x;
          tempsAboveZero++;
      }
  }
  string avrString = to_string(tempTotal/tempsAboveZero).substr(0,5);

  averageTemp+=avrString+"C";
  DrawTempGraph(tempValues[max(index - 1, 0)],max(index - 1, 0),tempValues[index], index,averageTemp);
}

// Buttons
LCD_Button resetButton(280, 160, 200, 30, LCD_COLOR_BLACK, "");
LCD_Button alarmButton(280, 130, 200, 30, LCD_COLOR_BLACK, "");
LCD_Button toSoundScreen(260, 200, 200, 60, LCD_COLOR_DARKCYAN, "Sound Graph");
LCD_Button toTempScreen(20, 200, 200, 60, LCD_COLOR_DARKCYAN, "Temp Graph");
LCD_Button toHomeScreen(10,15,37,32,LCD_COLOR_DARKCYAN, "");

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
      } else if (strcmp(buffer, "\b\b") == 0) { // No working
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
  toSoundScreen.draw();
  toTempScreen.draw();
  //toHomeScreen.draw(false);
  thread2.start(&valueReply);

  while (true) {
    temperature = tempSensor.getTemperature();
    soundLevel = soundSensor.read();
    lightLevel = lightSensor.read();
    addSoundLevelToArray(soundLevel, soundValuesArrayCounter);
    addTemperatureToArray(temperature,tempValuesArrayCounter);
    // printf("percentage: %3.3f%%\n", ain.read() * 100.0f);
    // printf("Temp: %3.3f\n", temperature);
    // printf("Sound: %3.3f\n", soundLevel);
    // printf("Light: %3.3f\n", lightLevel);

    // Touch screen operations
    BSP_TS_GetState(&TS_State);
    if (TS_State.touchDetected) {
      for (idx = 0; idx < TS_State.touchDetected; idx++) {
        x = TS_State.touchX[idx];
        y = TS_State.touchY[idx];
        printf("Touch %d: x=%d y=%d    \n", idx + 1, x, y);
      }
      if (resetButton.pressed(x, y)) {
        warningCounter = 0;
      }

      if (alarmButton.pressed(x, y)) {
        alarmInt++;
        printf("AlarmPress");
      }

      if (toSoundScreen.pressed(x, y)) {
        currentScreen = sound;
        printf("ToSound");
      }

      if(toTempScreen.pressed(x, y)) {
          currentScreen = temp;
      }

      if (toHomeScreen.pressed(x,y)) {
          currentScreen = home;
          printf("ToHome");
      }
    } else {
      if (!cleared) {
        printf("Touches: 0\n");
        cleared = 1;
      }
    }

    // printf("\n%s\n", officeLocale.c_str());
    celciusOrFahrenheit = toggleButton.getToggle();
    // tempUnit();
    switch (currentScreen) {
    case home:
    if (graphOutlineDrawn) {
        clearMain();
        toSoundScreen.draw();
        toTempScreen.draw();
    }
      NightOrDay(lightLevel);
      TempController(temperature);
      SoundController(soundLevel, lightLevel);
      graphOutlineDrawn = false;
      break;
    case sound:
      if (!graphOutlineDrawn) {
        clearMain();
        SoundGraphOutline();
      }
      drawCompleteSoundGraph(soundValuesArrayCounter);
      graphOutlineDrawn = true;
      break;
    
    case temp:
        if (!graphOutlineDrawn) {
            clearMain();
            TempGraphOutline();
        }
        drawCompleteTempGraph(tempValuesArrayCounter);
        graphOutlineDrawn = true;

    }


    // Starts alarm if dark and loud noises
    if (lightLevel < 0.150 && soundLevel > 0.900) {
      alarmInt = 0;
    }
    if (alarmInt != 2) {
      GreenLed = !GreenLed;
    } else {
      GreenLed = 0;
    }

    soundValuesArrayCounter++;
    tempValuesArrayCounter++;
    // printf("Array: %3d %3d %3d %3d %3d
    // \n",soundValues[0],soundValues[1],soundValues[2],soundValues[3],soundValues[4]);

    wait_us(250000);
  }
}
