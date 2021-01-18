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

uint16_t x, y;
uint8_t idx;
uint8_t cleared = 0;
uint8_t prev_nb_touches = 0;

uint8_t celciusString[25] = {0};
uint8_t fahrenheitString[25] = {0};
bool celciusOrFahrenheit;
char buffer[32] = {0};
string officeLocale = "";
int alarmInt = 2;

enum Screen { home, sound };
bool graphOutlineDrawn = false;
Screen currentScreen = home;
Thread thread2;

int soundValues[100];
int soundValuesArrayCounter = 0;


// Decides if show temp in fahrenheit or celcius
void tempUnit() {
  sprintf((char *)celciusString, "%4.2fC", temperature);
  sprintf((char *)fahrenheitString, "%4.2fF", temperature * 1.8 + 32);
  if (celciusOrFahrenheit) {
    TemperatureString(celciusString);
  } else {
    TemperatureString(fahrenheitString);
  }
}

void NightOrDay(float light) {
  if (light > 0.150) {
    NightOrDaytText("Daily Routine");
  } else {
    NightOrDaytText("Night Routine");
  }
}

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

void SoundController(float noise, float light) {
  if (noise > 0.9) {
    SoundControllerText("NOISE WARNING", LCD_COLOR_RED, 20);

    warningCounter += 0.25;
  } else {
    SoundControllerText("Noise OK", LCD_COLOR_WHITE, 60);
  }
  SoundControllerString(warningCounter, 50);
}

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

void addSoundLevelToArray(float sound, int counter) {
  int index = counter % 100;
  int soundInPercent = sound * 100;
  soundValues[index] = soundInPercent;
}

void drawCompleteGraph(int allSsoundValues[100], int counter) {
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
  DrawGraph(soundValues[max(index - 1, 0)],max(index - 1, 0),soundValues[index], index,averageSound);
}

// Buttons
LCD_Button resetButton(280, 160, 160, 30, LCD_COLOR_BLACK, "");
LCD_Button alarmButton(280, 130, 160, 30, LCD_COLOR_BLACK, "");
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
  toSoundScreen.draw(false);
  toTempScreen.draw(false);
  toHomeScreen.draw(false);
  thread2.start(&valueReply);

  while (true) {
    temperature = tempSensor.getTemperature();
    soundLevel = soundSensor.read();
    lightLevel = lightSensor.read();
    addSoundLevelToArray(soundLevel, soundValuesArrayCounter);
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
        toSoundScreen.draw(false);
        toTempScreen.draw(false);
    }
      NightOrDay(lightLevel);
      TempController(temperature);
      SoundController(soundLevel, lightLevel);
      graphOutlineDrawn = false;
      break;
    case sound:
      if (!graphOutlineDrawn) {
        clearMain();
        GraphOutline();
      }
      drawCompleteGraph(soundValues,soundValuesArrayCounter);
      /*for (int x : soundValues) {
          printf("%d - ",x);
          DrawGraph(int yValueOld,int xValueOld,int yValueNew,int xValueNew)
      }
      for (int i = 0; i < sizeof(soundValues); i++) {
          DrawGraph(soundValues[i],i,soundValues[min(i+1,99)],i+1);
      }
      */

      graphOutlineDrawn = true;
      break;
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
    // printf("Array: %3d %3d %3d %3d %3d
    // \n",soundValues[0],soundValues[1],soundValues[2],soundValues[3],soundValues[4]);

    wait_us(250000);
  }
}
