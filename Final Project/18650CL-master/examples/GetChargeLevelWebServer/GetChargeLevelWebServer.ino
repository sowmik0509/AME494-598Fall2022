#include <Pangodream_18650_CL.h>
#include <WiFi.h>
#include <HTTPClient.h>

//#define ADC_PIN 34
//#define CONV_FACTOR 1.7
//#define READS 20

Pangodream_18650_CL BL;
/**
 * If you need to change default values you can use it as
 * Pangodream_18650_CL BL(ADC_PIN, CONV_FACTOR, READS);
 */

const char* ssid = "MaRTinyAP";
const char* password = "MaRTiny123";

//Your Domain name with URL path or IP address with path
const char* serverName = "http://192.168.0.101:8080/sendData";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 1000;

String response;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");

}

void loop() {

   if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){

      float pinValue = analogRead(34);
      float avgPinValue = BL.pinRead();
      float volts = BL.getBatteryVolts();
      float chargeLevel = BL.getBatteryChargeLevel();
      
      Serial.print("Value from pin: ");
      Serial.println(pinValue);
      Serial.print("Average value from pin: ");
      Serial.println(avgPinValue);
      Serial.print("Volts: ");
      Serial.println(volts);
      Serial.print("Charge level: ");
      Serial.println(chargeLevel);
      Serial.println("");
      delay(1000);

      String url = String(serverName) + "?pv=" + pinValue + "&apv=" + avgPinValue + "&v=" + volts + "&cl=" + chargeLevel; 
      Serial.println(url);       
      response = httpGETRequest(url.c_str());
      Serial.println(response);

    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

String httpGETRequest(const char* serverName) {
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}
