#include "main.h"

void handleSchedulePage() {
    String html = R"rawliteral(
        <!DOCTYPE html>
        <html>
        <head>
          <meta charset="UTF-8">
          <title>Lịch Trình</title>
          <style>
            body { font-family: Arial; padding: 20px; }
            table, th, td { border: 1px solid #ccc; border-collapse: collapse; padding: 8px; }
            th { background: #eee; }
          </style>
          <script>
            window.onload = function () {
                document.getElementById('type').addEventListener('change', function () {
                document.getElementById('timedFields').style.display = this.value === 'timed' ? 'block' : 'none';
                document.getElementById('condFields').style.display = this.value === 'cond' ? 'block' : 'none';
                });

                // Set correct initial visibility on load
                const initType = document.getElementById("type").value;
                document.getElementById('timedFields').style.display = initType === 'timed' ? 'block' : 'none';
                document.getElementById('condFields').style.display = initType === 'cond' ? 'block' : 'none';
            }

            function refreshSchedules() {
                location.reload();
            }

            function submitForm() {
                const pump1 = document.getElementById("pump1").checked;
                const pump2 = document.getElementById("pump2").checked;

                let pump = 0;
                if (pump1 && pump2) pump = 3;
                else if (pump1) pump = 1;
                else if (pump2) pump = 2;
                else {
                alert("Chọn ít nhất một bơm");
                return;
                }

                const data = {
                type: document.getElementById("type").value,
                hour: document.getElementById("hour").value,
                minute: document.getElementById("minute").value,
                desc: document.getElementById("desc").value,
                pump: pump,
                enabled: document.getElementById("enabled").checked ? "on" : ""
                };

                if (data.type === "timed") {
                data.duration = document.getElementById("duration").value;
                } else {
                data.s1 = document.getElementById("s1").value;
                data.s2 = document.getElementById("s2").value;
                data.both = document.getElementById("both").checked ? "on" : "";
                }

                const formBody = new URLSearchParams(data).toString();

                fetch("/addSchedule", {
                method: "POST",
                headers: { "Content-Type": "application/x-www-form-urlencoded" },
                body: formBody
                }).then(res => location.reload());
            }
            </script>
        </head>
        <body>
          <h2>Lịch Trình</h2>
          <a href="/"><button>Quay lại trang chủ</button></a>
          <button onclick="refreshSchedules()">Làm mới</button>
          <table>
            <tr>
                <th>#</th>
                <th>Tên</th>
                <th>Loại</th>
                <th>Thời gian bắt đầu</th>
                <th>Trạng thái</th>
                <th>Thao tác</th>
            </tr>
      )rawliteral";
    
      for (size_t i = 0; i < schedules.size(); ++i) {
        const auto& s = schedules[i];
        html += "<tr><td>" + String(i) + "</td>";
        html += "<td>" + s.description + "</td>";
        html += "<td>" + String(s.type == ScheduleType::TIMED ? "Hẹn giờ" : "Điều kiện") + "</td>";
        html += "<td>" + String(s.hour) + ":" + String(s.minute) + "</td>";
        html += "<td>" + String(s.enabled ? "Bật" : "Tắt") + "</td>";
        html += "<td><a href='/deleteSchedule?id=" + String(i) + "'>Xoá</a></td></tr>";
      }
    
      html += R"rawliteral(
          </table>
          <h3>Thêm Lịch Trình</h3>
          <form id="addScheduleForm" onsubmit="submitForm(); return false;">
            <label>Loại:</label>
            <select id="type">
                <option value="timed">Hẹn giờ</option>
                <option value="cond">Theo cảm biến</option>
            </select><br>

            <label>Giờ:</label>
            <input type="number" id="hour" min="0" max="23" required><br>
            <label>Phút:</label>
            <input type="number" id="minute" min="0" max="59" required><br>
            <label>Mô tả:</label>
            <input type="text" id="desc"><br>

            <label>Chọn bơm:</label><br>
            <input type="checkbox" id="pump1"> Bơm 1
            <input type="checkbox" id="pump2"> Bơm 2<br>

            <div id="timedFields">
                <label>Thời lượng (phút):</label>
                <input type="number" id="duration"><br>
            </div>

            <div id="condFields" style="display:none;">
                <label>Ngưỡng cảm biến 1:</label>
                <input type="number" id="s1" step="0.01"><br>
                <label>Ngưỡng cảm biến 2:</label>
                <input type="number" id="s2" step="0.01"><br>
                <label><input type="checkbox" id="both"> Cần cả 2 cảm biến</label><br>
            </div>

            <label><input type="checkbox" id="enabled" checked> Bật</label><br>
            <button type="submit">Thêm lịch</button>
            </form>
        </body></html>
      )rawliteral";
    
      server.send(200, "text/html", html);
}

void handleAddSchedule() {
    String type = server.arg("type");
    uint8_t hour = server.arg("hour").toInt();
    uint8_t minute = server.arg("minute").toInt();
    String desc = server.arg("desc");
    uint8_t pump = server.arg("pump").toInt();
    bool enabled = server.hasArg("enabled");
  
    if (type == "timed") {
      unsigned long duration = server.arg("duration").toInt();
      bool success = addTimedSchedule(
        pump,
        hour,
        minute,
        pumpControl,  // replace this with your actual function
        desc,
        duration,
        enabled
      );
      if (!success) {
        server.send(500, "text/plain", "Failed to add timed schedule.");
        return;
      }
    } else if (type == "cond") {
      float s1 = server.arg("s1").toFloat();
      float s2 = server.arg("s2").toFloat();
      bool both = server.hasArg("both");
      bool success = addConditionalSchedule(
        pump,
        hour,
        minute,
        pumpControl,  // replace this with your actual function
        desc,
        s1,
        s2,
        both,
        enabled
      );
      if (!success) {
        server.send(500, "text/plain", "Failed to add conditional schedule.");
        return;
      }
    } else {
      server.send(400, "text/plain", "Invalid type.");
      return;
    }
  
    server.sendHeader("Location", "/lichtrinh");
    server.send(303);  // Redirect back to page
  }

void handleDeleteSchedule() {
    if (server.hasArg("id")) {
      int index = server.arg("id").toInt();
      if (index >= 0 && index < schedules.size()) {
        schedules.erase(schedules.begin() + index);
      }
    }
    server.sendHeader("Location", "/lichtrinh");
    server.send(303);
}