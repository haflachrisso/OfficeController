#ifndef SCREEN_H
#define SCREEN_H


#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include "button.h"
#include <cstdio>
#include <string>

// Initializes the LCD
void ScreenInit();

//Draws Start Screen
void StartScreen();
//Draws Input Screen
void InputScreen();

//Updates Screen Text on Input
void InputLocaleText(std::string locale);

//Draws Homescreen
void HomeScreen(std::string locale);

//Shows temperature and updates
void TemperatureString(uint8_t tempString[25]);

//Night or day text
void NightOrDaytText(std::string string);

//Temp controller text
void TempControllerText(std::string string, uint32_t color,uint16_t x);

//Sound controller text
void SoundControllerText(std::string string, uint32_t color,uint16_t x);

void SoundControllerString(int number,uint16_t x);

#endif