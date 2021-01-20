# OfficeController

A small, embedded controller project by Christian Nick Rasmussen.
Project started on 2021/01/11

The premise is an embedded controller for an office environment, where temperature and sound level is monitered. 

## Parts List

<ul>
<li>STM32 F7 Discovery Board</li>
<li>USB A to mini-B cable</li>
<li>Grove Button</li>
<li>Grove Temperature Sensor V1.2</li>
<li>Grove Sound Sensor</li>
<li>Grove Light Sensor</li>
<li>3 x Grove LED Sockets</li>
<li>Grove Red LED</li>
<li>Grove Green LED</li>
<li>Grove Blue LED</li>
<li>8 x 26AWG Grove Cables</li>
</ul>

## Connections
<ul>
<li>Grove Temperature Sensor V1.2 => A1</li>
<li>Grove Sound Sensor => A3</li>
<li>Grove Light Sensor => A2</li>
<li>3 x Grove LED Sockets => D2, D3 and D5</li>
<li>Grove Red LED in socket => D2</li>
<li>Grove Green LED in socket => D3</li>
<li>Grove Blue LED in socket => D5</li>
<li>Grove Button => D4</li>
</ul>

## Features
### Start Screen
Shows version and if touchscreen is initialized. 

![Start Screen Pic](https://i.imgur.com/gtavAgM.jpg)

### Input Screen
A screen to input location name through a serial connection using a keyboard

![Input Screen Pic](https://i.imgur.com/LEBHKNo.jpg)
### Home Screen

#### Header
Shows name of the project, and office location. Just underneath is a label showing whether the light sensor detects its day or night. On the left there is a home button. which always takes you to the home screen.

#### Main
The left side shows current temperature. To switch between Celsius and Fahrenheit, press Grove Button. The optimal temperature is 25°C on this controller. If the temperature drops, a heater turns on, symbolized by the Red Led. If the temperature rises, the air condition turns on, symbolized by the Blue Led.

The right side shows sound level. If noise becomes too high, the screen will show a warning. Every warning is counted and show underneath, to reset, press the counter. If it is night and noise is too high, an alarm goes off, symbolized by the green LED blinking. To turn the alarm off you have to double press the screen on the sound level text.


#### Bottom Buttons
Used to cycle through different screens, by touch.

![Home Screen Pic](https://i.imgur.com/dxrXLZD.jpg)

### Sound Graph Screen
Shows the last 100 measurements of sound level on a graph and the average soundlevel. 

![Home Screen Pic](https://i.imgur.com/BfEW3mP.jpg)

### Temperature Graph Screen
Shows the last 100 measurements of the temperature on a graph and the average temperature.

![Home Screen Pic](https://i.imgur.com/2ps0xZ5.jpg)