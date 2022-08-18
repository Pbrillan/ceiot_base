#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "DHTesp.h" 

#include "config.h"
  
DHTesp dht;

float humidity = 0.0;
float temperature = 0.0;
bool Registrado = 0 ;
String DEVICE_ID ;


void setup() {
  delay(10000);

  Serial.begin(115200);
  delay(10000);
  Serial.println("ESP8266 DHT HTTP POST");
  
  dht.setup(ONE_WIRE_GPIO, DHTesp::DHT11);
  delay(dht.getMinimumSamplingPeriod());

  Serial.println(dht.getStatusString());
  humidity    = dht.getHumidity();
  temperature = dht.getTemperature();
  Serial.print("Temperature : ");
  Serial.println(temperature);
  Serial.print("Humidity : ");
  Serial.println(humidity);


  Serial.println();
  Serial.println();
  Serial.println();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Waiting for " WIFI_SSID);
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // wait for WiFi connection
  if ((WiFi.status() == WL_CONNECTED)) {

  if ((Registrado== 0)) {
    Registrado = 1;
    DEVICE_ID = WiFi.macAddress();
    Serial.println(dht.getStatusString());
    Serial.print("Registrando Dispositivo como : ");
    Serial.print(DEVICE_ID);
    Serial.print(" \n");
    WiFiClient client;
    HTTPClient http;
    Serial.print("[HTTP] begin...\n");

    http.begin(client,"http://" API_IP_PORT "/device");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    Serial.print("[HTTP] POST...\n");
    // start connection and send HTTP header and body
    String post("id=");
    post += DEVICE_ID;
    post += "&n=";
    post += "ESP8266";
    post += "&k=";
    post += "12";
    int httpCode = http.POST(post);



    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        const String& payload = http.getString();
        Serial.println("received payload:\n<<");
        Serial.println(payload);
        Serial.println(">>");
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s when connecting to %s\n", http.errorToString(httpCode).c_str(), API_IP_PORT);
    }

    http.end();
  }




  Serial.println(dht.getStatusString());
  humidity    = dht.getHumidity();
  temperature = dht.getTemperature();
  Serial.print("Temperature : ");
  Serial.println(temperature);
  Serial.print("Humidity : ");
  Serial.println(humidity);


 


    WiFiClient client;
    HTTPClient http;




    Serial.print("[HTTP] begin...\n");
    // configure traged server and url
    http.begin(client, "http://" API_IP_PORT "/measurement");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    Serial.print("[HTTP] POST...\n");
    // start connection and send HTTP header and body
    String post("id=");
    post += DEVICE_ID;
    post += "&t=";
    post += temperature;
    post += "&h=";
    post += humidity;
    int httpCode = http.POST(post);

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        const String& payload = http.getString();
        Serial.println("received payload:\n<<");
        Serial.println(payload);
        Serial.println(">>");
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s when connecting to %s\n", http.errorToString(httpCode).c_str(), API_IP_PORT);
    }

    http.end();
  }

  delay(10000);
}
