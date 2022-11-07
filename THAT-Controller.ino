//********************************************************************************
// Simple THAT Controller
//
// Author:   Karsten Hansky
// Version : V1.0
// Date:     07.11.2022
// 
// Simple controller for the THAT "The Analog Thing" computer. 
//
// Project description: https://github.com/dl3hrt/Simple-THAT-Controller
//
// Libs:
// - RunningMedian: https://github.com/RobTillaart/RunningMedian
// - U8x8: https://github.com/olikraus/u8g2/ 
// 
//********************************************************************************
#include <RunningMedian.h>      // median filter for pot reading
#include <U8x8lib.h>            // display in text mode
#include <EEPROM.h>             // eeprom functions

// output pins
#define PIN_ENABLE            2
#define PIN_OP                3
#define PIN_IC                4

// input pins
#define KEY_ENABLE            5
#define KEY_HALT              6
#define KEY_IC                7
#define KEY_OP                8
#define KEY_REP               9
#define KEY_REPF             10

// modes
#define MODE_IC               0 // Mode IC
#define MODE_OP               1 // Mode OP
#define MODE_HALT             2 // Mode HALT

#define EEPROM_DURATION_IC    0 // EERPOM address
#define MAX_DURATION_IC      50 // 50 ms
#define DEFAULT_DURATION_IC  10 // 10ms

bool      isEnabled = false;
bool      isREP = false;
bool      isREPF = false;

int       cycle;
int       durationIC;

uint32_t  startMillis;

RunningMedian pot_Median = RunningMedian(15);
U8X8_SSD1306_64X32_1F_HW_I2C u8x8(U8X8_PIN_NONE);

char      szString[32];

void setup() 
{
  pinMode(PIN_ENABLE,OUTPUT);
  pinMode(PIN_OP,OUTPUT);
  pinMode(PIN_IC,OUTPUT);
  pinMode(13,OUTPUT);
    
  pinMode(KEY_ENABLE,INPUT_PULLUP);
  pinMode(KEY_HALT,INPUT_PULLUP);
  pinMode(KEY_IC,INPUT_PULLUP);
  pinMode(KEY_OP,INPUT_PULLUP);
  pinMode(KEY_REP,INPUT_PULLUP);
  pinMode(KEY_REPF,INPUT_PULLUP);

  doDisable();
  setMode(MODE_HALT);

  durationIC = EEPROM.read(EEPROM_DURATION_IC);
  if(!durationIC) durationIC = DEFAULT_DURATION_IC;
  if(durationIC > MAX_DURATION_IC) durationIC = MAX_DURATION_IC;

  u8x8.begin();
  u8x8.setContrast(64);
  u8x8.setFont(u8x8_font_pxplusibmcgathin_r); 

  u8x8.drawString(1,0,"THAT-");
  u8x8.drawString(0,1,"Control");
  u8x8.drawString(2,2,"V1.0");

  sprintf(szString,"IC:%dms",durationIC);
  u8x8.drawString(0,3,szString);

  delay(3000);
  
  u8x8.clearDisplay();
  u8x8.drawString(2,1,"OFF");
     
} // setup

void loop() 
{
  // check for keypress
  if(!digitalRead(KEY_ENABLE)) 
  {
    if(isEnabled)
      doDisable();
    else
      doEnable();
      
    delay(300);

  } // if(!digitalRead(KEY_ENABLE)) 

  if(isEnabled)
  {
    if(!digitalRead(KEY_HALT))
    {
      doHALT();
      isREP = isREPF = false;
      
      delay(300);
          
    } // if(!digitalRead(KEY_HALT))
  
    if(!digitalRead(KEY_IC))
    {
      doIC();
      isREP = isREPF = false;
      
      delay(300);
          
    } // if(!digitalRead(KEY_IC))
  
    if(!digitalRead(KEY_OP))
    {
      doOP();
      isREP = isREPF = false;
      
      delay(300);
          
    } // if(!digitalRead(KEY_OP))
  
    if(!digitalRead(KEY_REP))
    {
      doREP();
      delay(300);
          
    } // if(!digitalRead(KEY_REP))
  
    if(!digitalRead(KEY_REPF))
    {
      doREPF();
      delay(300);
          
    } // if(!digitalRead(KEY_REPF))
  } // if(isEnabled)
  else
  {
    if(!digitalRead(KEY_IC))
    {
      // IC key: long keypress -> IC cycle setup
      delay(500);
      if(!digitalRead(KEY_IC)) doSetupIC();
        
    } // if(!digitalRead(KEY_IC))
  } // else if(isEnabled)

  if(isREP || isREPF)  
  {
    // pot abfragen
    pot_Median.add(analogRead(A7));
    cycle = (int)((float)pot_Median.getMedian()/1000.0*1000); 
    cycle = cycle*10;       // 10ms steps

    if(!cycle) cycle = 10;
    if(cycle > 10000) cycle = 10000;
    if(isREPF) cycle = cycle/100;

    if(millis() - startMillis > cycle)
    {
      setMode(MODE_IC);
      delay(durationIC);
      setMode(MODE_OP);

      startMillis = millis();
      
    } // if(millis() - startMillis > cycle)

    if(isEnabled)
    {
      sprintf(szString,"%5d ms",cycle);
      u8x8.drawString(0,2,szString);
      
    } // if(isEnabled)

  } // if(isREP || isREPF)  
} // loop

