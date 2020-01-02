/*
** DSMRinjector
*/
#define _FW_VERSION "v3 (31-07-2019)"
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

#include "CRC16.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <TimeLib.h>  // https://github.com/PaulStoffregen/Time
#include <TelnetStream.h>
#include <ESP8266mDNS.h>
#include <WebSocketsServer.h>   // Version 20.05.2015 - https://github.com/Links2004/arduinoWebSockets
#include "macros.h"
#include "index_html.h"

#define MAXLINELENGTH 128 // longest normal line is 47 char (+3 for \r\n\0)
#define _SSID         "YOUR_SSID"       // WiFi SSID
#define _PASSWORD     "YOUR_WIFI_PASSWORD"      // WiFi PASSWORD
#define _HOSTNAME     "DSMRinjector"

ESP8266WebServer        httpServer (80);
ESP8266HTTPUpdateServer httpUpdater(true);
WebSocketsServer webSocket = WebSocketsServer(81);

enum { SInit, SMonth, SDay, SHour, SNormal };

uint8_t runMode   = 0;
uint8_t runStatus = 0;

char telegram[MAXLINELENGTH];


uint16_t    currentCRC; 
int16_t     calcCRC;
const bool  outputOnSerial = true;
uint32_t    nextTelegram = 0, actInterval = 5, nextMinute = 0, nextESPcheck = 0, nextGuiUpdate;
uint32_t    telegramCount = 0;
int8_t      State;
int16_t     actSec, actMinute, actHour, actDay, actMonth, actYear, actSpeed;
char        actDSMR[3], savDSMR[3];
double      ED_T1=0, ED_T2=0, ER_T1=0, ER_T2=0, V_l1=0, V_l2=0, V_l3=0, C_l1=0, C_l2=0, C_l3=0;
uint8_t     ETariffInd=1;
float       PDelivered, PReceived;
float       IPD_l1, IPD_l2, IPD_l3, IPR_l1, IPR_l2, IPR_l3;
float       GDelivered;
bool        Verbose = false;
char        cTimeStamp[25];

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
  redirectHTML += "<title>Redirect to DSMR-injector</title>";
  redirectHTML += "</head>";
  redirectHTML += "<body><h1>DSMR-injector</h1>";
  redirectHTML += "<h3>Rebooting DSMR-injector</h3>";
  redirectHTML += "<br><div style='width: 500px; position: relative; font-size: 25px;'>";
  redirectHTML += "  <div style='float: left;'>Redirect over &nbsp;</div>";
  redirectHTML += "  <div style='float: left;' id='counter'>20</div>";
  redirectHTML += "  <div style='float: left;'>&nbsp; seconden ...</div>";
  redirectHTML += "  <div style='float: right;'>&nbsp;</div>";
  redirectHTML += "</div>";
  redirectHTML += "<!-- Note: don't tell people to `click` the link, just tell them that it is a link. -->";
  redirectHTML += "<br><br><hr>If you are not redirected automatically, click this <a href='/'>DSMR-logger</a>.";
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
  
  Debugln("ReBoot DSMR-logger ..");
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
      _dThis = true;
      //Debugf("[%d] => [%c] @[%d] found[%s]\n", wordCount, delimiter, inxE, wOut[wordCount].c_str());
      inxS = inxE;
      inxS++;
      wordCount++;
    }
    if (inxS < inStrng.length()) {
      wOut[wordCount] = inStrng.substring(inxS, inStrng.length());  //captures first data String      
      _dThis = true;
      //Debugf("[%d] rest => [%s]\n", wordCount, wOut[wordCount].c_str());
    }

    return wordCount;
    
} // splitString()


