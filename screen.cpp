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

uint8_t status;

void ScreenInit() {
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);
    status = BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
}

void StartScreen() {
    BSP_LCD_Clear(LCD_COLOR_DARKCYAN);
    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_DARKCYAN);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(0, 100, (uint8_t *)"Office Environment", CENTER_MODE);
    BSP_LCD_DrawEllipse(233, 110, 170, 65);
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

void  InputScreen() {
    BSP_LCD_Clear(LCD_COLOR_BLACK);
    BSP_LCD_SetFont(&LCD_MEDIUM_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(0, 100, (uint8_t *)"Please enter office locale:", CENTER_MODE);
    BSP_LCD_DrawRect(100, 140, BSP_LCD_GetXSize()-200, 45);
}

void InputLocaleText(string locale) {
BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
BSP_LCD_SetBackColor(LCD_COLOR_BLACK);

BSP_LCD_DisplayStringAt(110, 152, (uint8_t *)locale.c_str(), LEFT_MODE);
}

void HomeScreen(string locale) {
    BSP_LCD_Clear(LCD_COLOR_BLACK);
    BSP_LCD_SetFont(&LCD_MEDIUM_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_DARKCYAN);
    BSP_LCD_SetTextColor(LCD_COLOR_DARKCYAN);
    BSP_LCD_DrawRect (0,0, BSP_LCD_GetXSize()-2,BSP_LCD_GetYSize()-2);
    BSP_LCD_FillRect(0,0, BSP_LCD_GetXSize()-2,70);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(0, 5, (uint8_t *)"Office Environment", CENTER_MODE);
    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
    BSP_LCD_DisplayStringAt(0, 35, (uint8_t *)locale.c_str(), CENTER_MODE);

    //updateValuesOnLCD();
}

void NightOrDaytText(string string) {
    BSP_LCD_SetFont(&LCD_MEDIUM_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(0, 80, (uint8_t *)string.c_str(), CENTER_MODE);
}


void TemperatureString(uint8_t tempString[25]){
    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(60, 130, (uint8_t *)tempString, LEFT_MODE);
};


void TempControllerText(string string, uint32_t color,uint16_t x) {
    BSP_LCD_SetFont(&LCD_MEDIUM_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_FillRect(1, 160, BSP_LCD_GetXSize()/2-1, 30);
    BSP_LCD_SetTextColor(color);
    BSP_LCD_DisplayStringAt(x, 160, (uint8_t *)string.c_str(), LEFT_MODE);
}

void SoundControllerText(string string, uint32_t color, uint16_t x) {
    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_FillRect(BSP_LCD_GetXSize()/2-10, 130, BSP_LCD_GetXSize()/2-10, 30);
    BSP_LCD_SetTextColor(color);
    BSP_LCD_DisplayStringAt(x, 130, (uint8_t *)string.c_str(), RIGHT_MODE);
}

void SoundControllerString(int number,uint16_t x) {
    BSP_LCD_SetFont(&LCD_MEDIUM_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    uint8_t text[10] = {0};
    sprintf((char *) text, "Warnings:%2d", number);
    BSP_LCD_DisplayStringAt(x, 160, (uint8_t *)text, RIGHT_MODE);
}