#include <main.h>

String generateHTML() {
    String color1 = pump1Status ? "green" : "red";
    String label1 = pump1Status ? "Bật" : "Tắt";
  
    String color2 = pump2Status ? "green" : "red";
    String label2 = pump2Status ? "Bật" : "Tắt";
  
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

        <div>
          <a href="/lichtrinh"><button>Quản lý lịch trình</button></a>
        </div>

        <h2>Cài đặt ngưỡng cảm biến & chế độ tưới tự động</h2>

        <form id="thresholdForm">
          <h3>Cảm biến 1 (Bơm 1)</h3>
          <label>Ngưỡng dưới (%):</label>
          <input type="number" id="sensor1Lower" min="0" max="100" step="0.01"><br>
          <label>Ngưỡng trên (%):</label>
          <input type="number" id="sensor1Upper" min="0" max="100" step="0.01"><br>

          <h3>Cảm biến 2 (Bơm 2)</h3>
          <label>Ngưỡng dưới (%):</label>
          <input type="number" id="sensor2Lower" min="0" max="100" step="0.01"><br>
          <label>Ngưỡng trên (%):</label>
          <input type="number" id="sensor2Upper" min="0" max="100" step="0.01"><br>

          <h3>Chế độ tưới tự động</h3>
          <label>
            <input type="checkbox" id="automaticWatering"> Bật tưới tự động
          </label><br><br>

          <button type="button" onclick="saveThresholds()">Lưu cài đặt</button>
        </form>

        <div id="saveStatus"></div>

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

        <div class="ap-config">
          <h2>Cài đặt điểm truy cập WiFi (Access Point)</h2>
          <p>Dùng để truy cập cài đặt của mạch khi mất kết nối WiFi</p>
          <p>Tên Access Point hiện tại: <span id="currentSSID">)rawliteral" + String(AP_SSID) + R"rawliteral(</span></p>
          <p>Mật khẩu Access Point hiện tại: <span id="currentPass">)rawliteral" + String(AP_PASS) + R"rawliteral(</span></p>
          <div id="wifiNetworks">
            <input type="text" id="AP_SSID" placeholder="Tên Access Point">
            <input type="password" id="AP_PASS" placeholder="Mật khẩu Access Point">
            <button onclick="saveAPConfig()">Lưu và khởi động lại</button>
          </div>
        </div>

        <button onclick="resetData()">Xoá toàn bộ dữ liệu (thông tin WiFI, AP, lịch trình,...)</button>
  
        <script>
          function toggleGPIO(pin) {
            fetch('/toggle?pin=' + pin)
              .then(response => response.json())
              .then(data => {
                // Update button 1
                const btn1 = document.getElementById('btn1');
                btn1.style.backgroundColor = data.pump1Status ? "green" : "red";
                btn1.textContent = "Bơm 1 - " + (data.pump1Status ? "Bật" : "Tắt");
                
                // Update button 2
                const btn2 = document.getElementById('btn2');
                btn2.style.backgroundColor = data.pump2Status ? "green" : "red";
                btn2.textContent = "Bơm 2 - " + (data.pump2Status ? "Bật" : "Tắt");
                
                })
                .catch(error => {
                  console.error('Error:', error);
                  alert("Error toggling GPIO");
              });
          }
          
          // Auto-refresh sensor data every 2 seconds
          function refreshStatusData() {
            fetch('/status')
              .then(response => response.json())
              .then(data => {
                document.getElementById('temperature').textContent = data.temperature;
                document.getElementById('humidity1').textContent = data.humidity.sensor1;
                document.getElementById('humidity2').textContent = data.humidity.sensor2;

                // Update button 1
                const btn1 = document.getElementById('btn1');
                btn1.style.backgroundColor = data.pump1Status ? "green" : "red";
                btn1.textContent = "Bơm 1 - " + (data.pump1Status ? "Bật" : "Tắt");
                
                // Update button 2
                const btn2 = document.getElementById('btn2');
                btn2.style.backgroundColor = data.pump2Status ? "green" : "red";
                btn2.textContent = "Bơm 2 - " + (data.pump2Status ? "Bật" : "Tắt");
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

          function resetData() {
            fetch('/resetData')
            .then(response => response.text())
            .then(data => {
              alert(data);
              setTimeout(() => { window.location.reload(); }, 2000);
            });
          }

          function saveAPConfig() {
            const ssid = document.getElementById('AP_SSID').value;
            const password = document.getElementById('AP_PASS').value;
            
            fetch('/saveAPConfig', {
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
          refreshStatusData();
          setInterval(refreshStatusData, 2000);

          function saveThresholds() {
            const data = {
              sensor1Lower: parseFloat(document.getElementById("sensor1Lower").value),
              sensor1Upper: parseFloat(document.getElementById("sensor1Upper").value),
              sensor2Lower: parseFloat(document.getElementById("sensor2Lower").value),
              sensor2Upper: parseFloat(document.getElementById("sensor2Upper").value),
              automaticWatering: document.getElementById("automaticWatering").checked
            };

            fetch("/saveThresholds", {
              method: "POST",
              headers: {
                "Content-Type": "application/json"
              },
              body: JSON.stringify(data)
            })
            .then(response => {
              if (response.ok) {
                document.getElementById("saveStatus").innerText = "Đã lưu thành công!";
              } else {
                document.getElementById("saveStatus").innerText = "Lỗi khi lưu!";
              }
            })
            .catch(error => {
              console.error("Error saving thresholds:", error);
              document.getElementById("saveStatus").innerText = "Lỗi kết nối!";
            });
          }

          window.onload = function() {
            fetch("/getThresholds")
              .then(response => response.json())
              .then(data => {
                document.getElementById("sensor1Lower").value = data.sensor1Lower;
                document.getElementById("sensor1Upper").value = data.sensor1Upper;
                document.getElementById("sensor2Lower").value = data.sensor2Lower;
                document.getElementById("sensor2Upper").value = data.sensor2Upper;
                document.getElementById("automaticWatering").checked = data.automaticWatering;
              })
              .catch(error => {
                console.error("Error loading thresholds:", error);
              });
          }
        </script>
      </body>
      </html>
    )rawliteral";
  
    return html;
}