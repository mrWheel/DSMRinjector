

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
      case 'v':
      case 'V':     Verbose = !Verbose;
                    break;
      case 'R':     Debugln("\n==========================================");
                    Debugln("      R E B O O T   R E Q U E ST E D");
                    Debugln("==========================================");
                    delay(2000);
                    ESP.reset();
                    break;
/*
      case 'W':     { WiFiManager manageWiFi;
                      _dThis = true;
                      Debugln("**============================**");
                      Debugln("** RESETTING WIFI CREDENTIALS **");
                      Debugln("**============================**");
                      DebugFlush();
                      manageWiFi.resetSettings();   
                      ESP.reset();
                    }
                    break;
*/
      default:      Debugln("\nCommandos are:\n");
                    Debugln("   B - Board, Build info & system status");
                    Debugln("   V - Toggle Verbose");
                    Debugln("  *R - Reboot");
                    Debugln("  *W - reset WiFi credentials");
                    Debugln(" ");

    } // switch()
  }
  
} // handleKeyInput()
