#include <WiFi.h>
#include <WebServer.h>
#include <HardwareSerial.h>
#include <ESP32Time.h>
#include <time.h>

// -------------------------------- ( Wifi config ) --------------------------------
const char* ssid     = "x";
const char* password = "x";

// ------------------------------- ( Server config ) -------------------------------
WebServer server(80);

// ------------------------------- ( Time config ) ---------------------------------
ESP32Time rtc;

// Denmark / Central Europe timezone with daylight saving
const char* ntpServer = "pool.ntp.org";
const char* timezone = "CET-1CEST,M3.5.0,M10.5.0/3";

// Values
float value1 = 0.0;
float value2 = 0.0;
int value3 = 0;

// UART config
HardwareSerial mySerial(1); // UART1

// -------------------------------- ( HTML code ) ----------------------------------
bool triggerFeed = false;
String getHTML() {
  String currentTime = rtc.getTime("%H:%M:%S");

  String html = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <meta http-equiv="refresh" content="2"/>
  <title>ESP32-C3 Dashboard</title>

  <style>
    body {
      margin: 0;
      font-family: Arial, sans-serif;
      background: #0f1115;
      color: #e6e6e6;
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
    }

    .card {
      background: #1a1d24;
      padding: 25px;
      border-radius: 12px;
      width: 320px;
      box-shadow: 0 0 20px rgba(0,0,0,0.5);
      text-align: center;
    }

    h1 {
      font-size: 20px;
      margin-bottom: 20px;
      color: #ffffff;
    }

    .value {
      margin: 15px 0;
      font-size: 18px;
    }

    .time {
      font-size: 22px;
      font-weight: bold;
      color: #f1c40f;
      margin-bottom: 20px;
    }

    .highlight {
      font-size: 26px;
      font-weight: bold;
      margin: 10px 0;
      color: #4da3ff;
    }

    button {
      width: 100%;
      padding: 12px;
      margin: 5px 0;
      font-size: 18px;
      border: none;
      border-radius: 8px;
      cursor: pointer;
      transition: 0.2s;
    }

    .inc {
      background: #2ecc71;
      color: white;
    }

    .inc:hover {
      background: #27ae60;
    }

    .dec {
      background: #e74c3c;
      color: white;
    }

    .dec:hover {
      background: #c0392b;
    }

    .feed-btn {
      background: #f39c12;
      color: white;
      margin-top: 20px;
      font-weight: bold;
    }
    .feed-btn:hover {
      background: #e67e22;
    }
  </style>
  </head>

  <body>
    <div class="card">
      <h1>ESP32-C3 Dashboard</h1>

      <div class="time">Time: )rawliteral" + currentTime + R"rawliteral(</div>

      <div class="value">Water left: )rawliteral" + String(value1, 1) + R"rawliteral(</div>
      <div class="value">Food left: )rawliteral" + String(value2) + R"rawliteral(</div>

      <form action="/inc">
        <button class="inc">+100</button>
      </form>

      <div class="highlight">)rawliteral" + String(value3) + R"rawliteral(</div>
      <div>Daily food amount</div>

      <form action="/dec">
        <button class="dec">-100</button>
      </form>

      <form action="/feed" method="POST">
        <button class="feed-btn">FEED NOW</button>
      </form>

      <form action="/inc"><button class="inc">+100</button></form>
      <div class="highlight">)rawliteral" + String(value3) + R"rawliteral(</div>
      <form action="/dec"><button class="dec">-100</button></form>
    </div>
  </body>
  </html>
  )rawliteral";

  return html;
}

// ------------------------- ( handlers for html buttons ) -------------------------
void handleRoot() {
  server.send(200, "text/html", getHTML());
}

void handleFeed() {
  triggerFeed = true; // Set the flag
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleInc() {
  value3 += 100;
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleDec() {
  value3 -= 100;
  server.sendHeader("Location", "/");
  server.send(303);
}

// ----------------------------- ( sync time function ) -----------------------------
void syncTime() {
  configTzTime(timezone, ntpServer);

  struct tm timeinfo;

  Serial.print("Syncing time");

  while (!getLocalTime(&timeinfo)) {
    delay(1000);
    Serial.print(".");
  }

  rtc.setTimeStruct(timeinfo);

  Serial.println();
  Serial.print("Time synced: ");
  Serial.println(rtc.getTime("%H:%M:%S"));
}

// ----------------------------------- ( setup ) -----------------------------------
void setup() {
  Serial.begin(115200);

  // baud, config, RX, TX
  mySerial.begin(9600, SERIAL_8N1, 20, 21);
  Serial.println("UART sender ready");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Sync internet time
  syncTime();

  // Routes
  server.on("/", handleRoot);
  server.on("/inc", handleInc);
  server.on("/dec", handleDec);
  server.on("/feed", HTTP_POST, handleFeed);

  server.begin();
  Serial.println("HTTP server started");
}

// ------------------------------------ ( loop ) -----------------------------------
void loop() {
  server.handleClient();

  // 1. Send commands TO the ESP8266
  String currentTime = rtc.getTime("%H:%M:%S");
  mySerial.print("TIME:");
  mySerial.print(currentTime);
  mySerial.print(",SETFOOD:"); // Renamed for clarity
  mySerial.println(value3);

  if (triggerFeed) {
    mySerial.print(",FEED:1");
    triggerFeed = false; // Reset flag after sending
  } else {
    mySerial.print(",FEED:0");
  }
  mySerial.println(); // End line

  // 2. Read sensor data FROM the ESP8266
  if (mySerial.available()) {
    String incoming = mySerial.readStringUntil('\n');
    
    // Simple parsing: expecting "W:45.5,F:80"
    int wIndex = incoming.indexOf("W:");
    int fIndex = incoming.indexOf(",F:");
    
    if (wIndex != -1 && fIndex != -1) {
      value1 = incoming.substring(wIndex + 2, fIndex).toFloat(); // Water
      value2 = incoming.substring(fIndex + 3).toFloat();          // Food
    }
  }

  delay(1000); // Sync once per second
}
