
char    tlgrmName[10][40];
uint8_t fnInx = 0;
//=======================================================================
void selectDSMRfile()
{
  char selChar = '*';
  int  selInt = 255;

  memset(tlgrmName, 0, sizeof(tlgrmName));
  fnInx = 0;

  listAllTelegramsInDir("/");

  for(int i=1; i<=fnInx; i++)
  {
    Debugf("[%d] - %s\r\n", i, tlgrmName[(i-1)]);
  }

  //-- empty buffer
  while (TelnetStream.available())
  {
    TelnetStream.read();
    yield();
  }

  selInt = -1;
  while ((selInt < 0) || (selInt > fnInx))
  {
    yield();
    Debug("\r\nSelect : ");
    selChar = '*';
    selInt = 255;
    uint32_t waitForChar = millis();
    while((millis() - 5000) < waitForChar)
    {
      selChar = (char)TelnetStream.read();
      selInt = ((int)selChar) - ((int)'0');
      if (selInt > 0 && selInt <= fnInx) break;

    }
    if ((millis() - 5000) >= waitForChar) break;

  }

  //-- skip rest --
  while (TelnetStream.available())
  {
    yield();
    (char)TelnetStream.read();
  }

  if (selInt > 0 && selInt <= fnInx)
  {
    Debugf("[%c]/[%d] You selected this [%s] telegramfile\r\n\n", selChar, selInt, tlgrmName[(selInt-1)]);
    snprintf(telegramFileName, sizeof(telegramFileName), "/%s", tlgrmName[(selInt-1)]);
    telegramFile.close();
    sprintf(actDSMR, "FS");
    writeSettings();
  }
  else  Debugln("Timeout!");

} //  selectDSMRfile()


//=======================================================================
void listAllTelegramsInDir(const char *path)
{
  char dirFile[40] = {};

  Dir dir = FSYS.openDir(path);
  while(dir.next())
  {
    if (dir.isFile())
    {
      // print file names
      if (dir.fileName().indexOf(".dat") != -1)
      {
        snprintf(tlgrmName[fnInx++], sizeof(tlgrmName[fnInx]), "%s", dir.fileName().c_str());
      }
    }
    if (dir.isDirectory())
    {
      // print directory names
      Debug("Dir: ");
      snprintf(dirFile, sizeof(dirFile), "%s%s/", path, dir.fileName().c_str());
      //Debugln(path + dir.fileName() + "/");
      Debugln(dirFile);
      // recursive file listing inside new directory
      listAllTelegramsInDir(dirFile);
    }
  }
} //  listAllTelegramsInDir()


//=======================================================================
void handleKeyInput()
{
  //=======================================================================
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
        checkESP8266();
        break;
      case 'D':
        selectDSMRfile();
        break;
      case 's':
      case 'S':
        runStatus = !runStatus;
        break;
      case 'v':
      case 'V':
        Verbose = !Verbose;
        verboseCount = 0;
        break;
      case 'R':
        Debugln("\n==========================================");
        Debugln("      R E B O O T   R E Q U E ST E D");
        Debugln("==========================================");
        delay(2000);
        ESP.reset();
        break;
      /*
            case 'W':     { WiFiManager manageWiFi;
                            DebugTln("**============================**");
                            DebugTln("** RESETTING WIFI CREDENTIALS **");
                            DebugTln("**============================**");
                            DebugFlush();
                            manageWiFi.resetSettings();
                            ESP.reset();
                          }
                          break;
      */
      default:
        Debugln("\nCommandos are:\r\n");
        Debugln("   B - Board, Build info & system status\r");
        Debugln("  *D - Select DSMR standard (or file)\r");
        Debugf( "   S - Start/Stop sending telegrams [%s]\r\n", runStatus ? "Running":"Stopped");
        Debugf( "   V - Toggle Verbose [%s] (less telegrams will be send!)\r\n", Verbose ? "On":"Off");
        Debugln("  *R - Reboot\r");
        Debugln("  *W - reset WiFi credentials\r");
        Debugln(" ");

    } // switch()
  }

} // handleKeyInput()
