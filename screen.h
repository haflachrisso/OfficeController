#ifndef BUTTON_H
#define BUTTON_H


#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include "button.h"


void ScreenInit() {
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);
}


#endif