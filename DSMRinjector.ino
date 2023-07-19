/*
** DSMRinjector v2.0
**
*/
#define _FW_VERSION "2.1 (19-07-2023)"
/*
   Arduino-IDE settings for ESP12 (Generic):

    - Board: "Generic ESP8266 Module"
    - Flash mode: "DOUT"
    - Flash size: "4M (FS 2MB)"
    - Debug port: "Disabled"
    - Debug Level: "None"
    - IwIP Variant: "v2 Lower Memory"
    - Reset Method: "nodemcu"   // but will depend on the programmer!
    - Crystal Frequency: "26 MHz"
    - VTables: "Flash"
    - Flash Frequency: "40MHz"
    - CPU Frequency: "80 MHz"
    - Buildin Led: "2"  // GPIO02 - Pin 2
    - Upload Speed: "115200"
    - Erase Flash: "Only Sketch"
    - Port: "?"

*/

/*--------------------------------------------------------
    Wemos D1 Pinout hardware 18-07-2023 v2.1c

                                  /----------------\
                                  |O +-+-  --  --  |
                                  |  | | | | |  |  |
                                  |  | | --  --  - |
                            RST   |                |  TX   GPIO1 -->
                     ADC0    A0   |                |  RX   GPIO3 <--
                   GPIO16    D0   |                |  D1   GPIO5 SCL
           LED <-- GPIO14    D5   |                |  D2   GPIO4 SDA
           DTR --> GPIO12    D6   |                |  D3   GPIO0 <-- BUTTON
          Swap(Rx) GPIO13    D7   |                |  D4   GPIO2 --> (LED_BUILTIN) 
          Swap(Tx) GPIO15    D8   |                |  GND
                            3v3   |                |  5V
                                   > reset        O|
                                   +---------------+
**
** Formatting ( http://astyle.sourceforge.net/astyle.html#_Quick_Start )
**   - Allman style (-A1)
**   - tab 2 spaces (-s2)
**   - Indent 'switch' blocks (-S)
**   - Indent preprocessor blocks (-xW)
**   - Indent multi-line preprocessor definitions ending with a backslash (-w)
**   - Indent C++ comments beginning in column one (-Y)
**   - Insert space padding after commas (-xg)
**   - Attach a pointer or reference operator (-k3)
**
** use:  astyle <*.ino>
**
*/

#include "DSMRinjector.h"


//===========================================================================================
void callIndex_html()
{
  Serial.println("uri() => / !");
  Debugln("uri() => / !");
  httpServer.send(200, "text/html", DSMRindex_html );

} // callIndex_html()


//===========================================================================================
void handleReBoot()
{
  String redirectHTML = "";

  redirectHTML += "<!DOCTYPE HTML><html lang='en-US'>";
  redirectHTML += "<head>";
  redirectHTML += "<meta charset='UTF-8'>";
  redirectHTML += "<style type='text/css'>";
  redirectHTML += "body {background-color: lightblue;}";
  redirectHTML += "</style>";
  redirectHTML += "<title>Redirect to DSMRinjector2</title>";
  redirectHTML += "</head>";
  redirectHTML += "<body><h1>DSMRinjector2</h1>";
  redirectHTML += "<h3>Rebooting DSMRinjector2</h3>";
  redirectHTML += "<br><div style='width: 500px; position: relative; font-size: 25px;'>";
  redirectHTML += "  <div style='float: left;'>Redirect over &nbsp;</div>";
  redirectHTML += "  <div style='float: left;' id='counter'>20</div>";
  redirectHTML += "  <div style='float: left;'>&nbsp; seconden ...</div>";
  redirectHTML += "  <div style='float: right;'>&nbsp;</div>";
  redirectHTML += "</div>";
  redirectHTML += "<!-- Note: don't tell people to `click` the link, just tell them that it is a link. -->";
  redirectHTML += "<br><br><hr>If you are not redirected automatically, click this <a href='/'>DSMRinjector2</a>.";
  redirectHTML += "  <script>";
  redirectHTML += "      setInterval(function() {";
  redirectHTML += "          var div = document.querySelector('#counter');";
  redirectHTML += "          var count = div.textContent * 1 - 1;";
  redirectHTML += "          div.textContent = count;";
  redirectHTML += "          if (count <= 0) {";
  redirectHTML += "              window.location.replace('/'); ";
  redirectHTML += "          } ";
  redirectHTML += "      }, 1000); ";
  redirectHTML += "  </script> ";
  redirectHTML += "</body></html>\r\n";

  httpServer.send(200, "text/html", redirectHTML);

  Debugln("ReBoot DSMRinjector2 ..");
  TelnetStream.flush();
  delay(1000);
  ESP.reset();

} // handleReBoot()


