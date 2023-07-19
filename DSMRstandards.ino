


//==================================================================================================
int16_t buildTelegram42(int16_t line, char telegramLine[])
{
  /*
  **  /XMX5LGBBFG1012462ORG
  **
  **  1-3:0.2.8(42)
  **  0-0:1.0.0(211221191204W)
  **  0-0:96.1.1(4530303330303033323737343233363136)
  **  1-0:1.8.1(008780.685*kWh)
  **  1-0:1.8.2(007377.481*kWh)
  **  1-0:2.8.1(005760.594*kWh)
  **  1-0:2.8.2(012968.774*kWh)
  **  0-0:96.14.0(0002)
  **  1-0:1.7.0(00.090*kW)
  **  1-0:2.7.0(00.024*kW)
  **  0-0:96.7.21(00005)
  **  0-0:96.7.9(00002)
  **  1-0:99.97.0(2)(0-0:96.7.19)(190128094407W)(0000001319*s)(180118110136W)(0000000184*s)
  **  1-0:32.32.0(00002)
  **  1-0:52.32.0(00002)
  **  1-0:72.32.0(00006)
  **  1-0:32.36.0(00000)
  **  1-0:52.36.0(00000)
  **  1-0:72.36.0(00000)
  **  0-0:96.13.1(444444)
  **  0-0:96.13.0(19012809440718011811013619012809440713191801181101360000001849012809440790128094407131918011811013600000018490128094409012809440713191801181101360000001849012809440000131911811013619012809440744071319180118110136000000184901280944070001319118110440713191801181101360000001849012809440700013191181104407131918011811013600000018490128094407000131911811044071319180118110136000000184901280944070001319118110)
  **  1-0:31.7.0(002*A)
  **  1-0:51.7.0(001*A)
  **  1-0:71.7.0(000*A)
  **  1-0:21.7.0(00.030*kW)
  **  1-0:41.7.0(00.030*kW)
  **  1-0:61.7.0(00.030*kW)
  **  1-0:22.7.0(00.008*kW)
  **  1-0:42.7.0(00.008*kW)
  **  1-0:62.7.0(00.008*kW)
  **  0-1:24.1.0(003)
  **  0-1:96.1.0(4730303235303033353033313037313137)
  **  0-1:24.2.1(211221190000W)(00328.183*m3)
  **  !D13B
  */

  int16_t len = 0;
  float val;

  memset(telegramLine, 0, sizeof(telegramLine));

  switch (line)
  {
    //XMX5LGBBFG1012462ORG
    case 0:
      sprintf(telegramLine, "/XMX5LGBBFG1012460042\r\n");
      break;
    case 1:
      sprintf(telegramLine, "\r\n");
      break;
    case 2:
      sprintf(telegramLine, "1-3:0.2.8(42)\r\n");
      break;
    case 3:
      sprintf(telegramLine, "0-0:1.0.0(%02d%02d%02d%02d%02d%02dS)\r\n", (year() - 2000), month(), day(), hour(), minute(), thisSecond);
    //sprintf(telegramLine, "0-0:1.0.0(%02d%02d%02d%02d%02d%02d%c)\r\n", (year() - 2000), month(), day(), hour(), minute(), thisSecond, isDST()?'S':'W');
      break;
    case 4:
      sprintf(telegramLine, "0-0:96.1.1(4530303336303000000000000000000042)\r\n", val);
      break;
    case 5:   // Energy Delivered
      sprintf(telegramLine, "1-0:1.8.1(%s*kWh)\r\n", Format(ED_T1, 10, 3).c_str());
      break;
    case 6:
      sprintf(telegramLine, "1-0:1.8.2(%s*kWh)\r\n", Format(ED_T2, 10, 3).c_str());
      break;
    case 7:   // Energy Returned
      sprintf(telegramLine, "1-0:2.8.1(%s*kWh)\r\n", Format(ER_T1, 10, 3).c_str());
      break;
    case 8:
      sprintf(telegramLine, "1-0:2.8.2(%s*kWh)\r\n", Format(ER_T2, 10, 3).c_str());
      break;
    case 9:   // Tariff indicator electricity
      sprintf(telegramLine, "0-0:96.14.0(%04d)\r\n", ETariffInd);
      break;
    case 10:  // Actual electricity power delivered (+P) in 1 Watt resolution
      sprintf(telegramLine, "1-0:1.7.0(%s*kW)\r\n", Format(PDelivered, 6, 2).c_str());
      break;
    case 11:  // Actual electricity power received (-P) in 1 Watt resolution
      sprintf(telegramLine, "1-0:2.7.0(%s*kW)\r\n", Format(PReceived, 6, 2).c_str());
      break;
    case 12:  // Number of power failures in any phase
      sprintf(telegramLine, "0-0:96.7.21(00042)\r\n", val);
      break;
    case 13:  // Number of long power failures in any phase
      sprintf(telegramLine, "0-0:96.7.9(00002)\r\n", val);
      break;
    case 14:  // Power Failure Event Log (long power failures)
      sprintf(telegramLine, "1-0:99.97.0(10)(0-0:96.7.19)(190508094303S)(0000055374*s)"
              "(190507165813S)(0000007991*s)(190507141021S)(0000000274*s)"
              "(190507135954S)(0000000649*s)(190507134811S)(0000083213*s)"
              "(190506143928S)(0000090080*s)(190505123501S)(0000073433*s)"
              "(190504152603S)(0000003719*s)(190504120844S)(0000337236*s)"
              "(190430142638S)(0000165493*s)\r\n", val);
      break;
    case 15:  // Number of voltage sags in phase L1
      sprintf(telegramLine, "1-0:32.32.0(00002)\r\n", val);
      break;
    case 16:  // Number of voltage sags in phase L2 (polyphase meters only)
      sprintf(telegramLine, "1-0:52.32.0(00003)\r\n", val);
      break;
    case 17:  // Number of voltage sags in phase L3 (polyphase meters only)
      sprintf(telegramLine, "1-0:72.32.0(00003)\r\n", val);
      break;
    case 18:  // Number of voltage swells in phase L1
      sprintf(telegramLine, "1-0:32.36.0(00000)\r\n", val);
      break;
    case 19:  // Number of voltage swells in phase L2
      sprintf(telegramLine, "1-0:52.36.0(00000)\r\n", val);
      break;
    case 20:  // Number of voltage swells in phase L3
      sprintf(telegramLine, "1-0:72.36.0(00000)\r\n", val);
      break;
    case 21:  // Text message max 2048 characters (only with 4.2)
      sprintf(telegramLine, "0-0:96.13.0(190128094407180118110136190128094407131918011"
              "811013600000018490128094407901280944071319180118110136000"
              "000184901280944090128094407131918011811013600000018490128"
              "094400001319118110136190128094407440713191801181101360000"
              "001849012809440700013191181104407131918011811013600000018"
              "490128094407000131911811044071319180118110136000000184901"
              "280944070001319118110440713191801181101360000001849012809"
              "44070001319118110)\r\n", val);
      break;
    case 22:  // Text message max 2048 characters
      sprintf(telegramLine, "0-0:96.13.0()\r\n", val);
      break;
    case 23:  // Instantaneous current L1 in A resolution
      sprintf(telegramLine, "1-0:31.7.0(%03d*A)\r\n", random(1, 5));
      break;
    case 24:  // Instantaneous current L2 in A resolution
      sprintf(telegramLine, "1-0:51.7.0(%03d*A)\r\n", random(0, 4));
      break;
    case 25:  // Instantaneous current L3 in A resolution
      sprintf(telegramLine, "1-0:71.7.0(%03d*A)\r\n", random(0, 4));
      break;

    case 26:  // Instantaneous active power L1 (+P) in W resolution
      sprintf(telegramLine, "1-0:21.7.0(%s*kW)\r\n", Format(IPD_l1, 6, 3).c_str());
      break;
    case 27:  // Instantaneous active power L2 (+P) in W resolution
      sprintf(telegramLine, "1-0:41.7.0(%s*kW)\r\n", Format(IPD_l2, 6, 3).c_str());
      break;
    case 28:  // Instantaneous active power L3 (+P) in W resolution
      sprintf(telegramLine, "1-0:61.7.0(%s*kW)\r\n", Format(IPD_l3, 6, 3).c_str());
      break;

    case 29:  // Instantaneous active power L1 (-P) in W resolution
      sprintf(telegramLine, "1-0:22.7.0(%s*kW)\r\n", Format(IPR_l1, 6, 3).c_str());
      break;
    case 30:  // Instantaneous active power L2 (-P) in W resolution
      sprintf(telegramLine, "1-0:42.7.0(%s*kW)\r\n", Format(IPR_l2, 6, 3).c_str());
      break;
    case 31:  // Instantaneous active power L3 (-P) in W resolution
      sprintf(telegramLine, "1-0:62.7.0(%s*kW)\r\n", Format(IPR_l3, 6, 3).c_str());
      break;

    case 32:  // Gas Device-Type
      sprintf(telegramLine, "0-%d:24.1.0(003)\r\n", actGasMBus, val);
      break;
    case 33:  // Equipment identifier (Gas)
      sprintf(telegramLine, "0-%d:96.1.0(4730303339303031363532303530323136)\r\n", actGasMBus, val);
      break;
    case 34:  //  0-n:24.4.0(1) - gas_valve_position
      sprintf(telegramLine, "0-%d:24.4.0(%1d)\r\n", actGasMBus, (telegramCount%2));
      break;
    case 35:  // Last 5-minute value (temperature converted), gas delivered to client
      // in m3, including decimal values and capture time (Note: 4.x spec has
      sprintf(telegramLine, "0-%d:24.2.1(%02d%02d%02d%02d%02d01S)(%s*m3)\r\n", actGasMBus
              , (year() - 2000), month(), day(), hour(), minute()
              , Format(GDelivered, 9, 3).c_str());
      break;

    case 36:  // [4] Device-Type
      sprintf(telegramLine, "0-4:24.1.0(005)\r\n", val);
      break;
    case 37:  // Equipment identifier [4]
      sprintf(telegramLine, "0-4:96.1.0(4730303339303031344444444444444444)\r\n", val);
      break;
    case 38:  //  0-4:24.4.0(1) - spare [4] valve_position
      sprintf(telegramLine, "0-4:24.4.0(%1d)\r\n", (telegramCount%2));
      break;
    case 39:  // [4]
      sprintf(telegramLine, "0-4:24.2.1(%02d%02d%02d%02d%02d01S)(%s*GJ)\r\n", (year() - 2000), month(), day(), hour(), minute(),
              Format((GDelivered / 5), 9, 3).c_str());
      break;

    case 40:
      //sprintf(telegramLine, "!xxxx\r\n");
      sprintf(telegramLine, "!\0\0\0\0");
      break;

  } // switch(line)

  maxLines42 = 40;
  if (line < 1)
  {
    memset(telegram, 0, _MAX_TELEGRAM_LEN);
    telegramPos = 0;
  }

  if (line <= maxLines42)
  {
    if (Verbose && ((telegramCount % 3) == 0))
    {
      if (line == 0)
      {
        Debugf("\r\nTelegram [#%d]\r\n", telegramCount);
      }
      Debug(telegramLine);
    }
  }

  for(len = 0; len < _MAX_LINE_LEN, telegramLine[len] != '\0'; len++) {}
  return len;

} // buildTelegram42()


