const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
  <meta charset='UTF-8'>
  <meta name='viewport' content='width=device-width,initial-scale=1.0'>
  <title>ESP32 WiFi Manager</title>
  <style>
    * {
      margin: 0;
      padding: 0;
      box-sizing: border-box;
    }
    body {
      font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, sans-serif;
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      min-height: 100vh;
      padding: 20px;
      display: flex;
      align-items: center;
      justify-content: center;
    }
    .container {
      background: white;
      border-radius: 16px;
      box-shadow: 0 20px 60px rgba(0,0,0,0.3);
      max-width: 500px;
      width: 100%;
      padding: 32px;
    }
    .header {
      text-align: center;
      margin-bottom: 32px;
    }
    .header h1 {
      color: #333;
      font-size: 28px;
      margin-bottom: 8px;
    }
    .header p {
      color: #666;
      font-size: 14px;
    }
    .status {
      background: #f8f9fa;
      border-radius: 8px;
      padding: 16px;
      margin-bottom: 24px;
      border-left: 4px solid #667eea;
    }
    .status-title {
      font-weight: 600;
      color: #333;
      margin-bottom: 4px;
    }
    .status-value {
      color: #666;
      font-size: 14px;
    }
    .btn {
      background: #667eea;
      color: white;
      border: none;
      padding: 12px 24px;
      border-radius: 8px;
      font-size: 16px;
      font-weight: 600;
      cursor: pointer;
      width: 100%;
      margin-bottom: 16px;
      transition: all 0.3s;
    }
    .btn:hover {
      background: #5568d3;
      transform: translateY(-2px);
      box-shadow: 0 4px 12px rgba(102,126,234,0.4);
    }
    .btn:active {
      transform: translateY(0);
    }
    .btn:disabled {
      background: #ccc;
      cursor: not-allowed;
      transform: none;
    }
    .networks {
      margin-top: 24px;
    }
    .network-item {
      background: #f8f9fa;
      border: 2px solid #e9ecef;
      border-radius: 8px;
      padding: 16px;
      margin-bottom: 12px;
      cursor: pointer;
      transition: all 0.3s;
      display: flex;
      justify-content: space-between;
      align-items: center;
    }
    .network-item:hover {
      border-color: #667eea;
      background: #f0f2ff;
    }
    .network-item.selected {
      border-color: #667eea;
      background: #f0f2ff;
    }
    .network-name {
      font-weight: 600;
      color: #333;
    }
    .network-signal {
      color: #666;
      font-size: 14px;
    }
    .password-input {
      width: 100%;
      padding: 12px;
      border: 2px solid #e9ecef;
      border-radius: 8px;
      font-size: 16px;
      margin: 16px 0;
      transition: border 0.3s;
    }
    .password-input:focus {
      outline: none;
      border-color: #667eea;
    }
    .hidden {
      display: none;
    }
    .loading {
      text-align: center;
      color: #666;
      padding: 20px;
    }
    .spinner {
      border: 3px solid #f3f3f3;
      border-top: 3px solid #667eea;
      border-radius: 50%;
      width: 40px;
      height: 40px;
      animation: spin 1s linear infinite;
      margin: 0 auto 16px;
    }
    @keyframes spin {
      0% { transform: rotate(0deg); }
      100% { transform: rotate(360deg); }
    }
    .success {
      color: #28a745;
      font-weight: 600;
      text-align: center;
      margin-top: 16px;
    }
    .error {
      color: #dc3545;
      font-weight: 600;
      text-align: center;
      margin-top: 16px;
    }
  </style>
