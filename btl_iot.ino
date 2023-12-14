#include <WebServer.h>
#include <WebSocketsServer.h>
#include <WiFi.h>
#include <esp32cam.h>

// Thêm thư viện để đọc giá trị từ cảm biến HY-SRF05
#include <NewPing.h>

const char* WIFI_SSID = "301";
const char* WIFI_PASS = "12345678";

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

static auto hiRes = esp32cam::Resolution::find(800, 600);

// Khai báo chân kết nối của cảm biến HY-SRF05
#define TRIGGER_PIN 14
#define ECHO_PIN    12
#define MAX_DISTANCE 500 // Giả sử khoảng cách tối đa là 500cm

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // Khởi tạo đối tượng NewPing

void handleRoot()
{
  server.send(200, "text/html", "<h1>ESP32-CAM Web Server</h1>");
}

void serveJpg()
{
  auto frame = esp32cam::capture();
  if (frame == nullptr) {
    Serial.println("CAPTURE FAIL");
    server.send(503, "", "");
    return;
  }

  server.setContentLength(frame->size());
  server.send(200, "image/jpeg");
  WiFiClient client = server.client();
  frame->writeTo(client);
}

void handleJpgHi()
{
  if (!esp32cam::Camera.changeResolution(hiRes)) {
    Serial.println("SET-HI-RES FAIL");
  }
  serveJpg();
}

void handleStream()
{
  server.sendHeader("Content-Type", "multipart/x-mixed-replace; boundary=frame");
  while (server.client().connected()) {
    auto frame = esp32cam::capture();
    if (frame == nullptr) {
      Serial.println("CAPTURE FAIL");
      delay(1000);
      continue;
    }

    // Đọc giá trị khoảng cách từ cảm biến HY-SRF05
    int distance = sonar.ping_cm();

    if (distance < 5) {
      // Nếu khoảng cách nhỏ hơn 5cm, hú còi
      // (Chú ý: bạn cần kết nối còi và đối tượng hú còi vào đúng chân của ESP32)
      tone(16, 1000, 1000); // 16 là chân nối của còi, 1000 là tần số, 1000 là thời gian hú còi (ms)
    }

    WiFiClient client = server.client();
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: image/jpeg");
    client.println("Connection: close");
    client.println();

    frame->writeTo(client);
    delay(100);
  }
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length)
{
  // Handle WebSocket events here
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      Serial.printf("[%u] Connected from %s\n", num, webSocket.remoteIP(num).toString().c_str());
      break;
    case WStype_TEXT:
      // Handle text messages if needed
      break;
    case WStype_BIN:
      // Handle binary data (video frames) from WebSocket
      webSocket.sendBIN(num, payload, length);
      break;
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println();

  {
    using namespace esp32cam;
    Config cfg;
    cfg.setPins(pins::AiThinker);
    cfg.setResolution(hiRes);
    cfg.setBufferCount(2);
    cfg.setJpeg(80);

    bool ok = Camera.begin(cfg);
    Serial.println(ok ? "CAMERA OK" : "CAMERA FAIL");
  }

  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.print("http://");
  Serial.println(WiFi.localIP());
  Serial.println("  /cam-hi.jpg");
  Serial.println("  /stream");

  server.on("/", HTTP_GET, handleRoot);
  server.on("/cam-hi.jpg", HTTP_GET, handleJpgHi);
  server.on("/stream", HTTP_GET, handleStream);

  server.begin();

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop()
{
  server.handleClient();
  webSocket.loop();
}
