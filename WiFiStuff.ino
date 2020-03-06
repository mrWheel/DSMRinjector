
//gets called when WiFiManager enters configuration mode
//===========================================================================================
void configModeCallback (WiFiManager *myWiFiManager) 
{
  DebugTln(F("Entered config mode\r"));
  DebugTln(WiFi.softAPIP().toString());
  //if you used auto generated SSID, print it
  DebugTln(myWiFiManager->getConfigPortalSSID());

} // configModeCallback()


//===========================================================================================
void startWiFi(const char* hostname) 
{
  WiFiManager manageWiFi;

  String thisAP = String(hostname) + "-" + WiFi.macAddress();
  
  manageWiFi.setDebugOutput(true);
  
  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  manageWiFi.setAPCallback(configModeCallback);

  //---sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep in seconds
  manageWiFi.setTimeout(240);  // 4 minuten
  
  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "DSMR-WS-<MAC>"
  //and goes into a blocking loop awaiting configuration
  if (!manageWiFi.autoConnect(thisAP.c_str())) 
  {
    DebugTln(F("failed to connect and hit timeout"));

    //reset and try again, or maybe put it to deep sleep
    delay(3000);
    ESP.reset();
    delay(2000);
  }

  DebugTf("Connected with IP-address [%s]\r\n\r\n", WiFi.localIP().toString().c_str());

  httpUpdater.setup(&httpServer);
  httpUpdater.setIndexPage(UpdateServerIndex);
  httpUpdater.setSuccessPage(UpdateServerSuccess);
  
} // startWiFi()
