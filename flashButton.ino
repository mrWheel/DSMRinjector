
//-- debounce time in MicroSeconds
#define _DEBOUNCETIME     10
//-- press times in MilliSeconds
#define _MIDPRESSTIME    1000
#define _LONGPRESSTIME   2500


//------ finite states ---------------------------------------------------------------
enum  {  BTN_INIT, BTN_FIRST_PRESS, BTN_IS_PRESSED, BTN_FIRST_RELEASE, BTN_IS_RELEASED };


//==========================================================================
void handleFlashButton()
{
  //---------------------------------------------------------------
  //-------------- finite state button ----------------------------
  //---------------------------------------------------------------

  switch(btnState)
  {
    case BTN_INIT:
      if (!digitalRead(_FLASH_BUTTON))
      {
        bounceTimer       = micros();
        btnState          = BTN_FIRST_PRESS;
      }
      break;

    case BTN_FIRST_PRESS:
      if ((micros() - bounceTimer) > _DEBOUNCETIME)
      {
        if (!digitalRead(_FLASH_BUTTON))
        {
          btnState    = BTN_IS_PRESSED;
          buttonTimer = millis();
        }
        else
        {
          btnState    = BTN_INIT;
        }
      }
      break;

    case BTN_IS_PRESSED:
      //-- button released?
      if (digitalRead(_FLASH_BUTTON))
      {
        bounceTimer = micros();
        btnState    = BTN_FIRST_RELEASE;
      }
      else
      {
        if ((millis() - buttonTimer) > _LONGPRESSTIME)
        {
          pressType = LONG_PRESSED;
          oledPrintMsg(1, "LONG Pressed!      ", 0);
        }
      }
      break;

    case BTN_FIRST_RELEASE:
      if ((micros() - bounceTimer) > _DEBOUNCETIME)
      {
        if (digitalRead(_FLASH_BUTTON) && (micros() - bounceTimer) > _DEBOUNCETIME)
        {
          btnState    = BTN_IS_RELEASED;
        }
        else
        {
          btnState    = BTN_IS_PRESSED;
        }
      }
      break;

    case BTN_IS_RELEASED:
      //aliveTimer = millis();
      Debugf("Button Released in [%d]ms! => ", (millis()-buttonTimer));
      if ((millis() - buttonTimer) > _LONGPRESSTIME)
      {
        //pressType = LONG_PRESSED;
        oledPrintMsg(1, "LONG Pressed!      ", 0);
        Debugln("Long Pressed: ");
      }
      else if ((millis() - buttonTimer) > _MIDPRESSTIME)
      {
        pressType = MID_PRESSED;
        oledPrintMsg(1, "MID Pressed!      ", 0);
        Debugln("Mid Pressed: ");
      }
      else
      {
        pressType = QUICK_PRESSED;
        oledPrintMsg(1, "QUICK Pressed!      ", 0);
        Debugln("Quick Pressed: ");
      }
      btnState    = BTN_INIT;
      bounceTimer = 0;
      showBtnTimer = millis();
      break;

    default:
      btnState    = BTN_INIT;
      pressType   = NOT_PRESSED;

  } // switch()

} // handleFlashButton()

//===========================================================================================
//assumes little endian
void printRegister(size_t const size, void const *const ptr)
{
  unsigned char *b = (unsigned char *) ptr;
  unsigned char byte;
  int i, j;
  Debug(F("["));
  for (i=size-1; i>=0; i--)
  {
    for (j=7; j>=0; j--)
    {
      byte = (b[i] >> j) & 1;
      Debug(byte);
    }
  }
  Debug(F("] "));

} // printRegister()