//==================================================================================================
int16_t buildTelegram50(int16_t line, char telegramLine[])
{
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
  **  1-0:99.97.0(10)(0-0:96.7.19)(190508094303S)        - electricity_failure_log
  *   (0000055374*s)(190507165813S)(0000007
  *   991*s)(190507141021S)(0000000274*s)(190507135954S)
  *   (0000000649*s)(190507134811S)(0000083213*s)
  *   (190506143928S)(0000090080*s)(190505123501S)
  *   (0000073433*s)(190504152603S)(0000003719*s)
  *   (190504120844S)(0000337236*s)(190430142638S)(0000165493*s)\r\n"
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
  **  0-1:24.4.0(1)                                       - gas_valve_position
  **  0-1:96.1.0(4730303339303031363532303530323136)      - gas_equipment_id
  **  0-1:24.2.1(140101072001S)(00100.115*m3)             - gas_delivered
  **  !349A                                               -
  */

  int16_t len = 0;
  float val;

  memset(telegramLine, 0, sizeof(telegramLine));

  switch (line)
  {
    //XMX5LGBBLB2410060050
    case 0:
                           //1   5    1    5    2 2 23
      sprintf(telegramLine, "/XMX5LGBBLB2410060050\r\n");
      break;
    case 1:
      sprintf(telegramLine, "\r\n");
      break;
    case 2:
      sprintf(telegramLine, "1-3:0.2.8(50)\r\n");
      break;
    case 3:
      sprintf(telegramLine, "0-0:1.0.0(%02d%02d%02d%02d%02d%02dS)\r\n", (year() - 2000), month(), day(), hour(), minute(), thisSecond);
    //sprintf(telegramLine, "0-0:1.0.0(%02d%02d%02d%02d%02d%02d%c)\r\n", (year() - 2000), month(), day(), hour(), minute(), thisSecond, isDST()?'S':'W');
      break;
    case 4:
      sprintf(telegramLine, "0-0:96.1.1(4530303336303000000000000000000040)\r\n", val);
      break;
    case 5:   // Energy Delivered
      sprintf(telegramLine, "1-0:1.8.1(%s*kWh)\r\n", Format(ED_T1, 10, 3).c_str());
      break;
    case 6:
      sprintf(telegramLine, "1-0:1.8.2(%s*kWh)\r\n", Format(ED_T2, 10, 3).c_str());
      break;
    case 7:   // Energy Returned
      sprintf(telegramLine, "1-0:2.8.1(%s*kWh)\r\n", Format(ER_T1, 10, 3).c_str());
      break;
    case 8:
      sprintf(telegramLine, "1-0:2.8.2(%s*kWh)\r\n", Format(ER_T2, 10, 3).c_str());
      break;
    case 9:   // Tariff indicator electricity
      sprintf(telegramLine, "0-0:96.14.0(%04d)\r\n", ETariffInd);
      break;
    case 10:  // Actual electricity power delivered (+P) in 1 Watt resolution
      sprintf(telegramLine, "1-0:1.7.0(%s*kW)\r\n", Format(PDelivered, 6, 2).c_str());
      break;
    case 11:  // Actual electricity power received (-P) in 1 Watt resolution
      sprintf(telegramLine, "1-0:2.7.0(%s*kW)\r\n", Format(PReceived, 6, 2).c_str());
      break;
    case 12:  // Number of power failures in any phase
      sprintf(telegramLine, "0-0:96.7.21(00010)\r\n", val);
      break;
    case 13:  // Number of long power failures in any phase
      sprintf(telegramLine, "0-0:96.7.9(00000)\r\n", val);
      break;
    case 14:  // Power Failure Event Log (long power failures)
      sprintf(telegramLine, "1-0:99.97.0(10)(0-0:96.7.19)(190508094303S)(0000055374*s)"
              "(190507165813S)(0000007991*s)(190507141021S)(0000000274*s)"
              "(190507135954S)(0000000649*s)(190507134811S)(0000083213*s)"
              "(190506143928S)(0000090080*s)(190505123501S)(0000073433*s)"
              "(190504152603S)(0000003719*s)(190504120844S)(0000337236*s)"
              "(190430142638S)(0000165493*s)\r\n", val);
      break;
    case 15:  // Number of voltage sags in phase L1
      sprintf(telegramLine, "1-0:32.32.0(00002)\r\n", val);
      break;
    case 16:  // Number of voltage sags in phase L2 (polyphase meters only)
      sprintf(telegramLine, "1-0:52.32.0(00003)\r\n", val);
      break;
    case 17:  // Number of voltage sags in phase L3 (polyphase meters only)
      sprintf(telegramLine, "1-0:72.32.0(00003)\r\n", val);
      break;
    case 18:  // Number of voltage swells in phase L1
      sprintf(telegramLine, "1-0:32.36.0(00000)\r\n", val);
      break;
    case 19:  // Number of voltage swells in phase L2
      sprintf(telegramLine, "1-0:52.36.0(00000)\r\n", val);
      break;
    case 20:  // Number of voltage swells in phase L3
      sprintf(telegramLine, "1-0:72.36.0(00000)\r\n", val);
      break;
    case 21:  // Text message max 2048 characters
      sprintf(telegramLine, "0-0:96.13.0()\r\n", val);
      break;
    case 22:  // Instantaneous voltage L1 in 0.1V resolution
      sprintf(telegramLine, "1-0:32.7.0(%5.1f*V)\r\n", (float)(random(219, 240) + (random(-30, 30)/10.0)));
      break;
    case 23:  // Instantaneous voltage L1 in 0.1V resolution
      sprintf(telegramLine, "1-0:52.7.0(%5.1f*V)\r\n", (float)(random(221, 230) + (random(-30, 30)/10.0)));
      break;
    case 24:  // Instantaneous voltage L1 in 0.1V resolution
      sprintf(telegramLine, "1-0:72.7.0(%5.1f*V)\r\n", (float)(random(223, 240) + (random(-30, 30)/10.0)));
      break;
    case 25:  // Instantaneous current L1 in A resolution
      sprintf(telegramLine, "1-0:31.7.0(%03d*A)\r\n", random(1, 5));
      break;
    case 26:  // Instantaneous current L2 in A resolution
      sprintf(telegramLine, "1-0:51.7.0(%03d*A)\r\n", random(0, 4));
      break;
    case 27:  // Instantaneous current L3 in A resolution
      sprintf(telegramLine, "1-0:71.7.0(%03d*A)\r\n", random(0, 4));
      break;

    case 28:  // Instantaneous active power L1 (+P) in W resolution
      sprintf(telegramLine, "1-0:21.7.0(%s*kW)\r\n", Format(IPD_l1, 6, 3).c_str());
      break;
    case 29:  // Instantaneous active power L2 (+P) in W resolution
      sprintf(telegramLine, "1-0:41.7.0(%s*kW)\r\n", Format(IPD_l2, 6, 3).c_str());
      break;
    case 30:  // Instantaneous active power L3 (+P) in W resolution
      sprintf(telegramLine, "1-0:61.7.0(%s*kW)\r\n", Format(IPD_l3, 6, 3).c_str());
      break;

    case 31:  // Instantaneous active power L1 (-P) in W resolution
      sprintf(telegramLine, "1-0:22.7.0(%s*kW)\r\n", Format(IPR_l1, 6, 3).c_str());
      break;
    case 32:  // Instantaneous active power L2 (-P) in W resolution
      sprintf(telegramLine, "1-0:42.7.0(%s*kW)\r\n", Format(IPR_l2, 6, 3).c_str());
      break;
    case 33:  // Instantaneous active power L3 (-P) in W resolution
      sprintf(telegramLine, "1-0:62.7.0(%s*kW)\r\n", Format(IPR_l3, 6, 3).c_str());
      break;

    case 34:  // Gas Device-Type
      sprintf(telegramLine, "0-%d:24.1.0(003)\r\n", actGasMBus, val);
      break;
    case 35:  // Equipment identifier (Gas)
      sprintf(telegramLine, "0-%d:96.1.0(4730303339303031363532303530323136)\r\n", actGasMBus, val);
      break;
    case 36:  //  0-n:24.4.0(1) - gas_valve_position
      sprintf(telegramLine, "0-%d:24.4.0(%1d)\r\n", actGasMBus, (telegramCount%2));
      break;
    case 37:  // Last 5-minute value (temperature converted), gas delivered to client
      // in m3, including decimal values and capture time (Note: 4.x spec has
      sprintf(telegramLine, "0-%d:24.2.1(%02d%02d%02d%02d%02d01S)(%s*m3)\r\n", actGasMBus
              , (year() - 2000), month(), day(), hour(), minute()
              , Format(GDelivered, 9, 3).c_str());
      break;

    case 38:  // [4] Device-Type
      sprintf(telegramLine, "0-4:24.1.0(005)\r\n", val);
      break;
    case 39:  // Equipment identifier [4]
      sprintf(telegramLine, "0-4:96.1.0(4730303339303031344444444444444444)\r\n", val);
      break;
    case 40:  //  0-4:24.4.0(1) - spare [4] valve_position
      sprintf(telegramLine, "0-4:24.4.0(%1d)\r\n", (telegramCount%2));
      break;
    case 41:  // [4]
      sprintf(telegramLine, "0-4:24.2.1(%02d%02d%02d%02d%02d01S)(%s*GJ)\r\n", (year() - 2000), month(), day(), hour(), minute(),
              Format((GDelivered / 5), 9, 3).c_str());
      break;

    case 42:
      //sprintf(telegramLine, "!xxxx\r\n");
      sprintf(telegramLine, "!\0\0\0\0\0\0");
      break;

  } // switch(line)

  maxLines50 = 42;
  
  if (line <= maxLines50)
  {
    if (Verbose && ((telegramCount % 3) == 0))
    {
      if (line == 0)
      {
        Debugf("\r\nTelegram [#%d]\r\n", telegramCount);
      }
      Debug(telegramLine);
    }
  }
  for(len = 0; len < _MAX_LINE_LEN, telegramLine[len] != '\0'; len++) {}
  return len;

} // buildTelegram50()


