/**
* @file button.h
* @brief This header file contains the LCD_Button class
*
* @author Christian Nick Rasmussen
*
* @date 2021/01/19
*/

#ifndef BUTTON_H
#define BUTTON_H

#include "mbed.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include <cstdio>
#include <string>

#define LCD_SMALL_FONT        Font16 //Small font constant
#define LCD_MEDIUM_FONT        Font20 //Medium font constant
#define LCD_XSMALL_FONT         Font12 //Extra small font constant


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
    // Draws the button if needed
    void draw() {
        BSP_LCD_SetTextColor(_borderColor);
        BSP_LCD_DrawRect(_x,_y, _w,_h);
        BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
        BSP_LCD_SetFont(&LCD_MEDIUM_FONT);
        //Calculates to center text on button
        int numChar = _label.length();
        int stringWidth = numChar*14;
        int marginToCenterTextH = (_w-stringWidth)/2;
        int marginToCenterTextV = (_h-20)/2;
        BSP_LCD_DisplayStringAt(_x+marginToCenterTextH,_y+marginToCenterTextV, (uint8_t *)_label.c_str(), LEFT_MODE);
    };

    //Returns true if the area of the button is touched
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