static const char DSMRindex_html[] PROGMEM =
  R"(<html charset="UTF-8">
  <style type='text/css'>
    <!--font-size: 20px;-->
    table tbody tr td {
      font-size: 20px;
    }
    body {
      background-color: lightblue;
    }
  </style>
<!-- ------------------------------------------------------------ -->
     <body>
      <div class="header">
        <h1>
          <span id="devName">DSMR Injector</span> &nbsp; &nbsp; &nbsp;
          <span id="devVersion">[version]</span> &nbsp; &nbsp; &nbsp;
          <span id="devIPaddress" style='font-size: small;'>-</span> &nbsp;
        </h1>
       </div>
     <div>
      <form action = '' name='mainForm' method = 'get'>
         <table>
         <tr>
         <td style='width:150px;'>actueel Jaar</td>
         <td style='width:300px;'>
            <input id='actYear' type='number' style='font-size:14px;' name='actYear' min='2014' max='2099' 
                                                onchange='validateField( "actYear" )'>
         </tr><tr>
         <td>actuele Maand</td>
         <td><input id='actMonth' type='number' style='font-size:14px;' name='actMonth' min='1' max='12' 
                                                onchange='validateField( "actMonth" )'></td>
         </tr><tr>
         <td>actuele Dag</td>
         <td><input id='actDay' type='number'   style='font-size:14px;' name='actDay' min='1' max='31' 
                                                onchange='validateField( "actDay" )'></td>
         </tr><tr>
         <td>actueel Uur</td>
         <td><input id='actHour' type='number'   style='font-size:14px;' name='actHour' min='0' max='24' 
                                                onchange='validateField( "actHour" )'></td>
         </tr><tr>
         <td>Stappen in Minuten</td>
         <td><input id='actSpeed' type='number'   style='font-size:14px;' name='actSpeed' min='1' max='15' 
                                                onchange='validateField( "actSpeed" )'></td>
         </tr><tr>
         <td><hr></td><td style='width:300px;'><hr><td>
         </tr><tr>
         <td valign='top'>Functie</td>
         <td>
              <input type="radio" id="doNormal" style='font-size:14px;' name="runMode" value="doNormal" checked
                                          onchange='doRunMode( "runMode" )'>
              <label for="doNormal">Normal Operation</label>
              <br/>
              <input type="radio" id="doMonths" style='font-size:14px;' name="runMode" value="doMonths"
                                          onchange='doRunMode( "runMode" )'>
              <label for="doMonths">vul Maanden</label>
              <br/>
              <input type="radio" id="doDays"   style='font-size:14px;' name="runMode" value="doDays"
                                          onchange='doRunMode( "runMode" )'>
              <label for="doDays">vul Dagen</label>
              <br/>
              <input type="radio" id="doHours"  style='font-size:14px;' name="runMode" value="doHours"
                                          onchange='doRunMode( "runMode" )'>
              <label for="doHours">vul Uren</label>
         </td>
         </tr><tr>
         <td><hr></td><td><hr><td>
         </tr><tr>
         <td style='width:120px;' valign='top'>DSMR standaard</td>
         <td>
             <select id='actDSMR' style='font-size:14px;' onchange='doDSMRstandard()'>
                <option value='40'>DSMR 4.0+</option>
                <option value='30'>DSMR 3.0</option>
              </select>
         </td>
         </tr><tr>
         <td><hr></td><td style='width:300px;'><hr><td>
         </tr><tr>
         <td style='width:120px;' valign='top'>run Status</td>
         <td>
              <input type="radio" id="doStop" style='font-size:14px;' name="runStatus" value="doStop" checked
                                          onchange='doRunStatus( "runStatus" )'>
              <label for="doStop">Stop</label>
              <br/>
              <input type="radio" id="doStart" style='font-size:14px;' name="runStatus" value="doStart"
                                          onchange='doRunStatus( "runStatus" )'>
              <label for="doStart">Run</label>
         </td>
         </tr><tr>
         <td><hr></td><td style='width:300px;'><hr><td>
         </tr><tr>
         <td style='width:120px;' valign='top'>Interval (sec.)</td>
         <td><input id='actInterval' type='number'   style='font-size:14px;' name='actinterval' min='1' max='20' 
                                          onchange='validateField( "actInterval" )'></td>
         </td>
         </tr><tr>
         <td><hr></td><td style='width:300px;'><hr><td>
         </tr><tr>
         <td style='width:120px;' valign='top'>timeStamp</td>
         <td>
            [<span id="timeStamp">?</span>]
         </td>
         </tr><tr>
         <td style='width:120px;' valign='top'>Telegrammen</td>
         <td>
            [<span id=telegramCount>-</span>] verstuurd
         </td>
         </tr><tr>
         <td><hr></td><td style='width:300px;'><hr><td>
         </tr>
         </table>
      </form>
     </div>
     <br>
      <div>
        <form action='/update' method='GET'><big>Update Firmware </big>
          <input type='submit' class='button' name='SUBMIT' value='select Firmware' ENABLED/>
        </form>

        <form action='/ReBoot' method='POST'>ReBoot DSMRinjector 
          <input type='submit' class='button' name='SUBMIT' value='ReBoot'>
        </form>
      </div>
     <br>