//==================================================================================================
int16_t buildTelegramBE(int16_t line, char telegramLine[])
{
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
  **  1-0:31.7.0(002.19*A)                                - current_l1
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
  **  1-0:51.7.0(002.32*A)                                - current_l2
  **  1-0:71.7.0(001*A)                                   - current_l3
  **  0-0:96.3.10(1)                                      - electricity_switch_position
  **  0-0:17.0.0(999.9*kW)                                - electricity_threshold
  **  1-0:31.4.0(999*A)                                   - fuse_treshold_l1
  **  0-0:96.13.0()                                       - message_long
  **  0-1:24.1.0(003)                                     - gas_device_type
  **  0-1:96.1.1(37464C4F32313139303935373430)            - gas_equipment_id (ntc)
  **  0-1:24.4.0(1)                                       - gas_valve_position
  **  0-1:24.2.3(191204184600W)(00070.043*m3)             - gas_delivered (ntc)
  **  !6E84
  */
  int16_t len = 0;
  float val;

  memset(telegramLine, 0, sizeof(telegramLine));

  switch (line)
  {
    //FLU5\253769484_A
    case 0:
      sprintf(telegramLine, "/FLU5\\253769484_A\r\n");
      break;
    case 1:
      sprintf(telegramLine, "\r\n");
      break;
    case 2:
      sprintf(telegramLine, "0-0:96.1.4(50213)\r\n");
      break;
    case 3:
      sprintf(telegramLine, "0-0:1.0.0(%02d%02d%02d%02d%02d%02dS)\r\n", (year() - 2000), month(), day(), hour(), minute(), thisSecond);
    //sprintf(telegramLine, "0-0:1.0.0(%02d%02d%02d%02d%02d%02d%c)\r\n", (year() - 2000), month(), day(), hour(), minute(), thisSecond, isDST()?'S':'W');
      break;
    case 4:
      sprintf(telegramLine, "0-0:96.1.1(4530303336303000000000000000000502)\r\n", val);
      break;
    case 5:   // Energy Delivered
      sprintf(telegramLine, "1-0:1.8.1(%s*kWh)\r\n", Format(ED_T1, 10, 3).c_str());
      break;
    case 6:
      sprintf(telegramLine, "1-0:1.8.2(%s*kWh)\r\n", Format(ED_T2, 10, 3).c_str());
      break;
    case 7:   // Energy Returned
      sprintf(telegramLine, "1-0:2.8.1(%s*kWh)\r\n", Format(ER_T1, 10, 3).c_str());
      break;
    case 8:
      sprintf(telegramLine, "1-0:2.8.2(%s*kWh)\r\n", Format(ER_T2, 10, 3).c_str());
      break;
    case 9:   // Tariff indicator electricity
      sprintf(telegramLine, "0-0:96.14.0(%04d)\r\n", ETariffInd);
      break;
    case 10:  // Actual electricity power delivered (+P) in 1 Watt resolution
      sprintf(telegramLine, "1-0:1.7.0(%s*kW)\r\n", Format(PDelivered, 6, 2).c_str());
      break;
    case 11:  // Actual electricity power received (-P) in 1 Watt resolution
      sprintf(telegramLine, "1-0:2.7.0(%s*kW)\r\n", Format(PReceived, 6, 2).c_str());
      break;
    case 12:  // Number of power failures in any phase
      sprintf(telegramLine, "0-0:96.7.21(00010)\r\n", val);
      break;
    case 13:  // Number of long power failures in any phase
      sprintf(telegramLine, "0-0:96.7.9(00000)\r\n", val);
      break;
    case 14:  // Power Failure Event Log (long power failures)
      sprintf(telegramLine, "1-0:99.97.0(0)(0-0:96.7.19)\r\n", val);
      break;
    case 15:  // Number of voltage sags in phase L1
      sprintf(telegramLine, "1-0:32.32.0(00002)\r\n", val);
      break;
    case 16:  // Number of voltage sags in phase L2 (polyphase meters only)
      sprintf(telegramLine, "1-0:52.32.0(00003)\r\n", val);
      break;
    case 17:  // Number of voltage sags in phase L3 (polyphase meters only)
      sprintf(telegramLine, "1-0:72.32.0(00004)\r\n", val);
      break;
    case 18:  // Number of voltage swells in phase L1
      sprintf(telegramLine, "1-0:32.36.0(00003)\r\n", val);
      break;
    case 19:  // Number of voltage swells in phase L2
      sprintf(telegramLine, "1-0:52.36.0(00002)\r\n", val);
      break;
    case 20:  // Number of voltage swells in phase L3
      sprintf(telegramLine, "1-0:72.36.0(00001)\r\n", val);
      break;
    case 21:  // Instantaneous voltage L1 in 0.1V resolution
      sprintf(telegramLine, "1-0:32.7.0(%5.1f*V)\r\n", (float)(random(219, 240) + (random(-30, 30)/10.0)));
      break;
    case 22:  // Instantaneous voltage L1 in 0.1V resolution
      sprintf(telegramLine, "1-0:52.7.0(%5.1f*V)\r\n", (float)(random(219, 240) + (random(-30, 30)/10.0)));
      break;
    case 23:  // Instantaneous voltage L1 in 0.1V resolution
      sprintf(telegramLine, "1-0:72.7.0(%5.1f*V)\r\n", (float)(random(219, 240) + (random(-30, 30)/10.0)));
      break;
    case 24:  // Instantaneous current L1 in A resolution
      sprintf(telegramLine, "1-0:31.7.0(%03.2f*A)\r\n", (float)(random(100, 4000)/1000.0));
      break;
    case 25:  // Instantaneous current L2 in A resolution
      sprintf(telegramLine, "1-0:51.7.0(%03d*A)\r\n", random(0, 4));
      break;
    case 26:  // Instantaneous current L3 in A resolution
      sprintf(telegramLine, "1-0:71.7.0(%03.2f*A)\r\n", (float)(random(0, 3500)/1000.0));
      break;
    case 27:  // Instantaneous active power L1 (+P) in W resolution
      sprintf(telegramLine, "1-0:21.7.0(%s*kW)\r\n", Format(IPD_l1, 6, 3).c_str());
      break;
    case 28:  // Instantaneous active power L3 (+P) in W resolution
      sprintf(telegramLine, "1-0:61.7.0(%s*kW)\r\n", Format(IPD_l3, 6, 3).c_str());
      break;
    case 29:  // Instantaneous active power L1 (-P) in W resolution
      sprintf(telegramLine, "1-0:22.7.0(%s*kW)\r\n", Format(IPR_l1, 6, 3).c_str());
      break;
    case 30:  // Instantaneous active power L3 (-P) in W resolution
      sprintf(telegramLine, "1-0:62.7.0(%s*kW)\r\n", Format(IPR_l3, 6, 3).c_str());
      break;
    case 31:  // electricity_switch_position
      sprintf(telegramLine, "0-0:96.3.10(1)\r\n", val);
      break;
    case 32:  // electricity_threshold
      sprintf(telegramLine, "0-0:17.0.0(999.9*kW)\r\n", val);
      break;
    case 33:  // fuse_treshold_l1
      sprintf(telegramLine, "1-0:31.4.0(999*A)\r\n", val);
      break;
    case 34:  // message_long
      sprintf(telegramLine, "0-0:96.13.0()\r\n", val);
      break;
    case 35:  // Gas Device-Type
      sprintf(telegramLine, "0-%d:24.1.0(003)\r\n", actGasMBus, val);
      break;
    case 36:  // Equipment identifier BE (Gas)
      sprintf(telegramLine, "0-%d:96.1.1(37464C4F323131393039353734BE)\r\n", actGasMBus, val);
      break;
    case 37:  // gas_valve_position
      sprintf(telegramLine, "0-%d:24.4.0(%1d)\r\n", actGasMBus, (telegramCount%2));
      break;
    case 38:  // gas_delivered
      sprintf(telegramLine, "0-%d:24.2.3(%02d%02d%02d%02d%02d00S)(%s*m3)\r\n", actGasMBus
              , (year() - 2000), month(), day(), hour(), minute()
              , Format(GDelivered, 9, 3).c_str());
      break;
    case 39:  // mbus-Type [4]
      sprintf(telegramLine, "0-4:24.1.0(005)\r\n", val);
      break;
    case 40:  // Equipment identifier BE (4)
      sprintf(telegramLine, "0-4:96.1.0(37464C4F3231314940494547BE40)\r\n", val);
      break;
    case 41:  // [4]_valve_position
      sprintf(telegramLine, "0-4:24.4.0(%1d)\r\n", (telegramCount%2));
      break;
    case 42:  // [4]
      sprintf(telegramLine, "0-4:24.2.1(%02d%02d%02d%02d%02d00S)(%s*m3)\r\n", (year() - 2000), month(), day(), hour(), minute(),
              Format((GDelivered * 1.3), 9, 3).c_str());
      break;
    case 43:
      sprintf(telegramLine, "!\0\0\0");
      break;

  } // switch(line)

  maxLinesBE = 43;

  if (line <= maxLinesBE)   // from line 0 upto line 38!
  {
    if (Verbose && ((telegramCount % 3) == 0))
    {
      if (line == 0)
      {
        Debugln();
      }
      Debug(telegramLine);
    }
  }

  for(len = 0; len < _MAX_LINE_LEN, telegramLine[len] != '\0'; len++) {}

  return len;

} // buildTelegramBE()



