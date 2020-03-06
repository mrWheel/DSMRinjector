


//==================================================================================================
int16_t buildTelegram40(int16_t line, char telegram[]) {
//==================================================================================================
/*
**  /XMX5LGBBLB2410065887                               - 
**                                                      - 
**  1-3:0.2.8(50)                                       - p1_version
**  0-0:1.0.0(140101072001S)                            - timestamp
**  0-0:96.1.1(4530303336303000000000000000000040)      - equiptment_id
**  1-0:1.8.1(000145.820*kWh)                           - energy_delivered_tariff1
**  1-0:1.8.2(000068.438*kWh)                           - energy_delivered_tariff2
**  1-0:2.8.1(000111.105*kWh)                           - energy_returned_tariff1
**  1-0:2.8.2(000075.502*kWh)                           - energy_returned_tariff2
**  0-0:96.14.0(0001)                                   - electricity_tariff
**  1-0:1.7.0(001.72*kW)                                - power_delivered
**  1-0:2.7.0(002.05*kW)                                - power_returned
**  0-0:96.7.21(00010)                                  - electricity_failures
**  0-0:96.7.9(00000)                                   - electricity_long_failures
**  1-0:99.97.0(0)(0-0:96.7.19)                         - electricity_failure_log
**  1-0:32.32.0(00002)                                  - electricity_sags_l1
**  1-0:52.32.0(00003)                                  - electricity_sags_l2
**  1-0:72.32.0(00003)                                  - electricity_sags_l3
**  1-0:32.36.0(00000)                                  - electricity_swells_l1
**  1-0:52.36.0(00000)                                  - electricity_swells_l2
**  1-0:72.36.0(00000)                                  - electricity_swells_l3
**  0-0:96.13.0()                                       - message_long
**  1-0:32.7.0(238.0*V)                                 - voltage_l1
**  1-0:52.7.0(239.0*V)                                 - voltage_l2
**  1-0:72.7.0(238.0*V)                                 - voltage_l3
**  1-0:31.7.0(002*A)                                   - current_l1
**  1-0:51.7.0(000*A)                                   - current_l2
**  1-0:71.7.0(000*A)                                   - current_l3
**  1-0:21.7.0(00.969*kW)                               - power_delivered_l1
**  1-0:41.7.0(00.392*kW)                               - power_delivered_l2
**  1-0:61.7.0(00.362*kW)                               - power_delivered_l3
**  1-0:22.7.0(01.036*kW)                               - power_returned_l1
**  1-0:42.7.0(00.694*kW)                               - power_returned_l2
**  1-0:62.7.0(00.324*kW)                               - power_returned_l3
**  0-1:24.1.0(003)                                     - gas_device_type
**  0-1:96.1.0(4730303339303031363532303530323136)      - gas_equipment_id
**  0-1:24.2.1(140101072001S)(00100.115*m3)             - gas_delivered
**  !349A                                               - 
*/

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
    case 37:  // Water Device-Type
              sprintf(telegram, "0-2:24.1.0(005)\r\n", val);
              break;
    case 38:  // Equipment identifier (Water)
              sprintf(telegram, "0-2:96.1.0(4730303339303031363532303530323136)\r\n", val);
              break;
    case 39:  // water
              sprintf(telegram, "0-2:24.2.1(%02d%02d%02d%02d%02d01S)(%s*m3)\r\n", (year() - 2000), month(), day(), hour(), minute(), 
                                                                            Format(GDelivered, 9, 3).c_str());
              break;
    case 40:  sprintf(telegram, "!xxxx\r\n");   
              break;
              
  } // switch(line)

  maxLines40 = 40;
  
  if (line < maxLines40) {
    Serial.print(telegram); // <<<<---- nooit weghalen!!!!
    if (Verbose && !((telegramCount % 2) == 0)) {
      if (line == 0) {
        Debugln();
      }
      Debug(telegram);
    }
    //else if (line = 3) Debug(telegram);
  }

  for(len = 0; len < MAXLINELENGTH, telegram[len] != '\0'; len++) {}    
  
  return len;

} // buildTelegram40()


