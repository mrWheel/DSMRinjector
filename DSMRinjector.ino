/*
** DSMRinjector
*/
#define _FW_VERSION "v4 (06-03-2020)"
/* 
*   Arduino-IDE settings for ESP01 (black):

    - Board: "Generic ESP8266 Module"
    - Flash mode: "DOUT"
    - Flash size: "1M (no SPIFFS)"
    - Debug port: "Disabled"
    - Debug Level: "None"
    - IwIP Variant: "v2 Lower Memory"
    - Reset Method: "nodemcu"   // but will depend on the programmer!
    - Crystal Frequency: "26 MHz" 
    - VTables: "Flash"
    - Flash Frequency: "40MHz"
    - CPU Frequency: "80 MHz"
    - Buildin Led: "1"  // GPIO01 - Pin 2
    - Upload Speed: "115200"
    - Erase Flash: "Only Sketch"
    - Port: "?"

*/

#include "DSMRinjector.h"

//===========================================================================================
void callIndex_html() {
//===========================================================================================
  //Serial.println("uri() => / !");
  Debugln("uri() => / !");
  httpServer.send(200, "text/html", DSMRindex_html );

} // callIndex_html()


//===========================================================================================
void handleReBoot() {
//===========================================================================================
  String redirectHTML = "";

  redirectHTML += "<!DOCTYPE HTML><html lang='en-US'>";
  redirectHTML += "<head>";
  redirectHTML += "<meta charset='UTF-8'>";
  redirectHTML += "<style type='text/css'>";
  redirectHTML += "body {background-color: lightblue;}";
  redirectHTML += "</style>";
  redirectHTML += "<title>Redirect to DSMRinjector</title>";
  redirectHTML += "</head>";
  redirectHTML += "<body><h1>DSMRinjector</h1>";
  redirectHTML += "<h3>Rebooting DSMRinjector</h3>";
  redirectHTML += "<br><div style='width: 500px; position: relative; font-size: 25px;'>";
  redirectHTML += "  <div style='float: left;'>Redirect over &nbsp;</div>";
  redirectHTML += "  <div style='float: left;' id='counter'>20</div>";
  redirectHTML += "  <div style='float: left;'>&nbsp; seconden ...</div>";
  redirectHTML += "  <div style='float: right;'>&nbsp;</div>";
  redirectHTML += "</div>";
  redirectHTML += "<!-- Note: don't tell people to `click` the link, just tell them that it is a link. -->";
  redirectHTML += "<br><br><hr>If you are not redirected automatically, click this <a href='/'>DSMRinjector</a>.";
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
  
  Debugln("ReBoot DSMRinjector ..");
  TelnetStream.flush();
  delay(1000);
  ESP.reset();
  
} // handleReBoot()


//===========================================================================================
void reloadPage(String goTo) {
//===========================================================================================
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
int8_t splitString(String inStrng, char delimiter, String wOut[], uint8_t maxWords) {
//=======================================================================
  uint16_t inxS = 0, inxE = 0, wordCount = 0;
    inStrng.trim();
    while(inxE < inStrng.length() && wordCount < maxWords) {
      inxE  = inStrng.indexOf(delimiter, inxS);         //finds location of first ,
      wOut[wordCount] = inStrng.substring(inxS, inxE);  //captures first data String
      wOut[wordCount].trim();
      //DebugTf("[%d] => [%c] @[%d] found[%s]\n", wordCount, delimiter, inxE, wOut[wordCount].c_str());
      inxS = inxE;
      inxS++;
      wordCount++;
    }
    if (inxS < inStrng.length()) {
      wOut[wordCount] = inStrng.substring(inxS, inStrng.length());  //captures first data String      
      //DebugTf("[%d] rest => [%s]\n", wordCount, wOut[wordCount].c_str());
    }

    return wordCount;
    
} // splitString()


//==================================================================================================
void checkESP8266() {
//==================================================================================================

  uint32_t    realSize = ESP.getFlashChipRealSize();
  uint32_t    ideSize  = ESP.getFlashChipSize();
  FlashMode_t ideMode  = ESP.getFlashChipMode();

  Debugln("\n=============================================================");

  Debugf("   Flash real id: %08X\n", ESP.getFlashChipId());
  Debugf(" Flash real size: %u bytes\n", realSize);
  Debugf("  Flash ide size: %u bytes\n", ideSize);

  if (ideSize != realSize) {
    Debugln(">> Flash Chip configuration wrong! <<");
  } else {
    Debugln("Flash Chip configuration ok.");
  }
  Debugf(" Flash ide speed: %u Hz\n", ESP.getFlashChipSpeed());
  Debugf("  Flash ide mode: %s\n", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN"));

  Debugf("        Run Mode: %d", runMode);
  switch(runMode) {
    case 0:   Debugln(" (Initialize)");       break;
    case 1:   Debugln(" (Vul Maanden)");      break;
    case 2:   Debugln(" (Vul Dagen)");        break;
    case 3:   Debugln(" (Vul Uren)");         break;
    case 4:   Debugln(" (Normal Operation)"); break;
    default:  Debugln(" (Unknown..)");
  }

  Debugf("Minuten per stap: %d seconden\n", actSpeed);
  Debugf("        Interval: %d seconden\n", actInterval);

  Debugf("  DSMR standaard: %s\n", actDSMR);
  Debugf("      Run Status: %d", runStatus);
  switch(runStatus) {
    case 0:   Debugln(" (geStopt)");    break;
    case 1:   Debugln(" (Running)");    break;
    default:  Debugln(" (Unknown..)");
  }

  Debugln("=============================================================\n");
  nextESPcheck = millis() + 1200000;

} // checkESP8266()


//==================================================================================================
String Format(double x, int len, int d) {
//==================================================================================================
  String r;
  int rl;
  
  r = String(x, d);
//Debugf("Format(%s, %d, %d)\n", r.c_str(), len, d);
  while (r.length() < len) r = "0" + r;
  rl = r.length();
  if (rl > len) {
    return r.substring((rl - len));
  }
  return r;

} // Format()


//==================================================================================================
int FindCharInArrayRev(unsigned char array[], char c, int len) {
//==================================================================================================
  for (int16_t i = len - 1; i >= 0; i--) {
    if (array[i] == c) {
      return i;
    }
  }
  return -1;
}

//==================================================================================================
int16_t decodeTelegram(int len) {
//==================================================================================================
  //need to check for start
  int startChar = FindCharInArrayRev((unsigned char*)telegram, '/', len);
  int endChar   = FindCharInArrayRev((unsigned char*)telegram, '!', len);
  
  bool validCRCFound = false;
  if(startChar>=0) {
    //start found. Reset CRC calculation
    currentCRC=CRC16(0x0000,(unsigned char *) telegram+startChar, len-startChar);
    if(outputOnSerial) {
      //Serial.print("S>");
      //----for(int cnt=startChar; cnt<len-startChar;cnt++)
      //----  Serial.print(telegram[cnt]);
    }    
    //Serial.println("Start found!");
    
  } else if(endChar>=0) {
    //add to crc calc 
    currentCRC=CRC16(currentCRC,(unsigned char*)telegram+endChar, 1);
    //char messageCRC[5];
    //strncpy(messageCRC, telegram + endChar + 1, 4);
    //messageCRC[4]=0; //thanks to HarmOtten (issue 5)
    //if(outputOnSerial) {
      //Serial.print("E>");
      //for(int cnt=0; cnt<len;cnt++)
        //Serial.print(telegram[cnt]);
    //}   
  
  } else {
    currentCRC=CRC16(currentCRC, (unsigned char*)telegram, len);
    if(outputOnSerial) {
      //Serial.print("->");
      //----for(int cnt=0; cnt<len;cnt++)
      //----  Serial.print(telegram[cnt]);
    }
  }

  //return validCRCFound;
  return currentCRC;
  
} // decodeTelegram()


//==================================================================================================
void updateTime() {
//==================================================================================================
  if (actMinute > 59) {
    actMinute = 0;
    actHour++;
  }
  if (actHour >= 24) {
    actHour = 0;
    actDay++;
  }
  if (actDay > 30) {
    actDay = 1;
    actMonth++;
  }
  if (actMonth > 12) {
    actMonth = 1;
    actYear++;
  }
  setTime(actHour, actMinute, 1, actDay, actMonth, actYear);

} // updateTime()


//==================================================================================================
void updateMeterValues(uint8_t period) {
//==================================================================================================
  float  Factor;
  String wsString = "";
  
  switch(period) {
    case SMonth:  Factor = 30.0 * 24.0; break;
    case SDay:    Factor = 24.0;        break;
    case SHour:   Factor = 1.0;         break;
    default:      Factor = 1.0;
  }

  ED_T1      += (float)((random(200,2200) / 3600000.0) * actInterval) * Factor;
  ED_T2      += (float)((random(100,9000) / 3600000.0) * actInterval) * Factor;
  if (actHour >= 6 && actHour <= 18) {
    ER_T1      += (float)((random(0,400)  / 3600000.0) * actInterval) * Factor;
    ER_T2      += (float)((random(0,200)  / 3600000.0) * actInterval) * Factor;
    ETariffInd  = 1;
  } else {
    ETariffInd  = 2;
  }
  GDelivered += (float)(random(2,25) / 10000.0) * Factor;     // Gas Delevered
  V_l1        = (float)(random(220,240) * 1.01);      // Voltages
  V_l2        = (float)(random(220,240) * 1.02);
  V_l3        = (float)(random(220,240) * 1.03);
  C_l1        = (float)(random(1,20) * 1.01);          // Current
  C_l2        = (float)(random(1,15) * 1.02);
  C_l3        = (float)(random(1,10) * 1.03);   
  IPD_l1      = (float)(random(1,1111) * 0.001102);
  IPD_l2      = (float)(random(1,892)  * 0.001015);
  IPD_l3      = (float)(random(1,773)  * 0.001062);
  if (actHour >= 4 && actHour <= 22) {
    IPR_l1    = (float)(random(1,975)  * 0.001109);
    IPR_l2    = (float)(random(1,754)  * 0.001031);
    IPR_l3    = (float)(random(1,613)  * 0.001092);
    
  } else {    // 's-nachts geen opwekking van energy!
    IPR_l1    = 0.0;
    IPR_l2    = 0.0;
    IPR_l3    = 0.0;
  }
  PDelivered  = (float)(IPD_l1 + IPD_l2 + IPD_l3) / 1.0;       // Power Delivered
  PReceived   = (float)(IPR_l1 + IPR_l2 + IPR_l3) / 1.0;       // Power Returned

  Debugf("l1[%5d], l2[%5d], l3[%5d] ==> l1+l2+l3[%9.3f]\n", (int)(IPD_l1 * 1000)
                                                          , (int)(IPD_l2 * 1000)
                                                          , (int)(IPD_l3 * 1000)
                                                          , PDelivered);

  currentCRC = 0;
  if (String(actDSMR) == "40") {
    for (int16_t line = 0; line <= maxLines40; line++) {
      yield();
      int16_t len = buildTelegram40(line, telegram);  // also: prints to DSMRsend
      calcCRC = decodeTelegram(len);
    } 
    Serial.printf("!%04X\r\n\r\n", (calcCRC & 0xFFFF));
    if (Verbose) Debugf("!%04X\r\n\r\n", (calcCRC & 0xFFFF));

  } else if (String(actDSMR) == "BE") {
    for (int16_t line = 0; line <= maxLinesBE; line++) {
      yield();
      int16_t len = buildTelegramBE(line, telegram);  // also: prints to DSMRsend
      calcCRC = decodeTelegram(len);
    } 
    Serial.printf("!%04X\r\n\r\n", (calcCRC & 0xFFFF));
    if (Verbose) Debugf("!%04X\r\n\r\n", (calcCRC & 0xFFFF));

  } else {
    //for (int16_t line = 0; line < 20; line++) {
    for (int16_t line = 0; line <= maxLines30; line++) {
      yield();
      int16_t len = buildTelegram30(line, telegram);  // also: prints to DSMRsend
//    calcCRC = decodeTelegram(len);  // why??
    }
    Serial.printf("!\r\n");
    if (Verbose) Debugf("!\r\n");
  }
  Serial.flush();
  DebugFlush();
  telegramCount++;
  
} // updateMeterValues()


//==================================================================================================
void setup() {
//==================================================================================================

  pinMode(LED_BUILTIN, OUTPUT);
  for (int i=0; i<20; i++) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(200);
  }
  Serial.begin(115200);

  TelnetStream.begin();
  TelnetStream.flush();

  //Serial.println("\nStarting ....\n");
  DebugTln("\nStarting ....\n");
  //Serial.println("\n=======================");
  DebugTln("\n=======================");

  DebugTf("Connect to WiFi as [%s]\n", String(_HOSTNAME).c_str());
  digitalWrite(LED_BUILTIN, HIGH);
  startWiFi(_HOSTNAME);
  for (int i=0; i<10; i++) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(400);
  }

  // WiFi connexion is OK
  Debugln ( "" );
  //Serial.print ( "Connected to " ); Serial.println ( WiFi.SSID() );
  DebugT ( "Connected to " ); Debugln ( WiFi.SSID() );
  //Serial.print ( "IP address: " );  Serial.println ( WiFi.localIP() );
  DebugT ( "IP address: " );  Debugln ( WiFi.localIP() );

  if (MDNS.begin(_HOSTNAME)) {              // Start the mDNS responder for DSMRinjector.local
    //Serial.println("mDNS responder started");
    DebugTln("mDNS responder started");
  } else {
    //Serial.println("Error setting up MDNS responder!");
    DebugTln("Error setting up MDNS responder!");
  }
  //--- webSockets -------
  MDNS.addService("arduino", "tcp", 81);
  MDNS.port(81);  

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  nextTelegram = millis() + 2500;
  nextMinute   = millis();

  // --- initial values
  ED_T1       = 145.67;   // Energy Delivered
  ED_T2       = 67.89;
  ER_T1       = 111.1;     // Energy Returned
  ER_T2       = 75.5;
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
  GDelivered  = 100.001;     // Gas Delevered
  actYear     = 2014;
  actMonth    = 1;
  actDay      = 1;
  actHour     = 0;
  actMinute   = 1;
  actSec      = 1;
  actSpeed    = 5;
  setTime(actHour, actMinute, actSec, actDay, actMonth, actYear);
  sprintf(actDSMR, "40");
  sprintf(savDSMR, "40");
  nextGuiUpdate = millis() + 1;
  
  checkESP8266();

  randomSeed(analogRead(0));
  
  Serial.flush();
  DebugFlush();

//  httpUpdater.setup(&httpServer);

  httpServer.on("/", HTTP_POST, callIndex_html);
  httpServer.on("/ReBoot", HTTP_POST, handleReBoot);

  httpServer.onNotFound([]() {
    //DebugTln("============================================================");
    //DebugTf("onNotFound(%s)\n", httpServer.uri().c_str());
    //DebugTln("============================================================");
    if (httpServer.uri() == "/update") {
      DebugTf("onNotFound(%s): ==> [/update]\n", httpServer.uri().c_str());
      httpServer.send(200, "text/html", "/update" );
      
    } else if (httpServer.uri() == "/") {
      //DebugTf("onNotFound(%s) ==> [/]\n", httpServer.uri().c_str());
      httpServer.send(200, "text/html", DSMRindex_html );
      reloadPage("/");
  //} else {
  //  DebugTf("onNotFound(%s) ==> [???] do nothing ..\n", httpServer.uri().c_str());
      
    }
  });
  
  httpServer.begin();
  Serial.println( "HTTP server started" );
  for (int i = 0; i< 10; i++) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(200);
  }

  Serial.println("\nAnd now it begins ....");
  Debugln("\nAnd now it begins ....");

} // setup()


//==================================================================================================
void loop() {
//==================================================================================================
  httpServer.handleClient();
  webSocket.loop();
  MDNS.update();
  handleKeyInput();

  if (millis() > nextESPcheck) {
    checkESP8266();
  }

  if (millis() > nextGuiUpdate) {
    updateGUI();
  }
  
  if (runStatus == 0) {
    if (String(actDSMR) != String(savDSMR)) {
      if (String(actDSMR) == "30") {
        sprintf(savDSMR, "30");
        Serial.end(); 
        delay(200);
        Serial.begin(9600, SERIAL_7E1);
      //Serial.begin(9600);
        delay(200);
      } else if (String(actDSMR) == "BE") {
        sprintf(savDSMR, "BE");
        Serial.end(); 
        delay(200);
        Serial.begin(115200);
        delay(200);
      } else {
        sprintf(savDSMR, "40");
        Serial.end(); 
        delay(200);
        Serial.begin(115200);
        delay(200);
      }
    }
    return;   // Stopped, nothing to do!
  }

  if (millis() < nextTelegram)  return;   // not yet time for new Telegram
  
  switch(runMode) {
    case SInit: // --- start date/time
                DebugTln("runMode [SInit]");
                actYear   = year();
                actMonth  = month();
                actDay    = day();
                actHour   = hour();
                actMinute = minute();
                actSec    = 1;
                setTime(actHour, actMinute, 1, actDay, actMonth, actYear);
                checkESP8266();
                nextTelegram = millis() + (actInterval * 1000);
                runMode = SNormal;
                break;
                
    case SMonth: // per Maand
                //DebugTln("runMode [SMonth]");
                actSec  = 5;
                for (int m = 1; m <= 3; m++) {
                    updateTime();
                    updateMeterValues(SMonth);
                    nextTelegram = millis() + 1000;
                    while ((millis() < nextTelegram) && (runMode == SMonth)) {
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
                checkESP8266();
                nextTelegram = millis() + (actInterval * 1000);
                break;
                
    case SDay: // per Dag
                //DebugTln("runMode [SDay]");
                actSec  = 11;
                for (int d = 1; d <= 3; d++) {
                  updateTime();
                  updateMeterValues(SDay);
                  nextTelegram = millis() + 1000;
                  while ((millis() < nextTelegram) && (runMode == SDay)) {
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
                for (int h = 1; h <= 3; h++) {
                  updateTime();
                  updateMeterValues(SHour);
                  nextTelegram = millis() + 1000;
                  while ((millis() < nextTelegram) && (runMode == SHour)) {
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
    default:    actMinute += actSpeed;
                updateTime();
                updateGUI();
                updateMeterValues(SNormal);
                nextTelegram = millis() + (actInterval * 1000);

  } // switch(runMode)

  
} // loop()