//==================================================================================================
void checkESP8266() {
//==================================================================================================

  uint32_t    realSize = ESP.getFlashChipRealSize();
  uint32_t    ideSize = ESP.getFlashChipSize();
  FlashMode_t ideMode = ESP.getFlashChipMode();

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
int16_t buildTelegram40(int16_t line, char telegram[]) {
//==================================================================================================
  int16_t len = 0;

  float val;

  switch (line) {
                                //XMX5LGBBLB2410065887
  	case 0:   sprintf(telegram, "/XMX5LGBBLB2410065887\r\n");
						  break;
  	case 1:   sprintf(telegram, "\r\n");		
  	          break;
  	case 2:   sprintf(telegram, "1-3:0.2.8(50)\r\n");
						  break;
  	case 3:   sprintf(telegram, "0-0:1.0.0(%02d%02d%02d%02d%02d%02dS)\r\n", (year() - 2000), month(), day(), hour(), minute(), second());
						  break;
  	case 4:   sprintf(telegram, "0-0:96.1.1(4530303336303000000000000000000040)\r\n", val);
		          break;
    case 5:   // Energy Delivered
  	          sprintf(telegram, "1-0:1.8.1(%s*kWh)\r\n", Format(ED_T1, 10, 3).c_str());
		          break;
    case 6:   sprintf(telegram, "1-0:1.8.2(%s*kWh)\r\n", Format(ED_T2, 10, 3).c_str());
		          break;
    case 7:   // Energy Returned
              sprintf(telegram, "1-0:2.8.1(%s*kWh)\r\n", Format(ER_T1, 10, 3).c_str());
		          break;
    case 8:   sprintf(telegram, "1-0:2.8.2(%s*kWh)\r\n", Format(ER_T2, 10, 3).c_str());
		          break;
    case 9:   // Tariff indicator electricity
  	          sprintf(telegram, "0-0:96.14.0(%04d)\r\n", ETariffInd);
		          break;
    case 10:  // Actual electricity power delivered (+P) in 1 Watt resolution
  	          sprintf(telegram, "1-0:1.7.0(%s*kW)\r\n", Format(PDelivered, 6, 2).c_str());
		          break;
    case 11:  // Actual electricity power received (-P) in 1 Watt resolution
  	          sprintf(telegram, "1-0:2.7.0(%s*kW)\r\n", Format(PReceived, 6, 2).c_str());
		          break;
    case 12:  // Number of power failures in any phase
  	          sprintf(telegram, "0-0:96.7.21(00010)\r\n", val);
		          break;
    case 13:  // Number of long power failures in any phase
  	          sprintf(telegram, "0-0:96.7.9(00000)\r\n", val);
		          break;
    case 14:  // Power Failure Event Log (long power failures)
  	          sprintf(telegram, "1-0:99.97.0(0)(0-0:96.7.19)\r\n", val);
		          break;
    case 15:  // Number of voltage sags in phase L1
  	          sprintf(telegram, "1-0:32.32.0(00002)\r\n", val);
		          break;
    case 16:  // Number of voltage sags in phase L2 (polyphase meters only)
  	          sprintf(telegram, "1-0:52.32.0(00003)\r\n", val);
		          break;
    case 17:  // Number of voltage sags in phase L3 (polyphase meters only)
  	          sprintf(telegram, "1-0:72.32.0(00003)\r\n", val);
		          break;
    case 18:  // Number of voltage swells in phase L1
  	          sprintf(telegram, "1-0:32.36.0(00000)\r\n", val);
		          break;
    case 19:  // Number of voltage swells in phase L2
  	          sprintf(telegram, "1-0:52.36.0(00000)\r\n", val);
		          break;
    case 20:  // Number of voltage swells in phase L3
  	          sprintf(telegram, "1-0:72.36.0(00000)\r\n", val);
		          break;
    case 21:  // Text message max 2048 characters
  	          sprintf(telegram, "0-0:96.13.0()\r\n", val);
		          break;
    case 22:  // Instantaneous voltage L1 in 0.1V resolution
  	          sprintf(telegram, "1-0:32.7.0(%03d.0*V)\r\n", (240 + random(-3,3)));
		          break;
    case 23:  // Instantaneous voltage L1 in 0.1V resolution
  	          sprintf(telegram, "1-0:52.7.0(%03d.0*V)\r\n", (238 + random(-3,3)));
		          break;
    case 24:  // Instantaneous voltage L1 in 0.1V resolution
  	          sprintf(telegram, "1-0:72.7.0(%03d.0*V)\r\n", (236 + random(-3,3)));
		          break;
    case 25:  // Instantaneous current L1 in A resolution
  	          sprintf(telegram, "1-0:31.7.0(%03d*A)\r\n", random(0,4));
		          break;
    case 26:  // Instantaneous current L2 in A resolution
  	          sprintf(telegram, "1-0:51.7.0(%03d*A)\r\n",  random(0,4));
		          break;
    case 27:  // Instantaneous current L3 in A resolution
  	          sprintf(telegram, "1-0:71.7.0(000*A)\r\n", val);
		          break;
    case 28:  // Instantaneous active power L1 (+P) in W resolution
  	          sprintf(telegram, "1-0:21.7.0(%s*kW)\r\n", Format(IPD_l1, 6, 3).c_str());
		          break;
    case 29:  // Instantaneous active power L2 (+P) in W resolution
  	          sprintf(telegram, "1-0:41.7.0(%s*kW)\r\n", Format(IPD_l2, 6, 3).c_str());
		          break;
    case 30:  // Instantaneous active power L3 (+P) in W resolution
  	          sprintf(telegram, "1-0:61.7.0(%s*kW)\r\n", Format(IPD_l3, 6, 3).c_str());
		          break;
    case 31:  // Instantaneous active power L1 (-P) in W resolution
  	          sprintf(telegram, "1-0:22.7.0(%s*kW)\r\n", Format(IPR_l1, 6, 3).c_str());
		          break;
    case 32:  // Instantaneous active power L2 (-P) in W resolution
  	          sprintf(telegram, "1-0:42.7.0(%s*kW)\r\n", Format(IPR_l2, 6, 3).c_str());
		          break;
    case 33:  // Instantaneous active power L3 (-P) in W resolution
  	          sprintf(telegram, "1-0:62.7.0(%s*kW)\r\n", Format(IPR_l3, 6, 3).c_str());
		          break;
    case 34:  // Gas Device-Type
  	          sprintf(telegram, "0-1:24.1.0(003)\r\n", val);
		          break;
    case 35:  // Equipment identifier (Gas)
  	          sprintf(telegram, "0-1:96.1.0(4730303339303031363532303530323136)\r\n", val);
              break;
    case 36:  // Last 5-minute value (temperature converted), gas delivered to client
              // in m3, including decimal values and capture time (Note: 4.x spec has
  	          sprintf(telegram, "0-1:24.2.1(%02d%02d%02d%02d%02d01S)(%s*m3)\r\n", (year() - 2000), month(), day(), hour(), minute(), 
                                                                            Format(GDelivered, 9, 3).c_str());
              break;
  	case 37:  sprintf(telegram, "!xxxx\r\n");		
  	          break;
              
  } // switch(line)

  if (line < 37) {
    Serial.print(telegram); // <<<<---- nooit weghalen!!!!
    if (Verbose) {
      if (line == 0) {
        _dThis = true;
        Debugln();
      }
      _dThis = false;
      Debug(telegram);
    }
    //else if (line = 3) Debug(telegram);
  }

  for(len = 0; len < MAXLINELENGTH, telegram[len] != '\0'; len++) {}    
  
  return len;

} // buildTelegram40()


//==================================================================================================
int16_t buildTelegram30(int16_t line, char telegram[]) {
/*
**  /KMP5 KA6U001585575011                - Telegram begin-marker + manufacturer + serial number
**  
**  0-0:96.1.1(204B413655303031353835353735303131)    -  Serial number in hex
**  1-0:1.8.1(08153.408*kWh)                          -  +T1: Energy input, low tariff (kWh)
**  1-0:1.8.2(05504.779*kWh)                          -  +T2: Energy input, normal tariff (kWh)
**  1-0:2.8.1(00000.000*kWh)                          -  -T3: Energy output, low tariff (kWh)
**  1-0:2.8.2(00000.000*kWh)                          -  -T4: Energy output, normal tariff (kWh)
**  0-0:96.14.0(0002)                                 -  Current tariff (1=low, 2=normal)
**  1-0:1.7.0(0000.30*kW)                             -  Actual power input (kW)
**  1-0:2.7.0(0000.00*kW)                             -  Actual power output (kW)
**  0-0:17.0.0(999*A)                                 -  Max current per phase (999=no max)
**  0-0:96.3.10(1)                                    -  Switch position
**  0-0:96.13.1()                                     -  Message code
**  0-0:96.13.0()                                     -  Message text
**  0-1:24.1.0(3)                                     -  Attached device type identifier
**  0-1:96.1.0(3238313031353431303031333733353131)    -  Serial number of gas meter
**  0-1:24.3.0(190718190000)(08)(60)(1)(0-1:24.2.1)(m3) -  Time of last gas meter update
**  (04295.190)                                       -  Gas meter value (m³)
**  0-1:24.4.0(1)                                     -  Gas valve position
**  !                                                 -  Telegram end-marker
**  
*/
//==================================================================================================
  int16_t len = 0;

  float val;

  switch (line) {
                                //KMP5 KA6U001585575011
    case 0:   sprintf(telegram, "/KMP5 KA6U001585575011\r\n");
              break;
    case 1:   sprintf(telegram, "\r\n");    
              break;
    case 2:   sprintf(telegram, "0-0:96.1.1(4530303336303000000000000000000000)\r\n", val);
              break;
    case 3:   // Energy Delivered
              sprintf(telegram, "1-0:1.8.1(%s*kWh)\r\n", Format(ED_T1, 10, 3).c_str());
              break;
    case 4:   sprintf(telegram, "1-0:1.8.2(%s*kWh)\r\n", Format(ED_T2, 10, 3).c_str());
              break;
    case 5:   // Energy Returned
              sprintf(telegram, "1-0:2.8.1(%s*kWh)\r\n", Format(ER_T1, 10, 3).c_str());
              break;
    case 6:   sprintf(telegram, "1-0:2.8.2(%s*kWh)\r\n", Format(ER_T2, 10, 3).c_str());
              break;
    case 7:   // Tariff indicator electricity
              sprintf(telegram, "0-0:96.14.0(%04d)\r\n", ETariffInd);
              break;
    case 8:   // Actual electricity power delivered (+P) in 1 Watt resolution
              sprintf(telegram, "1-0:1.7.0(%s*kW)\r\n", Format(PDelivered, 6, 2).c_str());
              break;
    case 9:   // Actual electricity power received (-P) in 1 Watt resolution
              sprintf(telegram, "1-0:2.7.0(%s*kW)\r\n", Format(PReceived, 6, 2).c_str());
              break;
    case 10:  // Max current per phase (999=no max)
              sprintf(telegram, "0-0:17.0.0(016*A)\r\n", val);
              break;
    case 11:  // Switch position (?)
              sprintf(telegram, "0-0:96.3.10(1)\r\n", val);
              break;
    case 12:  // Text message code
              sprintf(telegram, "0-0:96.13.1()\r\n", val);
              break;
    case 13:  // Text message text
              sprintf(telegram, "0-0:96.13.0()\r\n", val);
              break;
    case 14:  // Gas Device-Type
              sprintf(telegram, "0-1:24.1.0(3)\r\n", val);
              break;
    case 15:  // Equipment identifier (Gas)
              sprintf(telegram, "0-1:96.1.0(4730303339303031363500000000000000)\r\n", val);
              break;
    case 16:  // Last 5-minute value (temperature converted), gas delivered to client
              // in m3, including decimal values and capture time 
              sprintf(telegram, "0-1:24.3.0(%02d%02d%02d%02d%02d00)(08)(60)(1)(0-1:24.2.1)(m3)\r\n", (year() - 2000), month(), day(), hour(), minute());
              break;
    case 17:  sprintf(telegram, "(%s)\r\n", Format(GDelivered, 9, 3).c_str());
              break;
    case 18:  // Gas valve position
              sprintf(telegram, "0-1:24.4.0(1)\r\n", val);
              break;
    case 19:  sprintf(telegram, "!\r\n\r\n");     // just for documentation 
              break;
              
  } // switch(line)

  if (line < 19) {
    Serial.print(telegram); // <<<<---- nooit weghalen!!!!
    if (Verbose) {
      if (line == 0) {
        _dThis = true;
        Debugln();
      }
      _dThis = false;
      Debug(telegram);
    }
  }

  for(len = 0; len < MAXLINELENGTH, telegram[len] != '\0'; len++) {}    
  
  return len;

} // buildTelegram30()


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
  if (actHour >= 6 && actHour <= 18) {
    IPR_l1    = (float)(random(1,975) * 0.001109);
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
    for (int16_t line = 0; line < 38; line++) {
      yield();
      int16_t len = buildTelegram40(line, telegram);  // also: prints to DSMRsend
      calcCRC = decodeTelegram(len);
    } 
    Serial.printf("!%04X\r\n\r\n", (calcCRC & 0xFFFF));
    if (Verbose) Debugf("!%04X\r\n\r\n", (calcCRC & 0xFFFF));

  } else {
    for (int16_t line = 0; line < 20; line++) {
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

  Serial.println("\nStarting ....\n");
  Debugln("\nStarting ....\n");
  Serial.println("\n=======================");
  Debugln("\n=======================");

  Serial.printf("Connect to WiFi as [%s]\n", String(_HOSTNAME).c_str());
  WiFi.hostname(_HOSTNAME);
  WiFi.begin ( _SSID, _PASSWORD );
  WiFi.mode(WIFI_STA);
  int probeerWiFi = 0;
  // Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 ); Serial.print ( "." );
    probeerWiFi++;
    if ( probeerWiFi > 20 ) {
      ESP.reset();
      while(true)
        delay(1);
    }
  }
  // WiFi connexion is OK
  Debugln ( "" );
  Serial.print ( "Connected to " ); Serial.println ( WiFi.SSID() );
  Debug ( "Connected to " ); Serial.println ( WiFi.SSID() );
  Serial.print ( "IP address: " );  Serial.println ( WiFi.localIP() );
  Debug ( "IP address: " );  Serial.println ( WiFi.localIP() );

  if (MDNS.begin(_HOSTNAME)) {              // Start the mDNS responder for DSMRinjector.local
    Serial.println("mDNS responder started");
    Debugln("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
    Debugln("Error setting up MDNS responder!");
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

  httpUpdater.setup(&httpServer);

  httpServer.on("/", HTTP_POST, callIndex_html);
  httpServer.on("/ReBoot", HTTP_POST, handleReBoot);

  httpServer.onNotFound([]() {
    //_dThis = true;
    //Debugln("============================================================");
    //_dThis = true;
    //Debugf("onNotFound(%s)\n", httpServer.uri().c_str());
    //_dThis = true;
    //Debugln("============================================================");
    if (httpServer.uri() == "/update") {
      _dThis = true;
      Debugf("onNotFound(%s): ==> [/update]\n", httpServer.uri().c_str());
      httpServer.send(200, "text/html", "/update" );
      
    } else if (httpServer.uri() == "/") {
      //_dThis = true;
      //Debugf("onNotFound(%s) ==> [/]\n", httpServer.uri().c_str());
      httpServer.send(200, "text/html", DSMRindex_html );
      reloadPage("/");
  //} else {
  //  _dThis = true;
  //  Debugf("onNotFound(%s) ==> [???] do nothing ..\n", httpServer.uri().c_str());
      
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
  _dThis = true;

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
                _dThis = true;
                Debugln("runMode [SInit]");
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
                _dThis = true;
                //Debugln("runMode [SMonth]");
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
                _dThis = true;
                //Debugln("runMode [SDay]");
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
                _dThis = true;
                //Debugln("runMode [SHour]");
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
