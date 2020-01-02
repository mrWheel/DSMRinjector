/*
***************************************************************************  
**  Program  : webSocketEvent, part of DSMRloggerWS
**  Version  : v0.4.3
**
**  Copyright (c) 2019 Willem Aandewiel
**
**  TERMS OF USE: MIT License. See bottom of file.                                                            
***************************************************************************      
*/


static String   prevTimestamp;
static bool     isConnected;
static int8_t   savMin = 0;
String wOut[10], wParm[10], wPair[4];


//===========================================================================================
void webSocketEvent(uint8_t wsClient, WStype_t type, uint8_t * payload, size_t lenght) {
//===========================================================================================
    String  wsPayload = String((char *) &payload[0]);
    char *  wsPayloadC = (char *) &payload[0];
    String  wsString;

    switch(type) {
        case WStype_DISCONNECTED:
            _dThis = true;
            Debugf("[%u] Disconnected!\n", wsClient);
            isConnected = false;
            break;
            
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(wsClient);
                if (!isConnected) {
                 _dThis = true;
                 Debugf("[%u] Connected from %d.%d.%d.%d url: %s\n", wsClient, ip[0], ip[1], ip[2], ip[3], payload);
                 isConnected = true;
                 webSocket.sendTXT(wsClient, "{\"msgType\":\"ConnectState\",\"Value\":\"Connected\"}");
                }
        
            }
            break;
            
        case WStype_TEXT:
            _dThis = true;
            Debugf("[%u] Got message: [%s]\n", wsClient, payload);
            String FWversion = String(_FW_VERSION);

            if (wsPayload.indexOf("getDevInfo") > -1) {
              wsString  = "";
              wsString += ", devName=" + String(_HOSTNAME);
              wsString += ", devIPaddress=" + WiFi.localIP().toString() ;
              wsString += ", devVersion=[" + FWversion.substring(0, (FWversion.indexOf('(') -1)) + "]";
              wsString += ", actYear=" + String(actYear);
              wsString += ", actMonth=" + String(actMonth);
              wsString += ", actDay=" + String(actDay);
              wsString += ", actHour=" + String(actHour);
              wsString += ", actSpeed=" + String(actSpeed);

              _dThis = true;
              Debugln(wsString);
              webSocket.sendTXT(wsClient, "msgType=devInfo" + wsString);
              
            } else if (wsPayload.indexOf("newDate") > -1) {
              int8_t wc = splitString(wsPayload.c_str(), ':', wOut, 10);
              Debugf("wOut[1] => [%s]\n", wOut[1].c_str());
              wc = splitString(wOut[1].c_str(), ',', wParm, 10);
              for(int f=0; f <wc; f++) {
                Debugf("wParm[%d] => [%s]\n", f, wParm[f].c_str());
                splitString(wParm[f].c_str(), '=', wPair, 4);
                Debugf("newDate >> wParm[%d][%s] : Field[%s] => Value[%s]\n", f, wParm[f].c_str(), wPair[0].c_str(), wPair[1].c_str());
                if (wPair[0] == "newYear")          actYear     = wPair[1].toInt();
                else if (wPair[0] == "newMonth")    actMonth    = wPair[1].toInt();
                else if (wPair[0] == "newDay")      actDay      = wPair[1].toInt();
                else if (wPair[0] == "newHour")     actHour     = wPair[1].toInt();
                else if (wPair[0] == "newSpeed")    actSpeed    = wPair[1].toInt();
                else if (wPair[0] == "newInterval") actInterval = wPair[1].toInt();
              }
              updateTime();
              //[zit in updateTime()] setTime(actHour, actMinute, 1, actDay, actMonth, actYear);
              wsString  = "";
              wsString += ", actYear="      + String(actYear);
              wsString += ", actMonth="     + String(actMonth);
              wsString += ", actDay="       + String(actDay);
              wsString += ", actHour="      + String(actHour);
              wsString += ", actSpeed="     + String(actSpeed);
              wsString += ", actInterval="  + String(actInterval);

              _dThis = true;
              Debugf("msgType=timeStamp %s\n", wsString.c_str());
              webSocket.sendTXT(wsClient, "msgType=timeStamp" + wsString);

              
            } else if (wsPayload.indexOf("runMode") > -1) {
              int8_t wc = splitString(wsPayload.c_str(), ':', wOut, 10);
              //Debugf("wOut[1] => [%s]\n", wOut[1].c_str());
              wc = splitString(wOut[1].c_str(), ',', wParm, 10);
              for(int f=0; f <wc; f++) {
                //Debugf("wParm[%d] => [%s]\n", f, wParm[f].c_str());
                splitString(wParm[f].c_str(), '=', wPair, 4);
                Debugf("runMode >> wParm[%d][%s] : Field[%s] => Value[%s]\n", f, wParm[f].c_str(), wPair[0].c_str(), wPair[1].c_str());
                if (wPair[1] == "doNormal")       runMode = SNormal;
                else if (wPair[1] == "doHours")   runMode = SHour;
                else if (wPair[1] == "doDays")    runMode = SDay;
                else if (wPair[1] == "doMonths")  runMode = SMonth;
                else                              runMode = SInit;
                Debugf("runMode [%s] set to [%d]\n", wPair[1].c_str(), runMode);
              }

            } else if (wsPayload.indexOf("runStatus") > -1) {
              int8_t wc = splitString(wsPayload.c_str(), ':', wOut, 10);
              //Debugf("wOut[1] => [%s]\n", wOut[1].c_str());
              wc = splitString(wOut[1].c_str(), ',', wParm, 10);
              for(int f=0; f <wc; f++) {
                //Debugf("wParm[%d] => [%s]\n", f, wParm[f].c_str());
                splitString(wParm[f].c_str(), '=', wPair, 4);
                Debugf("runStatus >> wParm[%d][%s] : Field[%s] => Value[%s]\n", f, wParm[f].c_str(), wPair[0].c_str(), wPair[1].c_str());
                if (wPair[1] == "doStart")    runStatus = 1;
                else                          runStatus = 0;
                Debugf("runStatus [%s] set to [%d]\n", wPair[1].c_str(), runStatus);
              }

            } else if (wsPayload.indexOf("setDSMR") > -1) {
              int8_t wc = splitString(wsPayload.c_str(), ':', wOut, 10);
              //Debugf("wOut[1] => [%s]\n", wOut[1].c_str());
              wc = splitString(wOut[1].c_str(), '=', wParm, 10);
              //_dThis = true;
              //Debugf("wOut[1][%s] => [%s]=[%s]\n", wOut[1].c_str(), wParm[0].c_str(), wParm[1].c_str());
              if (String(actDSMR) != wParm[0]) {
                runStatus = 0;  // stop running!
                sprintf(actDSMR, "%s", wParm[1].c_str());
                _dThis = true;
                Debugf("DSMR standaard changed to [%s]\n", actDSMR);
              }
              //_dThis = true;
              //Debugf("DSMR standaard [%s]\n", actDSMR);

            }

            break;
                        
    } // switch(type)
  
} // webSocketEvent()


//==================================================================================================
void updateGUI() {
//==================================================================================================
  String wsString;
  
  sprintf(cTimeStamp, "%04d-%02d-%02d %02d:%02d:%02d", year(), month(), day(), hour(), minute(), second());
  wsString  = "";
  wsString += ", timeStamp=" + String(cTimeStamp);
  wsString += ", actYear=" + String(year());
  wsString += ", actMonth=" + String(month());
  wsString += ", actDay=" + String(day());
  wsString += ", actHour=" + String(hour());
  wsString += ", actSpeed=" + String(actSpeed);
  wsString += ", actInterval=" + String(actInterval);
  wsString += ", actDSMR=" + String(actDSMR);
  wsString += ", telegramCount=" + String(telegramCount);
  if (runStatus == 1) wsString += ", runStatus=1";
  else                wsString += ", runStatus=0";
  _dThis = true;
  //Debugf("msgType=timeStamp %s\n", wsString.c_str());
  nextGuiUpdate = millis() + 5000;

  webSocket.broadcastTXT("msgType=timeStamp" + wsString);

} // updateGUI()
