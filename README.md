# Simple THAT controller
Simple controller for the THAT analog computer based on Arduino Nano
![Patchpanel](https://github.com/dl3hrt/Simple-THAT-controller/blob/main/Simple%20THAT%20Controller%20-%20Inside.jpg)

![Patchpanel](https://github.com/dl3hrt/Simple-THAT-controller/blob/main/Simple%20THAT-Controller.jpg)

This is a simple controller for the THAT analog computer (see: https://shop.anabrid.com/)

Schematic:
![Patchpanel](https://github.com/dl3hrt/Simple-THAT-controller/blob/main/Simple%20THAT%20Controller%20-%20Schematic.png)
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

![Patchpanel](https://github.com/dl3hrt/Simple-THAT-controller/blob/main/Simple%20THAT%20Controller%20-%20setting%20cycle%20time.jpg)

Code:
