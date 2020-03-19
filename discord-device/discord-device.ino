// 3.3V => button =>  D8

#include <ESP8266HTTPClient.h>  // To send HTTP requests.

// Third-party libraries.
#include <WiFiManager.h>  // To manage network configuration and connection.

// Network configuration.
const char* WIFI_AP_SSID = "DiscordButton";       // The name of the wireless network to create if cannot connect using the previously saved credentials.
const char* WIFI_AP_PASSWORD = "DiscordButton!";  // The password required to connect to the wireless network used to configure the network parameters.

WiFiManager wifiManager;
int lastButtonState = LOW;

void setup() {
  initSerial();
  initLed();
  initButton();

  Serial.println(F("Setup DONE."));
}

void loop() {
  int buttonState = digitalRead(D8);
  
  if(buttonState != lastButtonState) {
    if(buttonState == HIGH) {
      onButtonPressed();
    } 
    else {
      onButtonReleased();
    }

    lastButtonState = buttonState;
  }
  
  delay(300);
}


void initSerial() {
  Serial.begin(115200);
  Serial.println();
  Serial.println(F("Initializing serial connection DONE."));

  initNetwork();
}

void initLed() {
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);  // Turn off
}

void initButton() {
  pinMode(D8, INPUT);
}

void initNetwork() {
  Serial.printf("Initializing connection to the network with MAC address %s using WiFiManager (SSID: %s)...\n", WiFi.macAddress().c_str(), WIFI_AP_SSID);
  wifiManager.autoConnect(WIFI_AP_SSID, WIFI_AP_PASSWORD);
  Serial.printf("WiFi connected. IP address: %s, MAC address: %s\n", WiFi.localIP().toString().c_str(), WiFi.macAddress().c_str());
}

void onButtonPressed() {
  Serial.println(F("Pressed"));
  digitalWrite(BUILTIN_LED, LOW);
  sendRequest("http://192.168.0.107:20314/discord/press");
}

void onButtonReleased() {
  Serial.println(F("Released"));
  digitalWrite(BUILTIN_LED, HIGH);
  sendRequest("http://192.168.0.107:20314/discord/release");
}

void sendRequest(String url) {
  Serial.println("Sending POST request to " + url);

  HTTPClient http;
  http.begin(url);
  http.addHeader("X-API-Key", "alma");  

  String requestBody = "";

  int statusCode = http.POST(requestBody);
  Serial.printf("Received HTTP status code: %d\r\n", statusCode);

  if (statusCode != HTTP_CODE_OK) {
    String responseBody = http.getString();
    Serial.println("Received HTTP response body: " + responseBody);
  }

  http.end();  
}