</head>
<body>
  <div class='container'>
    <div class='header'>
      <h1>ESP32 WiFi Manager</h1>
      <p>Configure your device connection</p>
    </div>

    <div class='status'>
      <div class='status-title'>Status</div>
      <div class='status-value' id='statusText'>Loading...</div>
    </div>

    <button class='btn' id='scanBtn' onclick='scanNetworks()'>Scan for Networks</button>

    <div id='networks' class='networks hidden'></div>

    <div id='passwordSection' class='hidden'>
      <input type='password' class='password-input' id='passwordInput' placeholder='Enter WiFi password'>
      <button class='btn' onclick='connectToNetwork()'>Connect</button>
    </div>

    <div id='message'></div>
  </div>

  <script>
    let selectedSSID = '';
    let selectedEncryption = 0;

    function updateStatus() {
      fetch('/status')
        .then(r => r.json())
        .then(data => {
          const statusText = document.getElementById('statusText');
          if (data.connected) {
            statusText.innerHTML = `Connected to <strong>${data.ssid}</strong><br>IP: ${data.ip}`;
          } else {
            statusText.textContent = 'Not connected to any network';
          }
        })
        .catch(() => {
          document.getElementById('statusText').textContent = 'Unable to get status';
        });
    }

    function scanNetworks() {
      const btn = document.getElementById('scanBtn');
      const networksDiv = document.getElementById('networks');
      const passwordSection = document.getElementById('passwordSection');
      const message = document.getElementById('message');

      btn.disabled = true;
      btn.textContent = 'Scanning...';
      networksDiv.innerHTML = '<div class="loading"><div class="spinner"></div><p>Scanning for networks...</p></div>';
      networksDiv.classList.remove('hidden');
      passwordSection.classList.add('hidden');
      message.innerHTML = '';

      fetch('/scan')
        .then(r => r.json())
        .then(data => {
          networksDiv.innerHTML = '';
          if (data.networks.length === 0) {
            networksDiv.innerHTML = '<p class="loading">No networks found</p>';
          } else {
            data.networks.forEach(network => {
              const div = document.createElement('div');
              div.className = 'network-item';
              div.innerHTML = `<span class='network-name'>${network.ssid}</span><span class='network-signal'>${network.rssi} dBm ${network.encryption > 0 ? '🔒' : ''}</span>`;
              div.onclick = () => selectNetwork(network.ssid, network.encryption);
              networksDiv.appendChild(div);
            });
          }
          btn.disabled = false;
          btn.textContent = 'Scan for Networks';
        })
        .catch(err => {
          networksDiv.innerHTML = '<p class="error">Scan failed. Please try again.</p>';
          btn.disabled = false;
          btn.textContent = 'Scan for Networks';
        });
    }

    function selectNetwork(ssid, encryption) {
      selectedSSID = ssid;
      selectedEncryption = encryption;

      document.querySelectorAll('.network-item').forEach(item => {
        item.classList.remove('selected');
      });
      event.target.closest('.network-item').classList.add('selected');

      const passwordSection = document.getElementById('passwordSection');
      if (encryption > 0) {
        passwordSection.classList.remove('hidden');
        document.getElementById('passwordInput').value = '';
        document.getElementById('passwordInput').focus();
      } else {
        passwordSection.classList.add('hidden');
        connectToNetwork();
      }
    }

    function connectToNetwork() {
      const password = selectedEncryption > 0 ? document.getElementById('passwordInput').value : '';
      const message = document.getElementById('message');

      if (selectedEncryption > 0 && !password) {
        message.innerHTML = '<p class="error">Please enter a password</p>';
        return;
      }

      message.innerHTML = '<div class="loading"><div class="spinner"></div><p>Connecting to ' + selectedSSID + '...</p></div>';

      const formData = new FormData();
      formData.append('ssid', selectedSSID);
      formData.append('password', password);

      fetch('/connect', {
        method: 'POST',
        body: formData
      })
        .then(r => r.json())
        .then(data => {
          if (data.success) {
            message.innerHTML = '<p class="success">Connected successfully!</p>';
            setTimeout(updateStatus, 2000);
          } else {
            message.innerHTML = '<p class="error">Connection failed: ' + data.message + '</p>';
          }
        })
        .catch(() => {
          message.innerHTML = '<p class="error">Connection request failed</p>';
        });
    }

    updateStatus();
    setInterval(updateStatus, 10000);
  </script>
</body>
</html>
)=====";