//===========================================================================================
void reloadPage(String goTo)
{
  String goToPageHTML;
  goToPageHTML += "<!DOCTYPE HTML><html lang='de'><head><meta charset='UTF-8'>";
  goToPageHTML += "<meta name= viewport content=width=device-width, initial-scale=1.0, user-scalable=yes>";
  goToPageHTML += "<style>body {background-color: powderblue;}</style>";
  goToPageHTML += "</head>\r\n<body><center>Wait ..</center>";
  goToPageHTML += "<br><br><hr>If you are not redirected automatically, click this <a href='/'>"+String(_HOSTNAME)+"</a>.";
  goToPageHTML += "  <script>";
  goToPageHTML += "    window.location.replace('" + goTo + "'); ";
  goToPageHTML += "  </script> ";
  httpServer.send(200, "text/html", goToPageHTML );

} // reloadPage()


//=======================================================================
int8_t splitString(String inStrng, char delimiter, String wOut[], uint8_t maxWords)
{
  uint16_t inxS = 0, inxE = 0, wordCount = 0;
  inStrng.trim();
  while(inxE < inStrng.length() && wordCount < maxWords)
  {
    inxE  = inStrng.indexOf(delimiter, inxS);         //finds location of first ,
    wOut[wordCount] = inStrng.substring(inxS, inxE);  //captures first data String
    wOut[wordCount].trim();
    //DebugTf("[%d] => [%c] @[%d] found[%s]\n", wordCount, delimiter, inxE, wOut[wordCount].c_str());
    inxS = inxE;
    inxS++;
    wordCount++;
  }
  if (inxS < inStrng.length())
  {
    wOut[wordCount] = inStrng.substring(inxS, inStrng.length());  //captures first data String
    //DebugTf("[%d] rest => [%s]\n", wordCount, wOut[wordCount].c_str());
  }

  return wordCount;

} // splitString()


