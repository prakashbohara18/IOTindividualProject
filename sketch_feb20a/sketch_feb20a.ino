const int  aout = A0;
const int dout = 15;
float sensorvalue; //variable to store sensor value
float limit;
int red = 12;
int green = 13;

#include <ESP8266WiFi.h>
#define WIFI_SSID "merowifi"
#define WIFI_PASS "password"

#define HOST_IFTTT "maker.ifttt.com"
#define EVENT_NAME "gas_detected"
#define IFTTT_KEY "noHdYHnkkRvKKC1nDEZ5NV8fXaC4JrVUbf0_RCxzoRL"

const int buzzer = 2;

void wifiSetup() {

  // Set WIFI module to STA mode
  WiFi.mode(WIFI_STA);

  // Connect
  Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // Wait
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  // Connected!
  Serial.printf("Connected to Wi-Fi %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());

}

void sendNotification() {
  WiFiClient wifiClient;
  const int httpPort = 80;
  if (!wifiClient.connect(HOST_IFTTT, httpPort)) {
    Serial.println("Connection failed");
  }
  else {
    Serial.println("Sending Notification");
    String url = "GET /trigger/gas_detected/with/key/";
    url += IFTTT_KEY;
    url += " HTTP/1.1\r\nHost: ";
    url += HOST_IFTTT;
    url += "\r\nConnection: close\r\n\r\n";
    wifiClient.print(url);
  }
  delay(60000);
}

void setup() {
  Serial.begin(115200);
  pinMode(aout, INPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(dout, OUTPUT);
  pinMode(buzzer, OUTPUT);

  wifiSetup();

}

void loop() {
  sensorvalue = analogRead(aout);
  limit = digitalRead(dout);
  Serial.println(sensorvalue);//
  //  Serial.println(limit);
  delay(100);
  if (sensorvalue > 200) {
    tone(buzzer, 1000); // Send 1KHz sound signal...
    digitalWrite(red, HIGH);
    Serial.println(sensorvalue);
    digitalWrite(green, LOW);
    delay(1000);
    sendNotification();
  }
  else {
    noTone(buzzer);     // Stop sound...
    digitalWrite(red, LOW);
    digitalWrite(green, HIGH);
    delay(1000);
  }
}