//==================================================================================================
int16_t buildTelegram30(int16_t line, char telegramLine[])
{
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

  memset(telegramLine, 0, sizeof(telegramLine));

  switch (line)
  {
    //KMP5 KA6U003030303030
    case 0:
      sprintf(telegramLine, "/KMP5 KA6U003030303030\r\n");
      break;
    case 1:
      sprintf(telegramLine, "\r\n");
      break;
    case 2:
      sprintf(telegramLine, "0-0:96.1.1(4530303336303000000000000000000000)\r\n");
      break;
    case 3:   // Energy Delivered
      sprintf(telegramLine, "1-0:1.8.1(%s*kWh)\r\n", Format(ED_T1, 10, 3).c_str());
      break;
    case 4:
      sprintf(telegramLine, "1-0:1.8.2(%s*kWh)\r\n", Format(ED_T2, 10, 3).c_str());
      break;
    case 5:   // Energy Returned
      sprintf(telegramLine, "1-0:2.8.1(%s*kWh)\r\n", Format(ER_T1, 10, 3).c_str());
      break;
    case 6:
      sprintf(telegramLine, "1-0:2.8.2(%s*kWh)\r\n", Format(ER_T2, 10, 3).c_str());
      break;
    case 7:   // Tariff indicator electricity
      sprintf(telegramLine, "0-0:96.14.0(%04d)\r\n", ETariffInd);
      break;
    case 8:   // Actual electricity power delivered (+P) in 1 Watt resolution
      sprintf(telegramLine, "1-0:1.7.0(%s*kW)\r\n", Format(PDelivered, 6, 2).c_str());
      break;
    case 9:   // Actual electricity power received (-P) in 1 Watt resolution
      sprintf(telegramLine, "1-0:2.7.0(%s*kW)\r\n", Format(PReceived, 6, 2).c_str());
      break;
    /*******
    case xx:  // Max current per phase (999=no max)
              sprintf(telegramLine, "0-0:17.0.0(016*A)\r\n", val);
              break;
    *******/
    case 10:  // Switch position (?)
      sprintf(telegramLine, "0-0:96.3.10(1)\r\n");
      break;
    case 11:  // Text message code
      sprintf(telegramLine, "0-0:96.13.1()\r\n");
      break;
    case 12:  // Text message text
      sprintf(telegramLine, "0-0:96.13.0()\r\n");
      break;
    case 13:  // Gas Device-Type
      sprintf(telegramLine, "0-%d:24.1.0(3)\r\n", actGasMBus);
      break;
    case 14:  // Equipment identifier (Gas)
      sprintf(telegramLine, "0-%d:96.1.0(4730303339303031363500000000000000)\r\n", actGasMBus);
      break;
    case 15:  // Last 5-minute value (temperature converted), gas delivered to client
      // in m3, including decimal values and capture time
      sprintf(telegramLine, "0-%d:24.3.0(%02d%02d%02d%02d%02d00)(08)(60)(1)(0-%d:24.2.1)(m3)\r\n", actGasMBus
              , (year() - 2000), month(), day(), hour(), minute()
              , actGasMBus);
      break;
    case 16:
      sprintf(telegramLine, "(%s)\r\n", Format(GDelivered, 9, 3).c_str());
      break;
    case 17:  // Gas valve position
      sprintf(telegramLine, "0-%d:24.4.0(1)\r\n", actGasMBus, val);
      break;
    case 18:  // meter Device-Type (4)
      sprintf(telegramLine, "0-4:24.1.0(005)\r\n");
      break;
    case 19:  // Equipment identifier (4)
      sprintf(telegramLine, "0-4:96.1.0(4730303339303031363500004444444444)\r\n");
      break;
    case 20:  // value (temperature converted), (4)
      sprintf(telegramLine, "0-4:24.3.0(%02d%02d%02d%02d%02d00)(08)(60)(1)(0-4:24.2.1)(m3)\r\n"
              , (year() - 2000), month(), day(), hour(), minute());
      break;
    case 21:
      sprintf(telegramLine, "(%s)\r\n", Format((GDelivered/3), 9, 3).c_str());
      break;
    case 22:  // valve position (4)
      sprintf(telegramLine, "0-4:24.4.0(1)\r\n");
      break;
    case 23:
      sprintf(telegramLine, "!\r\n\0\0\0");     
      break;

  } // switch(line)

  maxLines30 = 23;

  if (line <= maxLines30)
  {
    if (Verbose && ((telegramCount % 3) == 0))
    {
      if (line == 0)
      {
        Debugln();
      }
      Debug(telegramLine);
    }
  }

  for(len = 0; len < _MAX_LINE_LEN, telegramLine[len] != '\0'; len++) {}

  return len;

} // buildTelegram30()