<!-- ------------------------------------------------------------ -->
<script>
"use strict";

  let webSocketConn;
  let needReload  = true;
  let singlePair;
  let onePair;

  window.onload=bootsTrap;
  window.onfocus = function() {
    if (needReload) {
      window.location.reload(true);
    }
  };
    
  
  function bootsTrap() {
    let count = 0;
    while ( document.getElementById('devVersion').value == "[version]" ) {
      count++;
      console.log( "count ["+count+"] devVersion is ["+document.getElementById('devVersion').value+"]" );
      if (count > 10) {
        alert( "Count="+count+" => reload from server!" );
        window.location.reload(true);
      }
      setTimeout("", 500);
    }

  }; // bootsTrap()
  
  webSocketConn = new WebSocket( 'ws://'+location.host+':81/', ['arduino'] );
  console.log( "WebSocket('ws://"+location.host+":81/', ['arduino']) " );
  
  webSocketConn.onopen    = function () { 
    console.log( "Connected!" );
    webSocketConn.send('Connect ' + new Date()); 
    console.log( "getDevInfo" );
    webSocketConn.send( "getDevInfo" );
    needReload  = false;

  }; 
  
  webSocketConn.onclose     = function () { 
    console.log( " " );
    console.log( "Disconnected!" );
    console.log( " " );
    needReload  = true;
    let redirectButton = "<p></p><hr><p></p><p></p>"; 
    redirectButton    += "<style='font-size: 50px;'>Disconneted from DSMR Injector"; 
    redirectButton    += "<input type='submit' value='re-Connect' "; 
    redirectButton    += " onclick='window.location=\"/\";' />  ";     
    redirectButton    += "<p></p><p></p><hr><p></p>"; 
    document.getElementById( "Actual" ).innerHTML = redirectButton;

  }; 
  webSocketConn.onerror   = function (error)  { 
    console.log( "Error: " + error);
    console.log('WebSocket Error ', error);
  };
  webSocketConn.onmessage = function (e) {
    needReload = false;
    parsePayload(e.data); 
  };

  function parsePayload(payload) {
    console.log( "parsePayload["+payload+"]" );

    singlePair   = payload.split("," );
    var msgType  = singlePair[0].split("=" );

    if (msgType[1] == "devInfo" ) {
        console.log("devInfo: "+devIPaddress+","+devVersion);
        for ( var i = 1; i < singlePair.length; i++ ) {
          onePair = singlePair[i].split("=" );
          console.log("set["+onePair[0].trim()+"] to["+onePair[1].trim()+"]" );
          document.getElementById(onePair[0].trim()).innerHTML = onePair[1].trim();
          document.getElementById(onePair[0].trim()).setAttribute('value',onePair[1].trim());
        }
    }

    if (msgType[1] == "timeStamp" ) {
        console.log("timeStamp: "+payload);
        for ( var i = 1; i < singlePair.length; i++ ) {
          onePair = singlePair[i].split("=");
          console.log("set["+onePair[0].trim()+"] to["+onePair[1].trim()+"]" );
          if (onePair[0].trim() == "runStatus" ) {
            if (onePair[1].trim() == "1" )
                  document.forms["mainForm"]["doStart"].checked=true;
            else  document.forms["mainForm"]["doStop"].checked=true;
          } else if (onePair[0].trim() == "actDSMR" ) {
            console.log("newDSMR["+onePair[1].trim()+"]" );
            document.getElementById(onePair[0].trim()).value = onePair[1].trim();
            var radios = document.getElementsByName( "runStatus" );
            if (radios[1].checked) // running
                  document.getElementById("actDSMR").disabled=true;
            else  document.getElementById("actDSMR").disabled=false;
          } else {
            document.getElementById(onePair[0].trim()).innerHTML = onePair[1].trim();
            document.getElementById(onePair[0].trim()).setAttribute('value',onePair[1].trim());
          }
          //console.log("Just got new value's, actHour["+document.getElementById('actHour').value+"]" );  
        }
    }
  };

  
  function validateField(field) {
    console.log(" validateField(): ["+field+"]" );
    var newYear     = document.getElementById('actYear').value;  
    var newMonth    = document.getElementById('actMonth').value;  
    var newDay      = document.getElementById('actDay').value;  
    var newHour     = document.getElementById('actHour').value;  
    var newSpeed    = document.getElementById('actSpeed').value;  
    var newInterval = document.getElementById('actInterval').value;  
    console.log("newDate:newYear="+newYear+",newMonth="+newMonth+",newDay="+newDay+",newHour="+newHour+",newSpeed="+newSpeed);
    webSocketConn.send("newDate:newYear="+newYear+",newMonth="+newMonth
                             +",newDay="+newDay+",newHour="+newHour+",newSpeed="+newSpeed+",newInterval="+newInterval);
    
  };  // validateField()

  
  function doRunMode(action) {
    console.log(" doRunMode(): ["+action+"]" );
    var radios = document.getElementsByName(action);
    for (var i = 0, length = radios.length; i < length; i++) {
      if (radios[i].checked) {
        // do whatever you want with the checked radio
        console.log("newMode:runMode="+radios[i].value); 
        webSocketConn.send("newMode:runMode="+radios[i].value); 
        break;
       }
    }
  };  // doRunMode()

  
  function doRunStatus(action) {
    console.log(" doRunStatus(): ["+action+"]" );
    var radios = document.getElementsByName(action);
    for (var i = 0, length = radios.length; i < length; i++) {
      if (radios[i].checked) {
        // do whatever you want with the checked radio
        console.log("newStatus:runStatus="+radios[i].value); 
        webSocketConn.send("newStatus:runStatus="+radios[i].value); 
        break;
       }
    }
  };  // doRunMode()
  
  function doDSMRstandard() {
    var mySelect = document.getElementById('actDSMR');
    var DSMR = mySelect.options[mySelect.selectedIndex].value;
    if (DSMR == "30" )  
          webSocketConn.send("setDSMR:DSMR=30" ); 
    else  webSocketConn.send("setDSMR:DSMR=40" ); 
  }
  
</script>
     </body></html>)";
