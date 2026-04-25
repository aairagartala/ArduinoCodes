const char MAIN_page[] PROGMEM = R"=====( 


<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>ESP32 RTC Controller</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    body {
      font-family: "Segoe UI", sans-serif;
      background: linear-gradient(to right, #141e30, #243b55);
      color: #fff;
      text-align: center;
      margin: 0;
      padding: 0;
    }
    .container {
      max-width: 420px;
      margin: 40px auto;
      background: rgba(255, 255, 255, 0.07);
      padding: 20px;
      border-radius: 16px;
      box-shadow: 0 4px 20px rgba(0,0,0,0.3);
    }
    h1 {
      font-size: 1.6rem;
      margin-bottom: 10px;
    }
    .clock {
      font-size: 2rem;
      font-weight: bold;
      margin: 10px 0;
    }
    .dateDisplay {
      font-size: 1.2rem;
      margin-bottom: 20px;
      opacity: 0.9;
    }
    input, button {
      width: 100%;
      padding: 10px;
      margin: 8px 0;
      border: none;
      border-radius: 8px;
      font-size: 1rem;
    }
    input {
      background: #fff;
      color: #000;
    }
    button {
      background: #00c6ff;
      color: #fff;
      font-weight: bold;
      cursor: pointer;
      transition: background 0.3s;
    }
    button:hover {
      background: #0072ff;
    }

     .displayBox {
    margin: 10px 0;
    text-align: left;
  }
  .displayBox label {
    font-weight: bold;
    display: block;
    margin-bottom: 5px;
  }
  .timeBox {
    background: #fff;
    color: #000;
    padding: 10px;
    border-radius: 8px;
    font-size: 1.1rem;
    text-align: center;
    box-shadow: 0 2px 6px rgba(0,0,0,0.2);
  }
  </style>
</head>
<body>
  <div class="container">
    <h1>ESP32 RTC Controller</h1>
    <div class="clock" id="clockDisplay">--:--:--</div>
    <div class="dateDisplay" id="dateDisplay">--/--/----</div>
    
    <h3>Set Date & Time</h3>
    <input type="date" id="dateInput">
    <input type="time" id="timeInput">
    <button onclick="sendRTC()">Update RTC</button>

    

    <h3>On/Off Time</h3>

    <div class="displayBox">
    <label>On Time:</label>
    <div class="timeBox" id="onTimeDisplay"></div>
    </div>

    <div class="displayBox">
    <label>Off Time:</label>
    <div class="timeBox" id="offTimeDisplay">21:00</div>
    </div>
    
  </div>

  <script>
    // ----------- FETCH TIME & DATE FROM ESP32 -------------
    function getDataRTC() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          // Example response: "14:25:36-23/08/2025"
          let resp = this.responseText.trim();
          let parts = resp.split("-");
          if (parts.length == 2) {
            document.getElementById("clockDisplay").innerHTML = parts[0];
            document.getElementById("dateDisplay").innerHTML = parts[1];
          }
        }
      };
      xhttp.open("GET", "getrtc", true); // <-- ESP32 must serve this
      xhttp.send();
    }
    setInterval(getDataRTC, 1000); 
    getDataRTC();

    // ----------- FETCH ON TIME -------------
    function getDataRTC_on() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          // Example response: "06:30"
           {
            document.getElementById("onTimeDisplay").innerHTML=this.responseText.trim();
            
          }
        }
      };
      xhttp.open("GET", "bl", true); // <-- ESP32 must serve this
      xhttp.send();
    }
    setInterval(getDataRTC_on, 1000); 
    getDataRTC_on();

    // ----------- SEND DATE & TIME TO ESP32 -------------
    function sendRTC() {
      let dateVal = document.getElementById("dateInput").value;  // yyyy-mm-dd
      let timeVal = document.getElementById("timeInput").value;  // hh:mm

      if (!dateVal || !timeVal) {
        alert("Please select both date and time");
        return;
      }

      // Split date
      let [year, month, day] = dateVal.split("-");
      month = parseInt(month, 10); // convert Jan=01 → 1
      day = parseInt(day, 10);

      // Split time
      let [hour, minute] = timeVal.split(":");
      hour = parseInt(hour, 10);
      minute = parseInt(minute, 10);

      // Seconds reset to 0
      let second = 0;

      // Send individually (your style GET requests)
       
       sendData2(year);
      sendData3(month);
         sendData4(day);

          
         sendData5(hour);
         sendData6(minute);
        sendData7(second);
      // sendGET("setDay?day="+day);
      // sendGET("setMonth?month="+month);
      // sendGET("setYear?year="+year);
      // sendGET("setHour?hour="+hour);
      // sendGET("setMinute?minute="+minute);
      // sendGET("setSecond?second="+second);
      


      }

      function sendData2(year)
{

     var xhttp1=new XMLHttpRequest();
    xhttp1.onreadystatechange=function(){
    if (this.readyState==4 && this.status == 200){
       document.getElementById("phstate1").inner.HTML=
       this.responseText;
       }
       
  };

xhttp1.open("GET", "setph1?phstate1="+year, true);
  xhttp1.send();

}


function sendData3(month)
{

     var xhttp1=new XMLHttpRequest();
    xhttp1.onreadystatechange=function(){
    if (this.readyState==4 && this.status == 200){
       document.getElementById("phstate2").inner.HTML=
       this.responseText;
       }
       
  };

     xhttp1.open("GET", "setph2?phstate2="+month, true);
  xhttp1.send();

}
  

  function sendData4(day)
{

     var xhttp1=new XMLHttpRequest();
    xhttp1.onreadystatechange=function(){
    if (this.readyState==4 && this.status == 200){
       document.getElementById("phstate3").inner.HTML=
       this.responseText;
       }
       
  };

     xhttp1.open("GET", "setph3?phstate3="+day, true);
  xhttp1.send();

}
  

  function sendData5(hour)
{

     var xhttp1=new XMLHttpRequest();
    xhttp1.onreadystatechange=function(){
    if (this.readyState==4 && this.status == 200){
       document.getElementById("phstate4").inner.HTML=
       this.responseText;
       }
       
  };

     xhttp1.open("GET", "setph4?phstate4="+hour, true);
  xhttp1.send();

}
  

  function sendData6(minute)
{

     var xhttp1=new XMLHttpRequest();
    xhttp1.onreadystatechange=function(){
    if (this.readyState==4 && this.status == 200){
       document.getElementById("phstate5").inner.HTML=
       this.responseText;
       }
       
  };

     xhttp1.open("GET", "setph5?phstate5="+minute, true);
  xhttp1.send();

}
  

  function sendData7(second)
{

     var xhttp1=new XMLHttpRequest();
    xhttp1.onreadystatechange=function(){
    if (this.readyState==4 && this.status == 200){
       document.getElementById("phstate6").inner.HTML=
       this.responseText;
       }
       
  };

     xhttp1.open("GET", "setph6?phstate6="+second, true);
  xhttp1.send();
}
  


  </script>
</body>
</html>



)=====";
 