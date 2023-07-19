
//=======================================================================
void selectDSMRfile()
{
  char selInput[10] = {};
  int  maxIndx = 0, selInt = 255;

  Debugln("[A]  - DSMR 3+");
  Debugln("[B]  - DSMR 42");
  Debugln("[C]  - DSMR 5+");
  Debugln("[D]  - DSMR 5+ BE");

  maxIndx = listAllTelegramsInDir("/");

  Debug("=> Select: ");

  uint32_t waitTimer = millis();
  int len = 0;
  while ((millis() - waitTimer) < 5000)
  {
    len = TelnetStream.readBytesUntil('\n', selInput, sizeof(selInput));
    if (len > 0) break;
  }
  if (len > 0 && selInput[(len-1)] == '\r') selInput[(len-1)] = 0; //-- remove '\r'
  if ((millis() - waitTimer) >= 5000)
  {
    Debugln(" timeout!");
    selInt = 0;
    return;
  }
  else
  {
    selInt = String(selInput).toInt();
    Debugf("You selected [%s](%d) ", selInput, selInt);
  }

  if (selInt == 0 && strlen(selInput) > 0)
  {
    switch(selInput[0])
    {
      case 'a':
      case 'A':
        sprintf(actDSMR, "30");
        break;
      case 'b':
      case 'B':
        sprintf(actDSMR, "42");
        break;
      case 'c':
      case 'C':
        sprintf(actDSMR, "50");
        break;
      case 'd':
      case 'D':
        sprintf(actDSMR, "BE");
        break;
      default:
        sprintf(actDSMR, "50");

    } // switch
    if (actDSMR != savDSMR)
    {
      //--aaw-runStatus = 0;
      memset(telegramFileName, 0, sizeof(telegramFileName));
      telegramFile.close();
      Debugf("\r\nYou selected DSMR profile [%s]\r\n\n", actDSMR);
      return;
    }
  }
  if (selInt > 0 && selInt <= maxIndx)
  {
    //--aaw-runStatus = 0;
    memset(telegramFileName, 0, sizeof(telegramFileName));
    telegramFile.close();
    readFileName("/", selInt);
    Debugf("=> [%s]\r\n\n", telegramFileName);
    if (strlen(telegramFileName) > 0)
      sprintf(actDSMR, "FS");
    else  sprintf(actDSMR, "50");
  }
  else
  {
    return;
  }
  writeSettings();

  //-- skip rest --
  while (TelnetStream.available())
  {
    yield();
    (char)TelnetStream.read();
  }

} //  selectDSMRfile()


//=======================================================================
uint8_t listAllTelegramsInDir(const char *path)
{
  char dirFile[40] = {};
  int8_t fnIndx = 1;

  Dir dir = FSYS.openDir(path);
  while(dir.next())
  {
    if (dir.isFile())
    {
      // print file names
      if (dir.fileName().indexOf(".dat") != -1)
      {
        //snprintf(tlgrmName[++fnInx], sizeof(tlgrmName[fnIndx]), "%s", dir.fileName().c_str());
        Debugf("[%2d] - %s\r\n", fnIndx++, dir.fileName().c_str());
      }
    }
    else if (dir.isDirectory())
    {
      Debugf("DIR %s\r\n", dir.fileName().c_str());
    }
  }

  //dir.close();

  return fnIndx;

} //  listAllTelegramsInDir()


//=======================================================================
void readFileName(const char *path, int fileNr)
{
  char dirFile[40] = {};
  int8_t fnIndx = 0;

  Dir dir = FSYS.openDir(path);
  while(dir.next())
  {
    if (dir.isFile())
    {
      // print file names
      if (dir.fileName().indexOf(".dat") != -1)
      {
        fnIndx++;
        if (fnIndx == fileNr)
        {
          snprintf(telegramFileName, sizeof(telegramFileName), "%s", dir.fileName().c_str());
          return;
        }
      }
    }
  }

} //  readFileName()


//=======================================================================
void handleKeyInput()
{
  String  slavesInfo, response;
  int16_t num;
  char    inChar;

  while (TelnetStream.available() > 0)
  {
    yield();
    inChar = (char)TelnetStream.read();
    while (TelnetStream.available() > 0)
    {
      yield();
      (char)TelnetStream.read();
    }

    switch(inChar)
    {
      case 'b':
      case 'B':
        showBoardInfo();
        break;
      case 'D':
        selectDSMRfile();
        break;
      case 'i':
      case 'I':
        ignoreDTR = !ignoreDTR;
        break;
      case 's':
      case 'S':
        runStatus = !runStatus;
        if (runStatus)  oledPrintMsg(1, "Status: Running",0);
        else            oledPrintMsg(1, "Status: Stopped",0);

        break;
      case 'v':
      case 'V':
        if (!Verbose) Debugln("Start showing every 3th telegram");
        Verbose = !Verbose;
        verboseCount = 0;
        break;
      case 'R':
        Debugln("\n==========================================");
        Debugln("      R E B O O T   R E Q U E ST E D");
        Debugln("==========================================");
        oledPrintMsg(0, "==== RE-BOOT ====",1);
        DebugFlush();
        TelnetStream.stop();
        delay(2000);
        ESP.restart();
        delay(5000);
        break;
      case 'W':     
        { 
          WiFiManager manageWiFi;
          DebugTln("**============================**");
          DebugTln("** RESETTING WIFI CREDENTIALS **");
          DebugTln("**         AND REBOOT         **");
          DebugTln("**============================**");
          DebugFlush();
          oledPrintMsg(0, "==== RE-BOOT ====",1);
          manageWiFi.resetSettings();
          DebugTln("**         R E B O O T        **");
          DebugFlush();
          TelnetStream.stop();
          delay(2000);
          ESP.reset();
          delay(5000);
        }
        break;
 
      default:
        Debugln("\nCommandos are:\r\n");
        Debugln("   B - Board, Build info & system status\r");
        Debugln("  *D - Select DSMR standard (or file)\r");
        Debugf( "   S - Start/Stop sending telegrams [%s]\r\n", runStatus ? "Running":"Stopped");
        Debugf( "   I - Ignore DTR [%s]\r\n", ignoreDTR ? "True":"False");
        Debugf( "   V - Toggle Verbose [%s] (less telegrams will be send!)\r\n", Verbose ? "On":"Off");
        Debugln("  *R - Reboot\r");
        Debugln("  *W - reset WiFi credentials\r");
        Debugln(" ");

    } // switch()
  }

} // handleKeyInput()
