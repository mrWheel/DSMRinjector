

#include "CRC16.h"
#include <TimeLib.h>  // https://github.com/PaulStoffregen/Time
#include <TelnetStream.h>
#include "Debug.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "ModUpdateServer.h"  // https://github.com/mrWheel/ModUpdateServer
#include "UpdateServerIndex.h"
#include <WiFiManager.h>        // version 0.14.0 - https://github.com/tzapu/WiFiManager
#include <ESP8266mDNS.h>
#include <WebSocketsServer.h>   // Version 20.05.2015 - https://github.com/Links2004/arduinoWebSockets
#include "index_html.h"

#define MAXLINELENGTH 128 // longest normal line is 47 char (+3 for \r\n\0)
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
uint16_t    maxLines40, maxLinesBE, maxLines30; 