//********************************************************************************
// set_mode(int mode):
//
//********************************************************************************
void setMode(int mode) 
{
  switch(mode)
  {
    case MODE_IC:
      digitalWrite(PIN_IC,LOW);
      digitalWrite(PIN_OP,HIGH);
      break;
    
    case MODE_OP:
      digitalWrite(PIN_IC,HIGH);
      digitalWrite(PIN_OP,LOW);
      break;
      
    case MODE_HALT:
      digitalWrite(PIN_IC,HIGH);
      digitalWrite(PIN_OP,HIGH);
      break;

  } // switch(mode)
} // setMode

//********************************************************************************
// doEnable():
//
//********************************************************************************
void doEnable()
{
  u8x8.clearDisplay(); 

  // enable hybrid mode
  setMode(MODE_HALT);
  digitalWrite(PIN_ENABLE,LOW);
  isEnabled = true;

  u8x8.clearLine(1);

  if(isREP)
    u8x8.drawString(0,0,"REP ");
  else 
    if(isREPF)
      u8x8.drawString(0,0,"REPF");
    else
      u8x8.drawString(0,0,"HALT");
      
} // doEnable

//********************************************************************************
// doDisable():
//
//********************************************************************************
void doDisable()
{
  // disable hybrid mode
  digitalWrite(PIN_ENABLE,HIGH);
  isEnabled = false;  

  u8x8.clearDisplay();     
  u8x8.drawString(2,1,"OFF");
      
} // doDisable

//********************************************************************************
// doHALT():
//
//********************************************************************************
void doHALT()
{
  setMode(MODE_HALT);

  u8x8.clearLine(2);
  u8x8.drawString(0,0,"HALT");
  
} // doHALT

//********************************************************************************
// doIC():
//
//********************************************************************************
void doIC()
{
  setMode(MODE_IC);

  u8x8.clearLine(2);
  u8x8.drawString(0,0,"IC  ");
  
} // doIC

//********************************************************************************
// doOP():
//
//********************************************************************************
void doOP()
{
  setMode(MODE_OP);
  
  u8x8.clearLine(2);
  u8x8.drawString(0,0,"OP  ");
  
} // doOP

//********************************************************************************
// doREP():
//
//********************************************************************************
void doREP()
{
  if(!isREP)
  {
    setMode(MODE_HALT);
    isREPF = false;
    isREP = true;
    startMillis = millis() + 20000;
    
    u8x8.clearLine(2);
    u8x8.drawString(0,0,"REP ");
    
  } // if(!isREP)
} // doREP

//********************************************************************************
// doREPF():
//
//********************************************************************************
void doREPF()
{
  if(!isREPF)
  {
    setMode(MODE_HALT);
    isREP = false;
    isREPF = true;
    startMillis = millis() + 20000;

    u8x8.clearLine(2);
    u8x8.drawString(0,0,"REPF");
    
  } // if(!isREPF)
} // doREPF

//********************************************************************************
// doSetupIC():
//
//********************************************************************************
void doSetupIC()
{
  u8x8.clearDisplay();
  u8x8.drawString(0,0,"IC  ");
  
  // wait for release of IC key
  while(!digitalRead(KEY_IC)) delay(10);
  delay(100);

  do
  {
    // pot abfragen
    pot_Median.add(analogRead(A7));
    durationIC = (int)((float)pot_Median.getMedian()/1000.0*50);
 
    if(!durationIC) durationIC = 1;
    if(durationIC > MAX_DURATION_IC) durationIC = MAX_DURATION_IC;

    sprintf(szString,"%3d ms",durationIC);
    u8x8.drawString(0,2,szString);

    delay(10);

  } while(digitalRead(KEY_IC) && digitalRead(KEY_ENABLE));

  // IC key -> confirm
  // ENABLE key -> cancel
  if(!digitalRead(KEY_IC))
  {
    EEPROM.write(EEPROM_DURATION_IC,durationIC);
    u8x8.drawString(1,3,"STORED");
        
  } // if(!digitalRead(KEY_IC))
  else
    u8x8.drawString(1,3,"CANCEL");
    
  delay(2000);
  u8x8.clearDisplay();
  u8x8.drawString(2,1,"OFF");
   
} // doSetupIC