//==================================================================================================
void showBoardInfo()
{
  uint32_t    realSize = ESP.getFlashChipRealSize();
  uint32_t    ideSize  = ESP.getFlashChipSize();
  FlashMode_t ideMode  = ESP.getFlashChipMode();

  Debugln("\n=============================================================");

  Debugf("   Flash real id: %08X\r\n", ESP.getFlashChipId());
  Debugf(" Flash real size: %u bytes\r\n", realSize);
  Debugf("  Flash ide size: %u bytes\r\n", ideSize);

  if (ideSize != realSize)
  {
    Debugln(">> Flash Chip configuration wrong! <<\r");
  }
  else
  {
    Debugln("Flash Chip configuration ok.\r");
  }
  Debugf(" Flash ide speed: %u Hz\r\n", ESP.getFlashChipSpeed());
  Debugf("  Flash ide mode: %s\r\n", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN"));

  Debugf("          Timing: %s\r\n", useNTPtiming ? "use NTP" : "INTERN");
  Debugf("        Run Mode: %d", runMode);
  switch(runMode)
  {
    case 0:
      Debugln(" (Initialize)");
      break;
    case 1:
      Debugln(" (Vul Maanden)");
      break;
    case 2:
      Debugln(" (Vul Dagen)");
      break;
    case 3:
      Debugln(" (Vul Uren)");
      break;
    case 4:
      Debugln(" (Normal Operation)");
      break;
    default:
      Debugln(" (Unknown..)");
  }

  Debugf("Minuten per stap: %d seconden\r\n", actSpeed);
  Debugf("        Interval: %d seconden\r\n", actInterval);
  Debugf("  gas meter MBus: %d seconden\r\n", actGasMBus);

  if (!strcmp(actDSMR, "FS"))
  {
    Debugf("  DSMR standaard: %s [%s]\r\n", actDSMR, telegramFileName);
  }
  else  Debugf("  DSMR standaard: %s\r\n", actDSMR);
  Debugf("        Checksum: %s\r\n", skipChecksum ? "Skip": "Check");
  Debugf("      Run Status: %d", runStatus);
  switch(runStatus)
  {
    case 0:
      Debugln(" (geStopt)\r");
      break;
    case 1:
      Debugln(" (Running)\r");
      break;
    default:
      Debugln(" (Unknown..)\r");
  }

  Debugf("Data Request pin: %s\r\n", digitalRead(_DATA_REGUEST) ? "High":"Low");
  Debugf("      Ignore DTR: %s\r\n", ignoreDTR ? "True" : "False");

  Debugln("=============================================================\n");
  nextESPcheck = millis() + 1200000;

} // showBoardInfo()


//==================================================================================================
String Format(double x, int len, int d)
{
  String r;
  int rl;

  r = String(x, d);
  //Debugf("Format(%s, %d, %d)\n", r.c_str(), len, d);
  while (r.length() < len) r = "0" + r;
  rl = r.length();
  if (rl > len)
  {
    return r.substring((rl - len));
  }
  return r;

} // Format()


//==================================================================================================
int FindCharInArrayRev(unsigned char array[], char c, int len)
{
  for (int16_t i = len - 1; i >= 0; i--)
  {
    if (array[i] == c)
    {
      return i;
    }
  }
  return -1;
}

//==================================================================================================
int16_t decodeTelegram(int len)
{
  //need to check for start
  int startChar = FindCharInArrayRev((unsigned char *)telegramLine, '/', len);
  int endChar   = FindCharInArrayRev((unsigned char *)telegramLine, '!', len);

  bool validCRCFound = false;
  if(startChar>=0)
  {
    //start found. Reset CRC calculation
    currentCRC=CRC16(0x0000, (unsigned char *) telegramLine+startChar, len-startChar);
    if(outputOnSerial)
    {
      //Serial.print("S>");
      //----for(int cnt=startChar; cnt<len-startChar;cnt++)
      //----  Serial.print(telegram[cnt]);
    }
    //Serial.println("Start found!");

  }
  else if(endChar>=0)
  {
    //add to crc calc
    currentCRC=CRC16(currentCRC, (unsigned char *)telegramLine+endChar, 1);
    //char messageCRC[5];
    //strncpy(messageCRC, telegram + endChar + 1, 4);
    //messageCRC[4]=0; //thanks to HarmOtten (issue 5)
    //if(outputOnSerial) {
    //Serial.print("E>");
    //for(int cnt=0; cnt<len;cnt++)
    //Serial.print(telegram[cnt]);
    //}

  }
  else
  {
    currentCRC=CRC16(currentCRC, (unsigned char *)telegramLine, len);
    if(outputOnSerial)
    {
      //Serial.print("->");
      //----for(int cnt=0; cnt<len;cnt++)
      //----  Serial.print(telegram[cnt]);
    }
  }

  //return validCRCFound;
  return currentCRC;

} // decodeTelegram()


//==================================================================================================
void updateTime()
{
  if (useNTPtiming)   // NTP
  {
    //actSecond = second(ntpTime);
    actMinute = minute(ntpTime);
    actHour   = hour(ntpTime);
    actDay    = day(ntpTime);
    actMonth  = month(ntpTime);
    actYear   = year(ntpTime);
    return;
  }

  if (actMinute > 59)
  {
    actMinute = 0;
    actHour++;
  }
  if (actHour >= 24)
  {
    actHour = 0;
    actDay++;
  }
  if (actDay > 30)
  {
    actDay = 1;
    actMonth++;
  }
  if (actMonth > 12)
  {
    actMonth = 1;
    actYear++;
  }
  setTime(actHour, actMinute, 1, actDay, actMonth, actYear);

} // updateTime()


//==================================================================================================
void updateMeterValues(uint8_t period)
{
  float  Factor;
  String wsString = "";

  handleFlashButton();

  switch(period)
  {
    case SMonth:
      Factor = 30.0 * 24.0;
      break;
    case SDay:
      Factor = 24.0;
      break;
    case SHour:
      Factor = 1.0;
      break;
    default:
      Factor = 1.0;
  }

  ED_T1      += (float)((random(200, 2200) / 3600000.0) * actInterval) * Factor;
  ED_T2      += (float)((random(100, 9000) / 3600000.0) * actInterval) * Factor;
  if (actHour >= 6 && actHour <= 18)
  {
    ER_T1      += (float)((random(0, 400)  / 3600000.0) * actInterval) * Factor;
    ER_T2      += (float)((random(0, 200)  / 3600000.0) * actInterval) * Factor;
    ETariffInd  = 1;
  }
  else
  {
    ETariffInd  = 2;
  }
  GDelivered += (float)(random(2, 25) / 10000.0) * Factor;    // Gas Delevered
  V_l1        = (float)(random(220, 240) * 1.01);     // Voltages
  V_l2        = (float)(random(220, 240) * 1.02);
  V_l3        = (float)(random(220, 240) * 1.03);
  C_l1        = (float)(random(1, 20) * 1.01);         // Current
  C_l2        = (float)(random(1, 15) * 1.02);
  C_l3        = (float)(random(1, 10) * 1.03);
  IPD_l1      = (float)(random(1, 1111) * 0.001102);
  IPD_l2      = (float)(random(1, 892)  * 0.001015);
  IPD_l3      = (float)(random(1, 773)  * 0.001062);
  if (actHour >= 4 && actHour <= 22)
  {
    IPR_l1    = (float)(random(1, 975)  * 0.001109);
    IPR_l2    = (float)(random(1, 754)  * 0.001031);
    IPR_l3    = (float)(random(1, 613)  * 0.001092);

  }
  else        // 's-nachts geen opwekking van energy!
  {
    IPR_l1    = 0.0;
    IPR_l2    = 0.0;
    IPR_l3    = 0.0;
  }
  PDelivered  = (float)(IPD_l1 + IPD_l2 + IPD_l3) / 1.0;       // Power Delivered
  PReceived   = (float)(IPR_l1 + IPR_l2 + IPR_l3) / 1.0;       // Power Returned

  thisSecond  = ((millis() / 1000) % 60);
  /**
  Debugf("injector: l1[%5d], l2[%5d], l3[%5d] ==> l1+l2+l3[%9.3f]\r\n\n"
                                                  , (int)(IPD_l1 * 1000)
                                                  , (int)(IPD_l2 * 1000)
                                                  , (int)(IPD_l3 * 1000)
                                                  , PDelivered);
  **/
  currentCRC = 0;

  if (String(actDSMR) == "42")
  {
    memset(telegram, 0, _MAX_TELEGRAM_LEN);
    telegramPos = 0;
    
    for (int16_t line = 0; line <= maxLines42; line++)
    {
      yield();
      memset(telegramLine, 0, _MAX_LINE_LEN);
      int16_t len = buildTelegram42(line, telegramLine);  // also: prints to DSMRsend
      calcCRC = decodeTelegram(len);
      snprintf(&telegram[telegramPos], _MAX_TELEGRAM_LEN, "%s", telegramLine);
      telegramPos += len;
    }

    if (skipChecksum)
    {
      snprintf(&telegram[telegramPos], _MAX_TELEGRAM_LEN, "\r\n\r\n\0\0\0\0\0\0");
      telegramPos += 4;
      if (Verbose && ((telegramCount % 3) == 0))
      {
        Debug("\r\n\r\n\0\0");
      }
    }
    else  
    {
      snprintf(&telegram[telegramPos], _MAX_TELEGRAM_LEN, "%04X\r\n\r\n\0\0", (calcCRC & 0xFFFF) );
      telegramPos += 8;
      if (Verbose && ((telegramCount % 3) == 0))
      {
        Debugf("%04X\r\n\r\n\0\0", (calcCRC & 0xFFFF));
      }
    }

    //--- send telegram to P1 port
    //Debugln("\r\n----- full telegram ------------");
    //for(int p=0; p<telegramPos; p++) Debug(telegram[p]);
    //Debugln("--------------------------------------------\r\n");

    for(int p=0; p<telegramPos; p++) P1_OUT.print(telegram[p]);
    P1_OUT.flush();

  }
  else if (String(actDSMR) == "50")
  {
    memset(telegram, 0, _MAX_TELEGRAM_LEN);
    telegramPos = 0;
    
    for (int16_t line = 0; line <= maxLines50; line++)
    {
      yield();
      memset(telegramLine, 0, _MAX_LINE_LEN);
      int16_t len = buildTelegram50(line, telegramLine);  // also: prints to DSMRsend
      calcCRC = decodeTelegram(len);
      snprintf(&telegram[telegramPos], _MAX_TELEGRAM_LEN, "%s", telegramLine);
      telegramPos += len;
    }

    if (skipChecksum)
    {
      //snprintf(&telegram[telegramPos-7], _MAX_TELEGRAM_LEN, "!\r\n\r\n\0\0\0\0\0\0");
      snprintf(&telegram[telegramPos], _MAX_TELEGRAM_LEN, "\r\n\r\n\0\0\0\0\0\0");
      telegramPos += 4;
      if (Verbose && ((telegramCount % 3) == 0))
      {
        Debug("\r\n\r\n\0\0");
      }
    }
    else  
    {
      //snprintf(&telegram[telegramPos-7], _MAX_TELEGRAM_LEN, "!%04X\r\n\r\n\0\0", (calcCRC & 0xFFFF) );
      snprintf(&telegram[telegramPos], _MAX_TELEGRAM_LEN, "%04X\r\n\r\n\0\0", (calcCRC & 0xFFFF) );
      telegramPos += 8;
      if (Verbose && ((telegramCount % 3) == 0))
      {
        Debugf("%04X\r\n\r\n\0\0", (calcCRC & 0xFFFF));
      }
    }

    //--- send telegram to P1 port
    //Debugln("\r\n----- full telegram ------------");
    //for(int p=0; p<telegramPos; p++) Debug(telegram[p]);
    //Debugln("--------------------------------------------\r\n");

    for(int p=0; p<telegramPos; p++) P1_OUT.print(telegram[p]);
    P1_OUT.flush();
  }
  else if (String(actDSMR) == "BE")
  {
    memset(telegram, 0, _MAX_TELEGRAM_LEN);
    telegramPos = 0;
    
    for (int16_t line = 0; line <= maxLinesBE; line++)
    {
      yield();
      memset(telegramLine, 0, _MAX_LINE_LEN);
      int16_t len = buildTelegramBE(line, telegramLine);  // also: prints to DSMRsend
      calcCRC = decodeTelegram(len);
      snprintf(&telegram[telegramPos], _MAX_TELEGRAM_LEN, "%s", telegramLine);
      telegramPos += len;
    }

    if (skipChecksum)
    {
      //snprintf(&telegram[telegramPos-7], _MAX_TELEGRAM_LEN, "!\r\n\r\n\0\0\0\0\0\0");
      snprintf(&telegram[telegramPos], _MAX_TELEGRAM_LEN, "\r\n\r\n\0\0\0\0\0\0");
      telegramPos += 4;
      if (Verbose && ((telegramCount % 3) == 0))
      {
        Debug("\r\n\r\n\0\0");
      }
    }
    else  
    {
      //snprintf(&telegram[telegramPos-7], _MAX_TELEGRAM_LEN, "!%04X\r\n\r\n\0\0", (calcCRC & 0xFFFF) );
      snprintf(&telegram[telegramPos], _MAX_TELEGRAM_LEN, "%04X\r\n\r\n\0\0", (calcCRC & 0xFFFF) );
      telegramPos += 8;
      if (Verbose && ((telegramCount % 3) == 0))
      {
        Debugf("%04X\r\n\r\n\0\0", (calcCRC & 0xFFFF));
      }
    }

    //--- send telegram to P1 port
    //Debugln("\r\n----- full telegram ------------");
    //for(int p=0; p<telegramPos; p++) Debug(telegram[p]);
    //Debugln("--------------------------------------------\r\n");

    for(int p=0; p<telegramPos; p++) P1_OUT.print(telegram[p]);
    //P1_OUT.print(telegram);
    P1_OUT.flush();

  }
  else if (String(actDSMR) == "30")
  {
    memset(telegram, 0, _MAX_TELEGRAM_LEN);
    telegramPos = 0;
    
    for (int16_t line = 0; line <= maxLines30; line++)
    {
      yield();
      memset(telegramLine, 0, _MAX_LINE_LEN);
      int16_t len = buildTelegram30(line, telegramLine);  
      snprintf(&telegram[telegramPos], _MAX_TELEGRAM_LEN, "%s", telegramLine);
      telegramPos += len;
    }

    //--- send telegram to P1 port
    for(int p=0; p<telegramPos; p++) P1_OUT.print(telegram[p]);
    P1_OUT.flush();

  }
  else     // from file!!!
  {
    Debugf("Telegram %d from file ...\r\n", ++recCount);
    readTelegramFromFile(telegramFileName);
  }
  P1_OUT.flush();
  DebugFlush();
  telegramCount++;

} // updateMeterValues()


//==================================================================================================
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(_SIGNAL_LED, OUTPUT);
  for (int i=0; i<20; i++)
  {
    digitalWrite(_SIGNAL_LED, digitalRead(LED_BUILTIN));
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(200);
  }
  Serial.begin(115200);
  while(!Serial) { delay(10); }
  Serial.println("\r\nStarting up ....\r\n");

  oled_Init();
  oled_Clear();  // clear the screen so we can paint the menu.

  lastReset     = ESP.getResetReason();

  pinMode(_DATA_REGUEST, INPUT);
  pinMode(_FLASH_BUTTON, INPUT);

  TelnetStream.begin();
  TelnetStream.flush();
  oledPrintLine("*DSMRinjector2*");
  if (lastReset != "")
  {
    oledPrintLine(lastReset.substring(0, 16));
    oledPrintLine(lastReset.substring(16));
  }
  oledPrintLine("telnet (poort 23)");

  DebugTln("\nStarting ....\n");
  DebugTln("\n=======================");

  //oledPrintMsg(0, " <DSMRinjector2>", 0);
  int8_t sPos = String(_FW_VERSION).indexOf(' ');
  snprintf(cMsg, sizeof(cMsg), "(c)2023 [%s]", String(_FW_VERSION).substring(0, sPos).c_str());
  oledPrintLine(String(cMsg));
  oledPrintLine("Willem Aandewiel");
  yield();

  DebugTf("Connect to WiFi as [%s]\n", String(_HOSTNAME).c_str());
  oledPrintLine("Setup WiFi as:");
  oledPrintLine(String(_HOSTNAME));
  digitalWrite(LED_BUILTIN, HIGH);

  startWiFi(_HOSTNAME);
  for (int i=0; i<10; i++)
  {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(400);
  }

  // WiFi connexion is OK
  Debugln ( "" );
  //Serial.print ( "Connected to " ); Serial.println ( WiFi.SSID() );
  DebugT ( "Connected to " );
  Debugln ( WiFi.SSID() );
  oledPrintLine(WiFi.SSID());

  //Serial.print ( "IP address: " );  Serial.println ( WiFi.localIP() );
  DebugT ( "IP address: " );
  Debugln ( WiFi.localIP() );
  oledPrintLine(WiFi.localIP().toString());

  if (MDNS.begin(_HOSTNAME))                // Start the mDNS responder for DSMRinjector2.local
  {
    DebugTln("mDNS responder started");
  }
  else
  {
    DebugTln("Error setting up MDNS responder!");
  }
  //--- webSockets -------
  MDNS.addService("arduino", "tcp", 81);
  MDNS.port(81);

  //--------------------------------------------------------------------------
  oledPrintLine("Setup NTP..");
  if (!startNTP())
  {
    DebugTln(F("ERROR!!! No NTP server reached!\r\n\r"));
    if (settingOledType > 0)
    {
      oledPrintLine("geen reactie van");
      oledPrintLine("NTP server's");
      oledPrintLine("Reboot DSMR-logger");
    }
    delay(2000);
    ESP.restart();
    delay(3000);
  }

  setSyncProvider(getNtpTime);
  snprintf(cMsg, sizeof(cMsg), "%02d-%02d-%02d %02d:%02d:%02d (%s)"
           , year(ntpTime), month(ntpTime), day(ntpTime)
           , hour(ntpTime), minute(ntpTime), second(ntpTime)
           , DSTactive ? "CEST":"CET");
  DebugTf("NTP time is [%s]\r\n", cMsg);
  oledPrintLine("NTP service ok!");
  oledPrintLine(cMsg);
  //--------------------------------------------------------------------------


  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  nextTelegram = millis() + 2500;
  nextMinute   = millis();

  // --- initial values
  ED_T1       = 6230.1;     // Energy Delivered
  ED_T2       = 8614.8;
  ER_T1       = 1183.1;     // Energy Returned
  ER_T2       = 2601.1;
  V_l1        = 230.0;      // Voltages
  V_l2        = 231.1;
  V_l3        = 232.2;
  C_l1        = 1;          // Current
  C_l2        = 2;
  C_l3        = 3;
  IPD_l1      = 0.0;
  IPD_l2      = 0.0;
  IPD_l3      = 0.0;
  IPR_l1      = 0.0;
  IPR_l2      = 0.0;
  IPR_l3      = 0.0;
  PDelivered  = 0.0;         // Power Delivered
  PReceived   = 0.0;         // Power Returned
  GDelivered  = 4770.1;      // Gas Delevered
  //actYear     = 2014;
  //actMonth    = 1;
  //actDay      = 1;
  //actHour     = 0;
  //actMinute   = 1;
  actSec      = 1;
  actSpeed    = 5;
  actGasMBus  = 1;
  //setTime(actHour, actMinute, actSec, actDay, actMonth, actYear);
  sprintf(actDSMR, "50");
  sprintf(savDSMR, "50");
  nextGuiUpdate = millis() + 1;

  showBoardInfo();

  randomSeed(analogRead(0));

  Serial.flush();
  DebugFlush();

  //  httpUpdater.setup(&httpServer);

  Serial.println("\r\nLittleFS.begin() ..");
  Debugln("\r\nLittleFS.begin() ..");
  oledPrintLine("Start littleFS ..");
  setupFS();

  readSettings(true);

  httpServer.on("/", HTTP_POST, callIndex_html);
  //httpServer.on("/ReBoot", HTTP_POST, handleReBoot);
  httpServer.serveStatic("/FSmanager",     LittleFS, "/littleFSmanager.html");
  httpServer.serveStatic("/FSmanager.png", LittleFS, "/littleFSmanager.png");

  httpServer.begin();
  Serial.println( "HTTP server started" );
  oledPrintLine("HTTP Server Started!");
  for (int i = 0; i< 10; i++)
  {
    digitalWrite(_SIGNAL_LED, !digitalRead(LED_BUILTIN));
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(100);
  }

  Serial.println("\nAnd now it begins ....");

  Serial.printf("Data Request Pin is %s\r\n", digitalRead(_DATA_REGUEST) ? "High":"Low");
  Serial.printf("Setting: Ignore DTR Pin is %s\r\n", ignoreDTR ? "True":"False");

  Serial.println("\r\n\n=================================");
  Serial.println(      "Further debugging only on Telnet!");
  Serial.println(      "=================================\r\n\n\n");
  Serial.flush();
  Debugln("\nAnd now it begins ....");
  Debugln("\r\n\n=================================");
  Debugln(      "Further debugging only on Telnet!");
  Debugln(      "=================================\r\n\n\n");
  DebugFlush();
  
  oled_Clear();  // clear the screen so we can paint the menu.
  sprintf(cMsg, "Protocol %s", actDSMR);
  Serial.println(cMsg);
  oledPrintMsg(0, cMsg, 0);
  if (runStatus)  oledPrintMsg(1, "Status: Running", 0);
  else            oledPrintMsg(1, "Status: Stopped", 0);
  sprintf(cMsg, "Send: %8u", telegramCount);
  Serial.println(cMsg);
  oledPrintMsg(3, cMsg, 10);

  digitalWrite(_SIGNAL_LED, HIGH);  //-- inversed
  delay(500);
  //-- does not work with optocoupler (pulled low) 
  //--  UART0 TX is now GPIO02!!
  //-- Serial.set_tx(2); 
  //-- reset any previous swap() - TX is "GPIO01", RX is "GPIO03"
  P1_OUT.begin(115200);  
  while(!P1_OUT) { delay(10); }
  //-- set TX to "GPIO15" [D8]  RX to "GPIO13" [D7]
  P1_OUT.swap();  

} // setup()


