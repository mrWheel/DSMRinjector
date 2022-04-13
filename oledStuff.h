/*
***************************************************************************
**  Program  : oledStuff.h, part of DSMRloggerAPI
**  Version  : v3.0
**
**  Copyright (c) 2020 Willem Aandewiel
**
**  TERMS OF USE: MIT License. See bottom of file.
***************************************************************************
*/

#include "SSD1306Ascii.h"       // https://github.com/greiman/SSD1306Ascii
#include "SSD1306AsciiWire.h"   // Version 1.2.x - Commit 97a05cd on 24 Mar 2019

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

// Define proper RST_PIN if required.
#define RST_PIN -1

SSD1306AsciiWire oled;

void oledPrintMsg(uint8_t, String, uint16_t);

//static bool     btnPressState = LOW;
static uint8_t  msgMode = 0;
static bool     boolDisplay = true;
static uint8_t  settingOledType = 1;  // 0=none, 1=SSD1306, 2=SH1106
static uint16_t settingOledSleep;
static uint8_t  settingOledFlip;
static String   oledLines[4];

uint8_t     lineHeight, charHeight;

DECLARE_TIMER_MIN(oledSleepTimer, 10);  // sleep the display in 10 minutes

//===========================================================================================
void oled_Init()
{
  Wire.begin();
  if (settingOledType == 2)
    oled.begin(&SH1106_128x64, I2C_ADDRESS);
  else  oled.begin(&Adafruit128x64, I2C_ADDRESS);

  oled.setFont(X11fixed7x14B);  // this gives us 4 rows by 18 chars
  charHeight  = oled.fontHeight();
  lineHeight  = oled.displayHeight() / 4;
  Debugf("OLED is [%3dx%3d], charHeight[%d], lineHeight[%d], nrLines[%d]\r\n", oled.displayWidth()
         , oled.displayHeight()
         , charHeight, lineHeight, 4);
  boolDisplay = true;
  if (settingOledFlip)  oled.displayRemap(true);
  RESTART_TIMER(oledSleepTimer);

}   // oled_Init()

//===========================================================================================
void oled_Clear()
{
  oled.clear();
  memset(oledLines, 0, sizeof(oledLines));

}   // oled_Clear


//===========================================================================================
DECLARE_TIMER_MS(timer, 0);
void oledPrintMsg(uint8_t line, String message, uint16_t wait)
{
  if (!boolDisplay) return;

  message += "                    ";
  oled.setCursor(0, ((line * lineHeight)/8));
  oled.print(message.c_str());

  if (wait>0)
  {
    CHANGE_INTERVAL_MS(timer, wait);
    RESTART_TIMER(timer);
    while (!DUE(timer))
    {
      delay(1);
    }
  }

}   // oledPrintMsg()


//===========================================================================================
void oledPrintLine(String message)
{
  for(int i=0; i<4; i++)
  {
    oledLines[i] = oledLines[i+1];
  }
  oledLines[3] = String(message+"                       ").substring(0, 17);
  for(int i=0; i<4; i++)
  {
    oled.setCursor(i, ((i * lineHeight)/8));
    oled.print(oledLines[i]);
  }
  delay(500);

}   // oledPrintLine()


/***************************************************************************
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the
* following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
* OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
* THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
***************************************************************************/
