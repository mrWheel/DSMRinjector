/*
***************************************************************************
**  Program  : settingsStuff, part of DSMRloggerAPI
**  Version  : v3.0
**
**  Copyright (c) 2020 Willem Aandewiel
**
**  TERMS OF USE: MIT License. See bottom of file.
***************************************************************************
* 1.0.11 added Mindergas Authtoken setting
*/

//=======================================================================
void writeSettings()
{
  yield();
  Debug(F("Writing to ["));
  Debug(_SETTINGS_FILE);
  Debugln(F("] ..."));
  File file = FSYS.open(_SETTINGS_FILE, "w"); // open for reading and writing
  if (!file)
  {
    Debugf("open(%s, 'w') FAILED!!! --> Bailout\r\n", _SETTINGS_FILE);
    return;
  }
  yield();

  Debug(F("Start writing setting data "));

  file.print("actDSMR = ");
  file.println(actDSMR);
  Debug(F("."));

  file.print("actInterval = ");
  file.println(actInterval);
  Debug(F("."));

  file.print("telegramFileName = ");
  file.println(telegramFileName);
  Debug(F("."));

  file.print("actGasMBus = ");
  file.println(actGasMBus);
  Debug(F("."));

  file.close();

  Debugln(F(" done"));
  Debugln(F("Wrote this:"));
  Debugf("actDSMR = [%s]\r\n", actDSMR);
  Debugf("actInterval = [%s]\r\n", actInterval);
  Debugf("telegramFileName = [%s]\r\n", telegramFileName);
  Debugf("actGasMBus = [%d]\r\n", actGasMBus);
  Debugln();

} // writeSettings()


//=======================================================================
void readSettings(bool show)
{
  String sTmp, nColor;
  String words[10];

  File file;

  Debugf(" %s ..\r\n", _SETTINGS_FILE);

  sprintf(actDSMR, "50");
  actGasMBus = 1;

  if (!FSYS.exists(_SETTINGS_FILE))
  {
    Debugln(F(" .. file not found! --> created file!"));
    writeSettings();
  }

  for (int T = 0; T < 2; T++)
  {
    file = FSYS.open(_SETTINGS_FILE, "r");
    if (!file)
    {
      if (T == 0) Debugf(" .. something went wrong opening [%s]\r\n", _SETTINGS_FILE);
      else        Debugln(T);
      delay(100);
    }
  } // try T times ..

  Debugln(F("Reading settings:\r"));
  while(file.available())
  {
    sTmp      = file.readStringUntil('\n');
    sTmp.replace("\r", "");
    //Debugf("[%s] (%d)\r\n", sTmp.c_str(), sTmp.length());
    int8_t wc = splitString(sTmp.c_str(), '=', words, 10);
    words[0].toLowerCase();
    nColor    = words[1].substring(0, 15);

    if (words[0].equalsIgnoreCase("actDSMR"))
      strCopy(actDSMR, sizeof(actDSMR), words[1].c_str());
    if (words[0].equalsIgnoreCase("actInterval"))   actInterval = words[1].toInt();
    if (words[0].equalsIgnoreCase("telegramFileName"))
      strCopy(telegramFileName, sizeof(telegramFileName), words[1].c_str());
    if (words[0].equalsIgnoreCase("actGasMBus"))   actGasMBus = words[1].toInt();

  } // while available()

  file.close();

  Debugln(F(" .. done\r"));

} // readSettings()


//=======================================================================
void updateSetting(const char *field, const char *newValue)
{
  Debugf("-> field[%s], newValue[%s]\r\n", field, newValue);

  writeSettings();

} // updateSetting()


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
