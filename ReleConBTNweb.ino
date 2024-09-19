#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "IoTB";
const char* password = "inventaronelVAR";

WebServer server(80);
const int pin13 = 13;

String htmlPage = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>Control del Pin 13</title>
</head>
<body>
  <h1>Control del Pin 13</h1>
  <button onclick="togglePin()">Alternar Pin 13</button>
  <script>
    function togglePin() {
      fetch('/toggle').then(response => response.text()).then(status => {
        console.log(status);
      });
    }
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleToggle() {
  digitalWrite(pin13, !digitalRead(pin13));
  server.send(200, "text/plain", digitalRead(pin13) ? "ENCENDIDO" : "APAGADO");
}

void setup() {
  Serial.begin(115200);
  pinMode(pin13, OUTPUT);
  digitalWrite(pin13, LOW);

  WiFi.begin(ssid, password);
  Serial.println("Conectando al Wi-Fi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConectado al Wi-Fi");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);
  server.begin();
}

void loop() {
  server.handleClient();
}