/*
  Blynk WiFi Air Temperature/Humidity Sensor

  Created: 26 February 2018
  Last edit: 2 April 2018
  Author: Dylan Sri-Jayantha

  This code was written for ESP8266 wired with AM1117 voltage
  regulator and DHT11 and DHT22 temperature sensor modules. Transmits
  temperature and humidity data over WiFi via Blynk.

*/

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <DHT_U.h>

int dht22Pin = 0;
DHT dht22(dht22Pin, DHT22);
int dht11Pin = 2;
DHT dht11(dht11Pin, DHT11);

// Blynk Auth token
char auth[] = "";

// WiFi credentials.
char ssid[] = "";
char pass[] = "";

BlynkTimer timer;
void timerEvent();

void setup()
{ 
  dht22.begin();
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(60000L, timerEvent);
}

void loop()
{
  Blynk.run();
  timer.run();
}

void timerEvent(){
  
  // Take reading on DHT22
  float tempf_22 = dht22.readTemperature(true);
  float hum_22 = dht22.readHumidity();
  float tempf_11 = dht11.readTemperature(true);
  float hum_11 = dht11.readHumidity();

  // Reconnect if disconnected from Blynk server
     if (!Blynk.connected()){
       Blynk.connect();
     }

  // Send readings to Blynk server
  Blynk.virtualWrite(V0, tempf_22);
  Blynk.virtualWrite(V1, hum_22);
  Blynk.virtualWrite(V2, tempf_11);
  Blynk.virtualWrite(V3, hum_11);
}


