/**
@file screen.cpp
*/
#include "mbed.h"
#include "screen.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include "button.h"
#include <cstdint>
#include <cstdio>
#include <string>
#define LCD_SMALL_FONT        Font16 //Small font constant
#define LCD_MEDIUM_FONT        Font20 //Medium font constant
#define LCD_XSMALL_FONT         Font12 //Extra small font constant

uint8_t status; ///< Becomes 1/true if the LCD and Touch Screen is initialized

/**
* This method is used to initialize the screen
* @author Christian Nick Rasmussen
* @date 2021/01/19
*/

void ScreenInit() {
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);
    status = BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
}

/**
* This method is used to draw the start screen
* @author Christian Nick Rasmussen
* @date 2021/01/19
*/
void StartScreen() {
    BSP_LCD_Clear(LCD_COLOR_DARKCYAN);
    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_DARKCYAN);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(0, 100, (uint8_t *)"Office Environment", CENTER_MODE);
    BSP_LCD_DrawEllipse(233, 110, 170, 60);
    BSP_LCD_SetFont(&LCD_SMALL_FONT);
    BSP_LCD_DisplayStringAt(0, 250, (uint8_t *)"Version 0.0.1", CENTER_MODE);
    HAL_Delay(1000);
    if (status != TS_OK) {
        BSP_LCD_SetTextColor(LCD_COLOR_RED);
        BSP_LCD_DisplayStringAt(0, 200, (uint8_t *)"TOUCHSCREEN INIT FAIL", CENTER_MODE);
    } else {
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        BSP_LCD_DisplayStringAt(0, 200, (uint8_t *)"TOUCHSCREEN INIT OK", CENTER_MODE);
    }
    HAL_Delay(1000);
}
/**
* This method is used to draw the input screen
* @author Christian Nick Rasmussen
* @date 2021/01/19
*/
void  InputScreen() {
    BSP_LCD_Clear(LCD_COLOR_BLACK);
    BSP_LCD_SetFont(&LCD_MEDIUM_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(0, 100, (uint8_t *)"Please enter office locale:", CENTER_MODE);
    BSP_LCD_DrawRect(100, 140, BSP_LCD_GetXSize()-200, 45);
}

/**
* This method is used to show input when entering the location from the keyboard using serial
* @author Christian Nick Rasmussen
* @param[in] locale The location string
* @date 2021/01/19
*/
void InputLocaleText(string locale) {
BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
BSP_LCD_SetBackColor(LCD_COLOR_BLACK);

BSP_LCD_DisplayStringAt(110, 152, (uint8_t *)locale.c_str(), LEFT_MODE);
}

/**
* This method is used to draw the Home icon in the home screen header
* @author Christian Nick Rasmussen
* @date 2021/01/19
*/

void DrawPixelHomeButton() {
    int homePixelArt[32][30] = {
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
{0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
{0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
{0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
{0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
{0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
{0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
{0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
{0,0,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,0,0},
{0,0,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,0,0},
{0,0,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,0,0},
{0,0,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,0,0},
{0,0,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,0,0},
{0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0},
{0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0},
{0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0},
{0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0},
{0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
{0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

int baseX = 15;
int baseY = 20;

    for (int i = 0; i < 32; i++){
        for (int j = 0; j < 28; j++){
            if(homePixelArt[i][j] == 1) {
                BSP_LCD_DrawPixel(baseX+j, baseY+i, LCD_COLOR_WHITE);
            }
        }
    }
}

/**
* This method is used to draw the home screen
* @author Christian Nick Rasmussen
* @param[in] locale The location string in the header
* @date 2021/01/19
*/

void HomeScreen(string locale) {
    BSP_LCD_Clear(LCD_COLOR_BLACK);
    BSP_LCD_SetFont(&LCD_MEDIUM_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_DARKCYAN);
    BSP_LCD_SetTextColor(LCD_COLOR_DARKCYAN);
    BSP_LCD_DrawRect (0,0, BSP_LCD_GetXSize()-2,BSP_LCD_GetYSize()-2);
    BSP_LCD_FillRect(0,0, BSP_LCD_GetXSize()-2,60);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(0, 5, (uint8_t *)"Office Environment", CENTER_MODE);
    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
    BSP_LCD_DisplayStringAt(0, 35, (uint8_t *)locale.c_str(), CENTER_MODE);
    DrawPixelHomeButton();
}

/**
* This method is used to indicate whether it is night or day
* @author Christian Nick Rasmussen
* @param[in] string String saying whether it is night or day
* @date 2021/01/19
*/
void NightOrDaytText(string string) {
    BSP_LCD_SetFont(&LCD_MEDIUM_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(0, 80, (uint8_t *)string.c_str(), CENTER_MODE);
}

/**
* This method is used to show the temperature on screen
* @author Christian Nick Rasmussen
* @param[in] tempString Character array with the temperature
* @date 2021/01/19
*/
void TemperatureString(uint8_t tempString[25]){
    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(60, 130, (uint8_t *)tempString, LEFT_MODE);
};
/**
* This method is used to show the status of the temperature. If too high, aircontion is on. Too low heater is on
* @author Christian Nick Rasmussen
* @param[in] string Status string
* @param[in] color  Color of the string
* @param[in] x      X position of the string on the display
* @date 2021/01/19
*/

void TempControllerText(string string, uint32_t color,uint16_t x) {
    BSP_LCD_SetFont(&LCD_MEDIUM_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_FillRect(1, 160, BSP_LCD_GetXSize()/2-1, 30);
    BSP_LCD_SetTextColor(color);
    BSP_LCD_DisplayStringAt(x, 160, (uint8_t *)string.c_str(), LEFT_MODE);
}
/**
* This method is used to show the status of the sound. If too high a warning will show
* @author Christian Nick Rasmussen
* @param[in] string Status string
* @param[in] color  Color of the string
* @param[in] x      X position of the string on the display
* @date 2021/01/19
*/
void SoundControllerText(string string, uint32_t color, uint16_t x) {
    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_FillRect(BSP_LCD_GetXSize()/2-10, 130, BSP_LCD_GetXSize()/2-10, 30);
    BSP_LCD_SetTextColor(color);
    BSP_LCD_DisplayStringAt(x, 130, (uint8_t *)string.c_str(), RIGHT_MODE);
}
/**
* This method is used to show how many times there has been a noise warning
* @author Christian Nick Rasmussen
* @param[in] number Number of warnings
* @param[in] x      X position of the string on the display
* @date 2021/01/19
*/
void SoundControllerString(int number,uint16_t x) {
    BSP_LCD_SetFont(&LCD_MEDIUM_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    uint8_t text[10] = {0};
    sprintf((char *) text, "Warnings:%2d", number);
    BSP_LCD_DisplayStringAt(x, 160, (uint8_t *)text, RIGHT_MODE);
}
/**
* This method is used to clear the main part of the screen
* @author Christian Nick Rasmussen
* @date 2021/01/19
*/
void clearMain() {
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_FillRect(1,61, BSP_LCD_GetXSize()-3,BSP_LCD_GetYSize()-62);
}
/**
* This method is used to draw the outline of the sound graph
* @author Christian Nick Rasmussen
* @date 2021/01/19
*/
void SoundGraphOutline() {

    BSP_LCD_SetFont(&LCD_XSMALL_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DrawHLine(40, BSP_LCD_GetYSize()-7, 400);
    BSP_LCD_DrawVLine(39, 65, 201);
    BSP_LCD_DrawVLine(BSP_LCD_GetXSize()-39, 65, 201);
    BSP_LCD_DisplayStringAt(20, 261, (uint8_t *)"0%", LEFT_MODE);
    BSP_LCD_DisplayStringAt(15, 161, (uint8_t *)"50%", LEFT_MODE);
    BSP_LCD_DisplayStringAt(10, 65, (uint8_t *)"100%", LEFT_MODE);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DrawHLine(40, 95, 400);
}
/**
* This method is used to draw the outline of the temperature graph
* @author Christian Nick Rasmussen
* @date 2021/01/19
*/
void TempGraphOutline() {

    BSP_LCD_SetFont(&LCD_XSMALL_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DrawHLine(40, BSP_LCD_GetYSize()-7, 400);
    BSP_LCD_DrawVLine(39, 65, 201);
    BSP_LCD_DrawVLine(BSP_LCD_GetXSize()-39, 65, 201);
    BSP_LCD_DisplayStringAt(15, 261, (uint8_t *)"20C", LEFT_MODE);
    BSP_LCD_DisplayStringAt(15, 161, (uint8_t *)"25C", LEFT_MODE);
    BSP_LCD_DisplayStringAt(15, 65, (uint8_t *)"30C", LEFT_MODE);

}
/**
* This method is used to draw one step of the sound graph
* @author Christian Nick Rasmussen
* @param[in] yValueOld  Percent value of first part of the step line to calculate the spot on the Y-axis
* @param[in] xValueOld  Int value to calculate the spot on the X-axis
* @param[in] yValueNew  Percent value of second part of the step line to calculate the spot on the Y-axis
* @param[in] xValueNew  Int value to calculate the spot on the X-axis
* @param[in] string     Draws string showing average sound level of the last 1000 measurements
* @date 2021/01/19
*/
void DrawSoundGraph(int yValueOld,int xValueOld,int yValueNew,int xValueNew,string string) {
    int xBoost = 40;
    int yBoost = 65;
    int xInc = 4;

    int x1 = (xValueOld*xInc)+xBoost;
    int x2 = (xValueNew*xInc)+xBoost;
    int y1 = yBoost+(200-yValueOld*2);
    int y2 = yBoost+(200-yValueNew*2);
    BSP_LCD_SetBackColor(LCD_COLOR_TRANSPARENT);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_FillRect(x2,65, 4,200);
    BSP_LCD_SetFont(&LCD_XSMALL_FONT);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(0, 250, (uint8_t *)string.c_str(), CENTER_MODE);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DrawHLine(40, 95, 400);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DrawLine(x1, y1, x2, y2);

}
/**
* This method is used to draw one step of the temperature graph
* @author Christian Nick Rasmussen
* @param[in] yValueOld  Percent value of first part of the step line to calculate the spot on the Y-axis
* @param[in] xValueOld  Int value to calculate the spot on the X-axis
* @param[in] yValueNew  Percent value of second part of the step line to calculate the spot on the Y-axis
* @param[in] xValueNew  Int value to calculate the spot on the X-axis
* @param[in] string     Draws string showing average temperature of the last 1000 measurements
* @date 2021/01/19
*/
void DrawTempGraph(int yValueOld,int xValueOld,int yValueNew,int xValueNew,string string) {
    int xBoost = 40;
    int yBoost = 65;
    int xInc = 4;

    int x1 = (xValueOld*xInc)+xBoost;
    int x2 = (xValueNew*xInc)+xBoost;
    int y1 = yBoost+(200-yValueOld*2);
    int y2 = yBoost+(200-yValueNew*2);
    BSP_LCD_SetBackColor(LCD_COLOR_TRANSPARENT);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_FillRect(x2,65, 4,200);
    BSP_LCD_SetFont(&LCD_XSMALL_FONT);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(0, 250, (uint8_t *)string.c_str(), CENTER_MODE);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DrawHLine(40, 145, 400);
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
    BSP_LCD_DrawHLine(40, 165, 400);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DrawLine(x1, y1, x2, y2);
}