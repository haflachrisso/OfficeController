#ifndef BUTTON_H
#define BUTTON_H

#include "mbed.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include <string>


class LCD_Button {
    public: 
    LCD_Button(int x, int y, int w, int h,uint32_t borderColor, string label) 
    {
        _x = x;
        _y = y; 
        _w = w;
        _h = h;
        _borderColor = borderColor;
        _fillColor = borderColor;
        _label = label;

    }

    void draw(bool active) {
        BSP_LCD_SetTextColor(_borderColor);
        BSP_LCD_DrawRect(_x,_y, _w,_h);
        if(active) {
            BSP_LCD_FillRect(_x,_y, _w,_h);
        }
    };

    bool pressed(int touchX,int touchY) {
        bool pressed = false;
        if((touchX >= _x && touchX <= _x+_w) && (touchY >= _y && touchY <= _y+_h)) {
            pressed = true;
        }
        return pressed;
    }

    private:
    int _x;
    int _y; 
    int _w;
    int _h;
    uint32_t _borderColor;
    uint32_t _fillColor;
    string _label;


};
#endif