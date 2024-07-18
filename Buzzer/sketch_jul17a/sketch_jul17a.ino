#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "UPB-Guest";
const char* password = NULL;
const int buzzerPin = 14; // GPIO pin connected to the buzzer I/O pin

WebServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(buzzerPin, OUTPUT); // Set the buzzer pin as an output

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("WiFi connected, IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/alert", handleAlert);
  server.begin();
}

void loop() {
  server.handleClient();
}

void handleAlert() {
  Serial.println("Face detected! Activating buzzer...");
  playCorrectSound();
  server.send(200, "text/plain", "Buzzer activated");
}

void playCorrectSound() {
  tone(buzzerPin, 1000); // Start tone at 1000 Hz
  delay(1000); // Wait for 1 second
  noTone(buzzerPin); // Stop the tone
}
