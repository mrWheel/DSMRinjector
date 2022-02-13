

//=======================================================================
void handleKeyInput() {
//=======================================================================
  String  slavesInfo, response; 
  int16_t num; 
  char    inChar;
  
  while (TelnetStream.available() > 0) {
    yield();
    inChar = (char)TelnetStream.read();
    while (TelnetStream.available() > 0) {
       yield();
       (char)TelnetStream.read();
    }

    switch(inChar) {
      case 'b':
      case 'B':     checkESP8266();
                    break;
      case 's':
      case 'S':     runStatus = !runStatus;
                    break;
      case 'v':
      case 'V':     Verbose = !Verbose;
                    verboseCount = 0;
                    break;
      case 'R':     Debugln("\n==========================================");
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
      default:      Debugln("\nCommandos are:\r\n");
                    Debugln("   B - Board, Build info & system status\r");
                    Debugf( "   S - Start/Stop sending telegrams [%s]\r\n", runStatus ? "Running":"Stopped");
                    Debugf( "   V - Toggle Verbose [%s] (less telegrams will be send!)\r\n", Verbose ? "On":"Off");
                    Debugln("  *R - Reboot\r");
                    Debugln("  *W - reset WiFi credentials\r");
                    Debugln(" ");

    } // switch()
  }
  
} // handleKeyInput()
