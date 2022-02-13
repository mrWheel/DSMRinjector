

#define MAXLINELENGTH   512 // longest normal line is 47 char (+3 for \r\n\0)
#define _HOSTNAME       "DSMRinjector2"
#define _SIGNAL_LED      14
#define _DATA_REGUEST    12
#define _FLASH_BUTTON      0
#define _SHOW_BTN_TIME    2000
#define _SETTINGS_FILE      "/DSMRsettings.ini"
#define _TELEGRAM_FILE      "/telegram.dat"

#include "CRC16.h"
#include <TimeLib.h>  // https://github.com/PaulStoffregen/Time
#include <TelnetStream.h>
#include "Debug.h"
#include "safeTimers.h"
#include "oledStuff.h"
#include "FS.h"
#include "LittleFS.h"
#define FSYS  LittleFS
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "ModUpdateServer.h"  // https://github.com/mrWheel/ModUpdateServer
#include "UpdateServerIndex.h"
#include <WiFiManager.h>        // version 0.14.0 - https://github.com/tzapu/WiFiManager
#include <ESP8266mDNS.h>
#include <WebSocketsServer.h>   // Version 20.05.2015 - https://github.com/Links2004/arduinoWebSockets
#include "index_html.h"

ESP8266WebServer        httpServer (80);
ESP8266HTTPUpdateServer httpUpdater(true);

WebSocketsServer webSocket = WebSocketsServer(81);

enum { SInit, SMonth, SDay, SHour, SNormal };

//------ status bit's -------------------------------------------------
enum  { PRESSED_BIT, QUICKRELEASE_BIT, MIDRELEASE_BIT, LONGRELEASE_BIT
       , DUM4_BIT, DUM5_BIT, DUM6_BIT, DUM7_BIT
      };

//------ press Types ----------
enum  { NOT_PRESSED, QUICK_PRESSED, MID_PRESSED, LONG_PRESSED };

uint8_t runMode   = 0;
uint8_t runStatus = 1;

char telegram[MAXLINELENGTH];


char ssid[] = "AandeWiFi";      // your network SSID (name)
char password[] = "3741TS12tl"; // your network key

char        cMsg[150], fChar[10];
String      lastReset           = "";
uint16_t    currentCRC; 
int16_t     calcCRC;
const bool  outputOnSerial = true;
uint32_t    nextTelegram = 0, actInterval = 5, nextMinute = 0, nextESPcheck = 0, nextGuiUpdate;
uint32_t    signalLedTimer = 0;
uint32_t    telegramCount = 0;
uint32_t    thisSecond;
int8_t      State;
int16_t     actSec, actMinute, actHour, actDay, actMonth, actYear, actSpeed;
int8_t      actGasMBus;
char        actDSMR[3], savDSMR[3];
char        actTiming[7]; // 'NTP' or 'INTERN'
bool        useNTPtiming = true;
double      ED_T1=0, ED_T2=0, ER_T1=0, ER_T2=0, V_l1=0, V_l2=0, V_l3=0, C_l1=0, C_l2=0, C_l3=0;
uint8_t     ETariffInd=1;
float       PDelivered, PReceived;
float       IPD_l1, IPD_l2, IPD_l3, IPR_l1, IPR_l2, IPR_l3;
float       GDelivered;
bool        Verbose = false;
int8_t      verboseCount = 0;
char        cTimeStamp[25];
uint16_t    maxLines40, maxLinesBE, maxLines30; 
uint8_t     btnState = 0;
uint32_t    interTimer, bounceTimer, buttonTimer, aliveTimer;
uint32_t    showBtnTimer;
uint8_t     pressType = 0;
//----- littleFSmanager --
bool            catalog[256];
bool            actCatalog = false;
//----- timeStamp stuff --
time_t      ntpTime;
bool        DSTactive;
uint64_t    upTimeSeconds;

File        telegramFile;


/*eof*/