//==================================================================================================
int16_t buildTelegramBE(int16_t line, char telegram[]) {
//==================================================================================================
/*
** ======== MONO fase ================================
**  /FLU5\253770234_A                                   - 
**                                                      - 
**  0-0:96.1.4(50213)                                   - p1_version (be)
**  0-0:96.1.1(3153414731313030303331373636)            - equiptment_id
**  0-0:1.0.0(200304193944W)                            - timestamp
**  1-0:1.8.1(000387.318*kWh)                           - energy_delivered_tariff1
**  1-0:1.8.2(000375.952*kWh)                           - energy_delivered_tariff2
**  1-0:2.8.1(000000.001*kWh)                           - energy_returned_tariff1
**  1-0:2.8.2(000000.025*kWh)                           - energy_returned_tariff2
**  0-0:96.14.0(0001)                                   - electricity_tariff
**  1-0:1.7.0(00.389*kW)                                - power_delivered
**  1-0:2.7.0(00.000*kW)                                - power_returned
**  1-0:32.7.0(240.9*V)                                 - voltage_l1
**  1-0:31.7.0(002*A)                                   - current_l1
**  0-0:96.3.10(1)                                      - electricity_switch_position
**  0-0:17.0.0(999.9*kW)                                - electricity_threshold
**  1-0:31.4.0(999*A)                                   - fuse_treshold_l1
**  0-0:96.13.0()                                       - message_long
**  0-1:24.1.0(003)                                     - gas_device_type
**  0-1:96.1.1(37464C4F32313139303935333131)            - gas_equipment_id (be)
**  0-1:24.4.0(1)                                       - gas_valve_position
**  0-1:24.2.3(200304193502W)(00820.743*m3)             - gas_delivered (be)
**  !B5E3
**
** ======== MULTI fase ===============================
**  /FLU5\253769484_A                                   - 
**                                                      - 
**  0-0:96.1.4(50213)                                   - p1_version (be)
**  0-0:96.1.1(3153414733313030303638373236)            - equiptment_id
**  0-0:1.0.0(191204184601W)                            - timestamp
**  1-0:1.8.1(000050.069*kWh)                           - energy_delivered_tariff1
**  1-0:1.8.2(000055.085*kWh)                           - energy_delivered_tariff2
**  1-0:2.8.1(000019.870*kWh)                           - energy_returned_tariff1
**  1-0:2.8.2(000005.678*kWh)                           - energy_returned_tariff2
**  0-0:96.14.0(0001)                                   - electricity_tariff
**  1-0:1.7.0(00.655*kW)                                - power_delivered
**  1-0:2.7.0(00.000*kW)                                - power_returned
**  1-0:32.7.0(225.1*V)                                 - voltage_l1
**  1-0:52.7.0(000.0*V)                                 - voltage_l2
**  1-0:72.7.0(225.7*V)                                 - voltage_l3
**  1-0:31.7.0(001*A)                                   - current_l1
**  1-0:51.7.0(002*A)                                   - current_l2
**  1-0:71.7.0(001*A)                                   - current_l3
**  0-0:96.3.10(1)                                      - electricity_switch_position
**  0-0:17.0.0(999.9*kW)                                - electricity_threshold
**  1-0:31.4.0(999*A)                                   - fuse_treshold_l1
**  0-0:96.13.0()                                       - message_long
**  0-1:24.1.0(003)                                     - gas_device_type
**  0-1:96.1.1(37464C4F32313139303935373430)            - gas_equipment_id (be)
**  0-1:24.4.0(1)                                       - gas_valve_position
**  0-1:24.2.3(191204184600W)(00070.043*m3)             - gas_delivered (be)
**  !6E84
*/

  int16_t len = 0;

  float val;

  switch (line) {
                                //FLU5\253769484_A
    case 0:   sprintf(telegram, "/FLU5\\253769484_A\r\n");
              break;
    case 1:   sprintf(telegram, "\r\n");    
              break;
    case 2:   sprintf(telegram, "0-0:96.1.4(50213)\r\n");
              break;
    case 3:   sprintf(telegram, "0-0:1.0.0(%02d%02d%02d%02d%02d%02dS)\r\n", (year() - 2000), month(), day(), hour(), minute(), second());
              break;
    case 4:   sprintf(telegram, "0-0:96.1.1(4530303336303000000000000000000502)\r\n", val);
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
              sprintf(telegram, "1-0:72.32.0(00004)\r\n", val);
              break;
    case 18:  // Number of voltage swells in phase L1
              sprintf(telegram, "1-0:32.36.0(00003)\r\n", val);
              break;
    case 19:  // Number of voltage swells in phase L2
              sprintf(telegram, "1-0:52.36.0(00002)\r\n", val);
              break;
    case 20:  // Number of voltage swells in phase L3
              sprintf(telegram, "1-0:72.36.0(00001)\r\n", val);
              break;
    case 21:  // Instantaneous voltage L1 in 0.1V resolution
              sprintf(telegram, "1-0:32.7.0(%03d.0*V)\r\n", (240 + random(-3,3)));
              break;
    case 22:  // Instantaneous voltage L1 in 0.1V resolution
              sprintf(telegram, "1-0:52.7.0(%03d.0*V)\r\n", (238 + random(-3,3)));
              break;
    case 23:  // Instantaneous voltage L1 in 0.1V resolution
              sprintf(telegram, "1-0:72.7.0(%03d.0*V)\r\n", (236 + random(-3,3)));
              break;
    case 24:  // Instantaneous current L1 in A resolution
              sprintf(telegram, "1-0:31.7.0(%03d*A)\r\n", random(0,4));
              break;
    case 25:  // Instantaneous current L2 in A resolution
              sprintf(telegram, "1-0:51.7.0(%03d*A)\r\n",  random(0,4));
              break;
    case 26:  // Instantaneous current L3 in A resolution
              sprintf(telegram, "1-0:71.7.0(000*A)\r\n", val);
              break;
    /*******
    case x1:  // Instantaneous active power L1 (+P) in W resolution
              sprintf(telegram, "1-0:21.7.0(%s*kW)\r\n", Format(IPD_l1, 6, 3).c_str());
              break;
    case x2:  // Instantaneous active power L2 (+P) in W resolution
              sprintf(telegram, "1-0:41.7.0(%s*kW)\r\n", Format(IPD_l2, 6, 3).c_str());
              break;
    case x3:  // Instantaneous active power L3 (+P) in W resolution
              sprintf(telegram, "1-0:61.7.0(%s*kW)\r\n", Format(IPD_l3, 6, 3).c_str());
              break;
    case x4:  // Instantaneous active power L1 (-P) in W resolution
              sprintf(telegram, "1-0:22.7.0(%s*kW)\r\n", Format(IPR_l1, 6, 3).c_str());
              break;
    case x5:  // Instantaneous active power L2 (-P) in W resolution
              sprintf(telegram, "1-0:42.7.0(%s*kW)\r\n", Format(IPR_l2, 6, 3).c_str());
              break;
    case x6:  // Instantaneous active power L3 (-P) in W resolution
              sprintf(telegram, "1-0:62.7.0(%s*kW)\r\n", Format(IPR_l3, 6, 3).c_str());
              break;
    *******/
    case 27:  // electricity_switch_position
              sprintf(telegram, "0-0:96.3.10(1)\r\n", val);
              break;
    case 28:  // electricity_threshold
              sprintf(telegram, "0-0:17.0.0(999.9*kW)\r\n", val);
              break;
    case 29:  // fuse_treshold_l1
              sprintf(telegram, "1-0:31.4.0(999*A)\r\n", val);
              break;
    case 30:  // message_long
              sprintf(telegram, "0-0:96.13.0(messageLong)\r\n", val);
              break;
    case 31:  // Gas Device-Type
              sprintf(telegram, "0-1:24.1.0(003)\r\n", val);
              break;
    case 32:  // Equipment identifier BE (Gas)
              sprintf(telegram, "0-1:96.1.1(37464C4F32313139303935373430)\r\n", val);
              break;
    case 33:  // gas_valve_position
              sprintf(telegram, "0-1:24.4.0(1)\r\n", val);
              break;
    case 34:  // gas_delivered
              sprintf(telegram, "0-1:24.2.3(%02d%02d%02d%02d%02d00S)(%s*m3)\r\n", (year() - 2000), month(), day(), hour(), minute(), 
                                                                            Format(GDelivered, 9, 3).c_str());
              break;
    case 35:  // Water Device-Type
              sprintf(telegram, "0-3:24.1.0(003)\r\n", val);
              break;
    case 36:  // Equipment identifier BE (Water)
              sprintf(telegram, "0-3:96.1.1(37464C4F32313149404945474440)\r\n", val);
              break;
    case 37:  // water_valve_position
              sprintf(telegram, "0-3:24.4.0(0)\r\n", val);
              break;
    case 38:  // water
              sprintf(telegram, "0-3:24.2.3(%02d%02d%02d%02d%02d00S)(%s*m3)\r\n", (year() - 2000), month(), day(), hour(), minute(), 
                                                                            Format((GDelivered * 2), 9, 3).c_str());
              break;
    case 39:  sprintf(telegram, "!xxxx\r\n");   
              break;
              
  } // switch(line)

  maxLinesBE = 39;
  
  if (line < maxLinesBE) {
    Serial.print(telegram); // <<<<---- nooit weghalen!!!!
    if (Verbose && !((telegramCount % 2) == 0)) {
      if (line == 0) {
        Debugln();
      }
      Debug(telegram);
    }
    //else if (line = 3) Debug(telegram);
  }

  for(len = 0; len < MAXLINELENGTH, telegram[len] != '\0'; len++) {}    
  
  return len;

} // buildTelegramBE()



