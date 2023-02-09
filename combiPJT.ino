#include <DHT.h>  // Including library for dht

#include <ESP8266WiFi.h>
String apiKey1 = "L796O3A1M0WEGEYI";     //  Enter your Write API key from ThingSpeak
//String apiKey2 = "1AYIFNP3C1XVY8YL";     //  Enter your Write API key from ThingSpeak

const char *ssid =  "POCO";     // replace with your wifi ssid and wpa2 key
const char *pass =  "23456789";
const char* server = "api.thingspeak.com";

#define DHTPIN D4         //pin where the dht11 is connec
DHT dht(DHTPIN, DHT11);

WiFiClient client;
void setup()
{
       Serial.begin(9600);
       delay(10);
       dht.begin();
 
       Serial.println("Connecting to ");
       Serial.println(ssid);
 
 
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
 
}
void loop()
{
  float g = analogRead(A0);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)||isnan(g))
  {
    Serial.println("Failed to read from sensor!");
    return;
  }
  if(client.connect(server,80))
  {
    String postStr=apiKey1;
    //String postStr2 = apiKey2;
    postStr +="&field1=";
    postStr += String(t);
    postStr +="&field2=";
    postStr += String(h);
    postStr += "\r\n\r\n";
    postStr += "&field3=";
    postStr += String(g/1023*100);
    postStr += "r\n";
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
   // client.print("X-THINGSPEAKAPIKEY: " + apiKey2 + "\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiKey1+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    //client.print(postStr2.length());
    client.print("\n\n");
    client.print(postStr);
 
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" degrees Celcius, Humidity: ");
    Serial.print(h);
    Serial.println("%. Send to Thingspeak.");

    client.print("\n\n");
    //client.print(postStr);
    Serial.print("Gas Level: ");
    Serial.println(g/1023*100);
    Serial.println("Data Send to Thingspeak");
  }
delay(500);
client.stop();
Serial.println("Waiting...");
 
// thingspeak needs minimum 15 sec delay between updates.
delay(1500);
  
}
