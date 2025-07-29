#include "esp_camera.h"
#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>

// Wi-Fi credentials
const char* ssid = "realme 5i";
const char* password = "15082003";

// Pin Definitions
#define BUTTON_PIN 13
#define FLASH_LED_PIN 4  // Flash LED pin

WebServer server(80);
camera_fb_t* lastCapturedPhoto = NULL;

// AI Thinker pin configuration
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// Capture a photo and store in RAM
void capturePhoto() {
  if (lastCapturedPhoto) {
    esp_camera_fb_return(lastCapturedPhoto);  // Free memory
    lastCapturedPhoto = NULL;
  }

  digitalWrite(FLASH_LED_PIN, HIGH); // Turn ON flash
  delay(150);                        // Let flash stabilize

  lastCapturedPhoto = esp_camera_fb_get();

  digitalWrite(FLASH_LED_PIN, LOW);  // Turn OFF flash

  if (!lastCapturedPhoto) {
    Serial.println("Camera capture failed");
  } else {
    Serial.println("Photo captured!");
  }
}

// Notify Python server on PC
void notifyPythonServer() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("http://192.168.43.121:5000/trigger");
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST("{\"message\": \"photo captured\"}");

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Server response: " + response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi not connected");
  }
}

// Serve UI and photo
void startCameraServer() {
  server.on("/", HTTP_GET, []() {
    String html = R"rawliteral(
      <html><head><title>ESP32-CAM</title></head><body>
      <h2>ESP32-CAM Button Trigger</h2>
      <p>Press the hardware button to capture a photo.</p>
      <p><a href="/photo"><button>📷 View Last Photo</button></a></p>
      </body></html>
    )rawliteral";
    server.send(200, "text/html", html);
  });

  server.on("/photo", HTTP_GET, []() {
    if (lastCapturedPhoto) {
      server.sendHeader("Content-Type", "image/jpeg");
      server.sendHeader("Content-Disposition", "inline; filename=capture.jpg");
      server.send_P(200, "image/jpeg", (const char*)lastCapturedPhoto->buf, lastCapturedPhoto->len);
    } else {
      server.send(200, "text/plain", "No photo captured yet.");
    }
  });

  server.begin();
  Serial.println("HTTP server started");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(BUTTON_PIN, INPUT_PULLUP); // Button with pull-up
  pinMode(FLASH_LED_PIN, OUTPUT);    // Flash pin
  digitalWrite(FLASH_LED_PIN, LOW);  // Initially OFF

  // Camera configuration
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer   = LEDC_TIMER_0;
  config.pin_d0       = Y2_GPIO_NUM;
  config.pin_d1       = Y3_GPIO_NUM;
  config.pin_d2       = Y4_GPIO_NUM;
  config.pin_d3       = Y5_GPIO_NUM;
  config.pin_d4       = Y6_GPIO_NUM;
  config.pin_d5       = Y7_GPIO_NUM;
  config.pin_d6       = Y8_GPIO_NUM;
  config.pin_d7       = Y9_GPIO_NUM;
  config.pin_xclk     = XCLK_GPIO_NUM;
  config.pin_pclk     = PCLK_GPIO_NUM;
  config.pin_vsync    = VSYNC_GPIO_NUM;
  config.pin_href     = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn     = PWDN_GPIO_NUM;
  config.pin_reset    = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size   = FRAMESIZE_VGA;   // Higher resolution than VGA
  config.jpeg_quality = 9;               // Better image quality (lower is better)
  config.fb_count     = 1;

  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Camera init failed");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());

  startCameraServer();
}

void loop() {
  server.handleClient();

  static bool lastButtonState = HIGH;
  bool currentState = digitalRead(BUTTON_PIN);

  if (lastButtonState == HIGH && currentState == LOW) {
    Serial.println("Button pressed!");
    capturePhoto();           // Take the photo with flash
    notifyPythonServer();     // Notify PC
    delay(500);               // Debounce
  }

  lastButtonState = currentState;
}