//==================================================================================================
int16_t buildTelegram30(int16_t line, char telegram[]) {
/*
**  /KMP5 KA6U001585575011                              - 
**                                                      - 
**  0-0:96.1.1(204B413655303031353835353735303131)      - equiptment_id
**  1-0:1.8.1(08153.408*kWh)                            - energy_delivered_tariff1
**  1-0:1.8.2(05504.779*kWh)                            - energy_delivered_tariff2
**  1-0:2.8.1(00000.000*kWh)                            - energy_returned_tariff1
**  1-0:2.8.2(00000.000*kWh)                            - energy_returned_tariff2
**  0-0:96.14.0(0002)                                   - electricity_tariff
**  1-0:1.7.0(0000.30*kW)                               - power_delivered
**  1-0:2.7.0(0000.00*kW)                               - power_returned
**  0-0:17.0.0(999*A)                                   - electricity_threshold
**  0-0:96.3.10(1)                                      - electricity_switch_position
**  0-0:96.13.1()                                       - message_short
**  0-0:96.13.0()                                       - message_long
**  0-1:24.1.0(3)                                       - gas_device_type
**  0-1:96.1.0(3238313031353431303031333733353131)      - gas_equipment_id
**  0-1:24.3.0(190718190000)(08)(60)(1)(0-1:24.2.1)(m3  - gas_delivered2 (30)
**  (04295.190)                                         - 
**  0-1:24.4.0(1)                                       - gas_valve_position
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
  
  maxLines30 = 19;
  
  if (line < maxLines30) {
    Serial.print(telegram); // <<<<---- nooit weghalen!!!!
    if (Verbose && !((telegramCount % 2) == 0)) {
      if (line == 0) {
        Debugln();
      }
      Debug(telegram);
    }
  }

  for(len = 0; len < MAXLINELENGTH, telegram[len] != '\0'; len++) {}    
  
  return len;

} // buildTelegram30()