//--------------------------------------------------------------------
void readTelegramFromFile(char *tlgrmFileName)
{
  bool eof = false, telegramLineEnd = false, telegramLineStart = false;
  char oneLine[_MAX_LINE_LEN];

  if (strlen(tlgrmFileName) == 0)
  {
    telegramFile.close();
    sprintf(actDSMR, "50");
    return;
  }

  if (!telegramFile)
  {
    //Debugf("open(%s, 'r') ", tlgrmFileName);
    telegramFile = FSYS.open(tlgrmFileName, "r"); // open for reading and writing
    if (!telegramFile)
    {
      Serial.printf("open(%s, 'r')-> FAILED!!! --> Bailout\r\n", tlgrmFileName);
      Debugf(" FAILED!!! --> Bailout\r\n");
      memset(tlgrmFileName, 0, sizeof(tlgrmFileName));
      sprintf(actDSMR, "50");
      return;
    }
    yield();
    Debugln();
    recCount = 1;
  }

  //Debugln("Read lines from telegramFile ..");
  while (!telegramLineEnd)
  {
    memset(oneLine, 0, sizeof(oneLine));

    if (!telegramFile.available())
    {
      eof = true;
      break;
    }

    int len = telegramFile.readBytesUntil('\n', oneLine, sizeof(oneLine));

    if (oneLine[0] == '/')
    {
      telegramLineStart = true;
    }

    if (telegramLineStart)
    {
      if (oneLine[0] == '!') telegramLineEnd   = true;
      snprintf(telegramLine, sizeof(telegramLine), "%s\r\n", oneLine);
      len += 2;
      //Debugf("(%2d) %s", len, telegramLine);  // frustrates processing???
      calcCRC = decodeTelegram(len);
    }
    if (telegramLineEnd)
    {
      if (skipChecksum)
            P1_OUT.printf("!\r\n\r\n");
      else  P1_OUT.printf("!%04X\r\n\r\n", (calcCRC & 0xFFFF));
    }
    else
    {
      P1_OUT.print(telegramLine);
      memset(telegramLine, 0, sizeof(telegramLine));
    }
    yield();

  } // while ...

  if (!telegramFile.available())
  {
    Debugln("\r\nRewind telegramFile ..");
    DebugFlush();
    telegramFile.seek(0);
    recCount = 0;
  }

} //  readTelegramFromFile()
