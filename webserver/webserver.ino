#include <WiFi.h>
#include <WebServer.h>
#include <HardwareSerial.h>

// -------------------------------- ( Wifi config ) --------------------------------
const char* ssid     = "AGNET";
const char* password = "11111111";

// ------------------------------- ( Server config ) -------------------------------
WebServer server(80);

// Values
float value1 = 0.0;
float value2 = 0.0;
int value3 = 0;

// HTML code
String getHTML() {
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

  </style>
  </head>

  <body>
    <div class="card">
      <h1>ESP32-C3 Dashboard</h1>

      <div class="value">Water left: )rawliteral" + String(value1) + R"rawliteral(</div>
      <div class="value">Food left: )rawliteral" + String(value2) + R"rawliteral(</div>

      <form action="/inc">
        <button class="inc">+100</button>
      </form>

      <div class="highlight">)rawliteral" + String(value3) + R"rawliteral(</div>
      <div>Daily food amount</div>

      <form action="/dec">
        <button class="dec">-100</button>
      </form>
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
// UART config 
HardwareSerial mySerial(1); // UART1

void setup() {
  Serial.begin(115200);

  // baud, config, RX, TX
  mySerial.begin(9600, SERIAL_8N1, 20, 21);
  Serial.println("UART sender ready");

  WiFi.begin(ssid, password);
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Routes
  server.on("/", handleRoot);
  server.on("/inc", handleInc);
  server.on("/dec", handleDec);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  mySerial.println(value3);   // send as text with newline
  Serial.println("Sent: " + String(value3));

  // Update example values
  value1 += 0.1;
  value2 = analogRead(2); // adjust pin if needed

  delay(100);
}