# Simple THAT controller
Simple controller for the THAT analog computer based on Arduino Nano
![Patchpanel](https://github.com/dl3hrt/Simple-THAT-controller/blob/main/Simple%20THAT%20Controller%20-%20Inside.jpg)

![Patchpanel](https://github.com/dl3hrt/Simple-THAT-controller/blob/main/Simple%20THAT-Controller.jpg)

This is a simple controller for the THAT analog computer (see: https://shop.anabrid.com/). 
It does nothing more than take over the function of the rotary switch on the THAT. One simply switches on the individual modes by pressing the corresponding button. The cycle times in the REP and REPF modes can be set via a potentiometer. It is also possible to set the IC time. This setting is permanently stored in the EEPROM of the Arduino Nano.

## Schematic

The controller is connected to the hybrid port of THAT. The 5 V power supply is provided by THAT.

![Patchpanel](https://github.com/dl3hrt/Simple-THAT-controller/blob/main/Simple%20THAT%20Controller%20-%20Schematic%2020221115.png)
Note: If P1 is replaced by a logarithmic potentiometer R1 can be omitted. 

Description of function keys:
* ON/OFF:
  * short keypress: enable/disable remote THAT control
  * long keypress: cancel IC time setup
* HALT: start HALT mode
* IC: (Initial Condition)
  * short keypress: start IC mode
  * short keypress during IC time setup: store new IC time in EEPROM
  * long keypress at OFF: setup IC time (adjust IC time with potentiometer between 1 ms and 50 ms)
* OP: start OP mode
* REP: start REP mode (adjust cycle time with potentiometer between 10 ms and 10 s)
* REPF: start REPF mode (adjust cycle time with potentiometer between 1 ms and 100 ms)

![Patchpanel](https://github.com/dl3hrt/Simple-THAT-controller/blob/main/Simple%20THAT%20Controller%20-%20Setting%20cycle%20time.jpg)

## Source code

https://github.com/dl3hrt/Simple-THAT-controller/blob/main/THAT-Controller.ino 
