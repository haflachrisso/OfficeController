#ifndef BUTTON_H
#define BUTTON_H


#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"


class LCD_Button {
    public: 
    LCD_Button(int x, int y, int w, int h,uint32_t borderColor) 
    {
        _x = x;
        _y = y; 
        _w = w;
        _h = h;
        _borderColor = borderColor;
        _fillColor = borderColor;

    }

    void draw(bool active) {
        BSP_LCD_SetTextColor(_borderColor);
        BSP_LCD_DrawRect(_x,_y, _w,_h);
        if(active) {
            BSP_LCD_FillRect(_x,_y, _w,_h);
        }
    };

    private:
    int _x;
    int _y; 
    int _w;
    int _h;
    uint32_t _borderColor;
    uint32_t _fillColor;


};
#endif