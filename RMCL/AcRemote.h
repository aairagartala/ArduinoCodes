const char MAIN_page[] PROGMEM = R"=====(

<!DOCTYPE html>
<html>
<head>
  <title>Smart AC Controller</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      font-family: Arial, sans-serif;
      background: #0f172a;
      color: white;
      text-align: center;
    }

    .container {
      background: #1e293b;
      margin: 15px;
      padding: 20px;
      border-radius: 12px;
      box-shadow: 0 0 10px rgba(0,0,0,0.5);
    }

    .row {
      display: flex;
      justify-content: space-around;
      font-size: 22px;
      margin-bottom: 10px;
    }

    select, button {
      padding: 10px;
      margin: 10px;
      border-radius: 8px;
      border: none;
      font-size: 16px;
    }

    button {
      cursor: pointer;
    }

    .on { background: green; color: white; }
    .off { background: red; color: white; }

    .disabled {
      background: grey !important;
      cursor: not-allowed;
    }

    .toggle {
      transform: scale(1.5);
    }
  </style>
</head>

<body>

<h2>ESP32 Smart AC Controller</h2>

<div class="container">
  <div class="row">
    <div>  Temp: <span id="temp">--</span>C</div>
    <div>  Humidity: <span id="hum">--</span>%</div>
  </div>

  <div>
    Cutoff Temp: <span id="cutoff">--</span>C
  </div>
</div>

<div class="container">
  <h3>Set Cutoff Temperature</h3>

  <select id="tempSelect">
    <script>
      for(let i=24;i<=32;i++){
        document.write(`<option value="${i}">${i} C</option>`);
      }
    </script>
  </select>

  <br>
  <button onclick="sendStage()">Set Cutoff Temp</button>
</div>

<div class="container">
  <h3>Automation</h3>
  <label>
    OFF
    <input type="checkbox" id="autoToggle" class="toggle" onchange="toggleAutomation()">
    ON
  </label>
</div>

<div class="container">
  <button id="onBtn" class="on" onclick="sendData(5)">ON</button>
  
  <button id="offBtn" class="off" onclick="sendData(6)">OFF</button>
  
</div>

<script>
let automation = false;

// function setCutoff() {
//   let value = document.getElementById("tempSelect").value;
//   document.getElementById("cutoff").innerText = value;

//   fetch(`/set?temp=${value}`);
// }


// Send Reset Command
function sendData(ph) {
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "setph?phstate="+ph, true);
  xhttp.send();
}








// ----------- SEND SELECTED Temp TO ESP32 -------------
function sendStage() {
  let stageVal = document.getElementById("tempSelect").value;

  if (!stageVal) {
    alert("Please select a Temperature");
    return;
  }

  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      console.log("Stage sent successfully: " + this.responseText);
      // Optional: Show success message or update UI
      alert("Temperature updated: " + stageVal);
    }
  };

  xhttp.open("GET", "setstage?stage=" + stageVal, true);  
  xhttp.send();
}


function toggleAutomation() {
  automation = document.getElementById("autoToggle").checked;

  let onBtn = document.getElementById("onBtn");
  let offBtn = document.getElementById("offBtn");

  if (!automation) {
    onBtn.classList.add("disabled");
    offBtn.classList.add("disabled");
    onBtn.disabled = true;
    offBtn.disabled = true;
  } else {
    onBtn.classList.remove("disabled");
    offBtn.classList.remove("disabled");
    onBtn.disabled = false;
    offBtn.disabled = false;
  }

  fetch(`/automation?state=${automation}`);
}

function manualControl(cmd) {
  fetch(`/control?state=${cmd}`);
}

// Example live update (replace with real API)

function gettemph() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          // Example response: "06:30"
           {
            document.getElementById("temp").innerHTML=this.responseText.trim();
          }
        }
      };
      xhttp.open("GET", "b4", true); // <-- ESP32 must serve this
      xhttp.send();
    }
    setInterval(gettemph, 1000); 
    gettemph();


  function gethum() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          // Example response: "06:30"
           {
            document.getElementById("hum").innerHTML=this.responseText.trim();
          }
        }
      };
      xhttp.open("GET", "b16", true); // <-- ESP32 must serve this
      xhttp.send();
    }
    setInterval(gethum, 1000); 
    gethum();


function getct() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          // Example response: "06:30"
           {
            document.getElementById("cutoff").innerHTML=this.responseText.trim();
          }
        }
      };
      xhttp.open("GET", "b6", true); // <-- ESP32 must serve this
      xhttp.send();
    }
    setInterval(getct, 1000); 
    getct();

</script>

</body>
</html>

)=====";