//==================================================================================================
void loop()
//==================================================================================================
{
  httpServer.handleClient();
  webSocket.loop();
  MDNS.update();
  handleKeyInput();

  handleFlashButton();


  if (millis() > DtrTimer)
  {
    //-- inversed, so "Off"
    digitalWrite(LED_BUILTIN, HIGH);
  }

  if (millis() > nextESPcheck)
  {
    showBoardInfo();
  }

  if (millis() > nextGuiUpdate)
  {
    updateGUI();
  }

  if (millis() > signalLedTimer)
  {
    digitalWrite(_SIGNAL_LED, LOW);  
  }


  //if (digitalRead(_DATA_REGUEST))
  //      digitalWrite(LED_BUILTIN, HIGH);
  //else  digitalWrite(LED_BUILTIN, LOW);

    //-- FS selected, but no file to use 
    if (!strcmp(actDSMR, "FS") && strlen(telegramFileName) == 0)
    {
      sprintf(actDSMR, "50");
    }
    if (String(actDSMR) != String(savDSMR))
    {
      telegramFile.close(); // closing!
      Debugln("Serial.end()..");
      DebugFlush();
      Serial.end();
      delay(200);
      
      oled_Clear();  // clear the screen so we can paint the menu.
      sprintf(cMsg, "Protocol %s", actDSMR);
      Serial.println(cMsg);
      oledPrintMsg(0, cMsg, 0);

      if (String(actDSMR) == "30")
      {
        sprintf(savDSMR, "30");
        P1_OUT.begin(9600, SERIAL_7E1);
        while(!P1_OUT) { delay(10); }
        P1_OUT.swap();
        Debugln("P1_OUT.begin(9600, SERIAL_7E1)");
        delay(200);
      }
      else if (String(actDSMR) == "42")
      {
        sprintf(savDSMR, "42");
        P1_OUT.begin(115200);
        while(!P1_OUT) { delay(10); }
        P1_OUT.swap();
        Debugln("P1_OUT.begin(115200)");
        DebugFlush();
        delay(200);
      }
      else if (String(actDSMR) == "BE")
      {
        sprintf(savDSMR, "BE");
        P1_OUT.begin(115200);
        while(!P1_OUT) { delay(10); }
        P1_OUT.swap();
        Debugln("P1_OUT.begin(115200)");
        DebugFlush();
        delay(200);
      }
      else if (String(actDSMR) == "FS")
      {
        sprintf(savDSMR, "FS");
        P1_OUT.begin(115200);
        while(!P1_OUT) { delay(10); }
        P1_OUT.swap();
        Debugln("P1_OUT.begin(115200)");
        DebugFlush();
        delay(200);
        Serial.println("-> Select File!");
        Debugln("-> Select File!");
        oledPrintLine("-> From FILE!");
        telegramFile = FSYS.open(telegramFileName, "r"); // open for reading
        if (!telegramFile)
        {
          Debugf("open(%s, 'r') FAILED!!! --> Bailout\r\n", telegramFileName);
          return;
        }

      }
      else
      {
        sprintf(savDSMR, "50");
        P1_OUT.begin(115200);
        while(!P1_OUT) { delay(10); }
        P1_OUT.swap();
        Debugln("P1_OUT.begin(115200)");
        DebugFlush();
        delay(200);
      }
    }

  //-- not running, so nothing to do
  if (runStatus)  oledPrintMsg(1, "Status: Running", 0);
  else            oledPrintMsg(1, "Status: Stopped", 0);
  if (runStatus == 0) return; 

  if (digitalRead(_DATA_REGUEST) || ignoreDTR)
  {
    if (millis() > DtrTimer)
    {
      //-- inversed (On)
      digitalWrite(LED_BUILTIN, LOW); 
      DtrTimer = millis() + 400;
    }
  }
  else
  {
    //-- inversed (Off)
    digitalWrite(LED_BUILTIN, HIGH);
    return;
  }

  if (millis() < nextTelegram)  return;   // not yet time for new Telegram

  //-- only show 3 telegrams in Verbode mode
  //Debugf("verboseCount [%d]\r\n", verboseCount);
  if (verboseCount > 6) 
  {
    if (Verbose) Debugln("Stop showing telegrams");
    Verbose = false;
  }
  digitalWrite(_SIGNAL_LED, HIGH); 
  signalLedTimer = millis()+150;

  switch(runMode)
  {
    case SInit: // --- start date/time
      DebugTln("runMode [SInit]");
      actYear   = year();
      actMonth  = month();
      actDay    = day();
      actHour   = hour();
      actMinute = minute();
      actSec    = 1;
      setTime(actHour, actMinute, 1, actDay, actMonth, actYear);
      showBoardInfo();
      nextTelegram = millis() + (actInterval * 1000);
      runMode = SNormal;
      break;

    case SMonth: // per Maand
      //DebugTln("runMode [SMonth]");
      actSec  = 5;
      for (int m = 1; m <= 3; m++)
      {
        updateTime();
        updateMeterValues(SMonth);
        nextTelegram = millis() + 1000;
        while ((millis() < nextTelegram) && (runMode == SMonth))
        {
          httpServer.handleClient();
          webSocket.loop();
          yield();
          handleKeyInput();
        }
        actSec += 5;
        updateGUI();
      }
      actMonth++;
      actMinute++;
      updateTime();
      showBoardInfo();
      nextTelegram = millis() + (actInterval * 1000);
      break;

    case SDay: // per Dag
      //DebugTln("runMode [SDay]");
      actSec  = 11;
      for (int d = 1; d <= 3; d++)
      {
        updateTime();
        updateMeterValues(SDay);
        nextTelegram = millis() + 1000;
        while ((millis() < nextTelegram) && (runMode == SDay))
        {
          httpServer.handleClient();
          webSocket.loop();
          yield();
          handleKeyInput();
        }
        actSec += 5;
        updateGUI();
      }
      actDay++;
      updateTime();
      nextTelegram = millis() + (actInterval * 1000);
      break;

    case SHour: // init
      //DebugTln("runMode [SHour]");
      actSec = 23;
      for (int h = 1; h <= 3; h++)
      {
        updateTime();
        updateMeterValues(SHour);
        nextTelegram = millis() + 1000;
        while ((millis() < nextTelegram) && (runMode == SHour))
        {
          httpServer.handleClient();
          webSocket.loop();
          yield();
          handleKeyInput();
        }
        actSec += 5;
        updateGUI();
      }
      nextTelegram = millis() + (actInterval * 1000);
      actHour++;
      updateTime();
      break;

    case SNormal:
    default:
      actMinute += actSpeed;
      updateTime();
      updateGUI();
      updateMeterValues(SNormal);
      nextTelegram = millis() + (actInterval * 1000);

  } // switch(runMode)

  if (Verbose)
  {
    verboseCount++;
  }

  if (lastReset != "")
  {
    oledPrintMsg(2, lastReset, 0);
  }

  if (millis()+_SHOW_BTN_TIME > showBtnTimer)
  {
    //oledPrintMsg(1, "                       ", 10);
  }
  //-- time to remove last reset message
  if (telegramCount > 10)
  {
    lastReset = "                           ";
  }
  sprintf(cMsg, "Send: %8u", telegramCount);
  oledPrintMsg(3, cMsg, 10);


} // loop()
