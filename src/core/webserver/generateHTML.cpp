#include <main.h>

String generateHTML() {
    String color1 = pin1Status ? "green" : "red";
    String label1 = pin1Status ? "Bật" : "Tắt";
  
    String color2 = pin2Status ? "green" : "red";
    String label2 = pin2Status ? "Bật" : "Tắt";
  
    String html = R"rawliteral(
      <!DOCTYPE html>
      <html>
      <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <title>Điều khiển và Giám sát</title>
        <style>
          body { text-align: center; font-family: sans-serif; margin-top: 50px; }
          .button {
            font-size: 24px;
            padding: 20px 40px;
            margin: 20px;
            color: white;
            border: none;
            border-radius: 10px;
            cursor: pointer;
          }
          .sensor-readings {
            margin: 30px auto;
            padding: 20px;
            border: 1px solid #ddd;
            border-radius: 10px;
            max-width: 500px;
            background-color: #f9f9f9;
          }
          .sensor-value {
            font-size: 24px;
            font-weight: bold;
            color: #2c3e50;
          }
        </style>
      </head>
      <body>
        <h1>Điều khiển</h1>
        
        <div class="sensor-readings">
          <h2>Thông số cảm biến</h2>
          <div id="sensorData">
            <p>Nhiệt độ: <span class="sensor-value" id="temperature">)rawliteral" + String("Đang tải...") + R"rawliteral(</span> °C</p>
            <p>Độ ẩm cảm biến 1: <span class="sensor-value" id="humidity1">)rawliteral" + "Đang tải..." + R"rawliteral(</span> %</p>
            <p>Độ ẩm cảm biến 2: <span class="sensor-value" id="humidity2">)rawliteral" + "Đang tải..." + R"rawliteral(</span> %</p>
          </div>
        </div>
        
        <button class="button" id="btn1" style="background-color: )rawliteral" + color1 + R"rawliteral(;") onclick="toggleGPIO(1)">Bơm 1 - )rawliteral" + label1 + R"rawliteral(</button>
        <button class="button" id="btn2" style="background-color: )rawliteral" + color2 + R"rawliteral(;") onclick="toggleGPIO(2)">Bơm 2 - )rawliteral" + label2 + R"rawliteral(</button>

        <div class="wifi-config">
          <h2>Cài đặt WiFi</h2>
          <p>Tên WiFi hiện tại: <span id="currentSSID">)rawliteral" + String(ssid) + R"rawliteral(</span></p>
          <p>Mật khẩu WiFi hiện tại: <span id="currentPass">)rawliteral" + String(pass) + R"rawliteral(</span></p>
          <div id="wifiNetworks">
            <input type="text" id="wifiSSID" placeholder="Tên WiFi">
            <input type="password" id="wifiPassword" placeholder="Mật khẩu WiFi">
            <button onclick="saveWiFiConfig()">Lưu và khởi động lại</button>
          </div>
        </div>
  
        <script>
          function toggleGPIO(pin) {
            fetch('/toggle?pin=' + pin)
              .then(response => response.json())
              .then(data => {
                // Update button 1
                const btn1 = document.getElementById('btn1');
                btn1.style.backgroundColor = data.pin1Status ? "green" : "red";
                btn1.textContent = "Bơm 1 - " + (data.pin1Status ? "Bật" : "Tắt");
                
                // Update button 2
                const btn2 = document.getElementById('btn2');
                btn2.style.backgroundColor = data.pin2Status ? "green" : "red";
                btn2.textContent = "Bơm 2 - " + (data.pin2Status ? "Bật" : "Tắt");
                
                })
                .catch(error => {
                  console.error('Error:', error);
                  alert("Error toggling GPIO");
              });
          }
          
          // Auto-refresh sensor data every 2 seconds
          function refreshSensorData() {
            fetch('/sensorData')
              .then(response => response.json())
              .then(data => {
                document.getElementById('temperature').textContent = data.temperature;
                document.getElementById('humidity1').textContent = data.humidity.sensor1;
                document.getElementById('humidity2').textContent = data.humidity.sensor2;
                // Update other sensor values similarly
              });
          }
          
          function saveWiFiConfig() {
            const ssid = document.getElementById('wifiSSID').value;
            const password = document.getElementById('wifiPassword').value;
            
            fetch('/saveWiFiConfig', {
              method: 'POST',
              headers: {
                'Content-Type': 'application/x-www-form-urlencoded',
              },
              body: 'ssid=' + encodeURIComponent(ssid) + '&password=' + encodeURIComponent(password)
            })
            .then(response => response.text())
            .then(data => {
              alert(data);
              setTimeout(() => { window.location.reload(); }, 2000);
            });
          }
          
          // Refresh immediately and then every 2 seconds
          refreshSensorData();
          setInterval(refreshSensorData, 2000);
        </script>
      </body>
      </html>
    )rawliteral";
  
    return html;
}