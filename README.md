# OfficeController

A small embedded controller project by Christian Nick Rasmussen.
Project started on 04/04/2021

The premise is an embedded controller for an office environment, where temperature and sound level is monitered. 

## Parts List

<ul>
<li>STM32 F7 Discovery Board</li>
<li>USB A to mini B cable</li>
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
<li>3 x Grove LED Sockets => D2 and D3</li>
<li>Grove Red LED in socket => D2</li>
<li>Grove Green LED in socket => D3</li>
<li>Grove Blue LED in socket => D5</li>
<li>Grove Button => D4</li>
</ul>

## Features
### Start Screen
Shows version and if touchscreen is initialized. 

### Input Screen
A screen to input location name through a serial connection using a keyboard

### Home Screen

#### Header
Shows name of the project, and office location. Just underneath is a label showint whether the light sensor detects its day or night.

#### Main
The left side shows current temperature. To switch between celcius and fahrenheit, press Grove Button. The optimal temperature is 25°C on this controller. If the the temperature drops, a heater turns on, symbolized by the Red Led.If the the temperature rises, the aircondition turns on, symbolized by the Blue Led.

The right side shows sound level. If noise becomes too high, the screen will show a warning. Every warning is counted and show underneath, to reset, press the counter. If it is night and noise is too high, an alarm goes off, symbolized by the green LED blinking. To turn the alarm off you have to double press the screen on the sound level text. 

#### Bottom Buttons
Used to cycle through different screens, by touch.

### Sound Graph Screen
Shows the last 100 measurements of sound level on a graph.//In progress

### Temperature Graph Screen
Shows the last 100 measurements of the temperature on a graph.//In progress
