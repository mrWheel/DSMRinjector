
static const char UpdateServerIndex[] PROGMEM =
  R"(<html charset="UTF-8">
     <style type='text/css'>
        body {background-color: lightblue;}
     </style>
     <body>
     <h1>DSMRinjector2 Flash utility</h1>
     <form method='POST' action='?cmd=0' enctype='multipart/form-data'>
          Selecteer een "<b>.ino.bin</b>" bestand<br/>
          <input type='file' accept='ino.bin' name='firmware'>
          <input type='submit' value='Flash Firmware'>
      </form>
      <form method='POST' action='?cmd=100' enctype='multipart/form-data'> 
          Selecteer een "<b>.mklittlefs.bin</b>" bestand<br/>
          <input type='file' accept='mklittlefs.bin' name='filesystem'>
          <input type='submit' value='Flash littleFS'>
      </form>
      <hr>
      <br/><font color='red'>Let op!!!</font>
      <br/>Bij het flashen van het LittleFS raakt u de aanwezige bestanden kwijt. 
      <br/>Maak daarom eerst een kopie van deze bestanden (met de littleFSmanager) 
      <br/>en zet de data-bestanden na het flashen van LittleFS weer terug.
      <hr>
      <br/>
      <br/>Wacht nog <span style='font-size: 1.3em;' id="waitSeconds">120</span> seconden ..
      <br>Als het lijkt of er niets gebeurd, wacht dan tot de teller
           op 'nul' staat en klik daarna <span style='font-size:1.3em;'><b><a href="/">hier</a></b></span>!
     </body>
     <script>
         var seconds = document.getElementById("waitSeconds").textContent;
         var countdown = setInterval(function() {
           seconds--;
           document.getElementById('waitSeconds').textContent = seconds;
           if (seconds <= 0) {
              clearInterval(countdown);
              window.location.assign("/")
           }
         }, 1000);
     </script>
     </html>)";

static const char UpdateServerSuccess[] PROGMEM =
  R"(<html charset="UTF-8">
     <style type='text/css'>
        body {background-color: lightgray;}
     </style>
     <body>
     <h1>DSMRinjector2 Flash utility</h1>
     <br/>
     <h2>Update successfull!</h2>
     <br/>
     <br/>Wait for the DSMRinjector2 to reboot and start the HTTP server
     <br/>
     <br>
     <br/>Wacht nog <span style='font-size: 1.3em;' id="waitSeconds">60</span> seconden ..
     <br/>Als het lijkt of er niets gebeurd, wacht dan tot de teller
          op 'nul' staat en klik daarna <span style='font-size:1.3em;'><b><a href="/">hier</a></b></span>!
     </body>
     <script>
         var seconds = document.getElementById("waitSeconds").textContent;
         var countdown = setInterval(function() {
           seconds--;
           document.getElementById('waitSeconds').textContent = seconds;
           if (seconds <= 0) {
              clearInterval(countdown);
              window.location.assign("/")
           }
         }, 1000);
     </script>
     </html>)";
