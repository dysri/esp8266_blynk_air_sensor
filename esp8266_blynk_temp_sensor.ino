/*
  Blynk WiFi Air Temperature/Humidity Sensor

  Created: 26 February 2018
  Last edit: 26 February 2018
  Author: Dylan Sri-Jayantha

  This code was written for ATmega328 wired with ESP8266 and DHT22 modules. Transmits
  temperature and humidity data over WiFi via

*/

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <DHT.h>
#include <DHT_U.h>
#include <avr/wdt.h>

#define aref_voltage 3.3

int dht22Pin = 2;
int tmp36Pin = 0;

// Time between transmissions
unsigned long idlePeriod = 300000;
unsigned long prevMillis = 0;

DHT dht22(dht22Pin, DHT22);

// Blynk Auth token
char auth[] = "5e92d6b9165148f1a8851542ae6e00b4";

// WiFi credentials.
char ssid[] = "Avis-and-Sri";
char pass[] = "32sherwoodsecurity#";

BlynkTimer timer;

// Hardware Serial
#define EspSerial Serial

// ESP8266 baud rate:
#define ESP8266_BAUD 115200

ESP8266 wifi(&EspSerial);

void timerEvent(){
  unsigned long currMillis = millis();
  if ((unsigned long) (currMillis - prevMillis) < idlePeriod) wdt_reset();

  else {
  // Take multiple TMP36 readings and average to smooth sensor signal
  int reading_total = 0;
  for (int i=0; i <= 9; i++) {
    int reading = analogRead(tmp36Pin); // Get the voltage reading from the temperature sensor
    reading_total += reading;
    delay(50);
  }
  int avg_reading = reading_total / 10.0;
  
  // Convert that reading to voltage, for 3.3v arduino use 3.3
  float voltage = avg_reading * aref_voltage;
  voltage /= 1024.0; 
 
  // Convert from 10 mv per degree wit 500 mV offset to degrees ((voltage - 500mV) times 100)
  float temperatureC = (voltage - 0.5) * 100 ;
 
  // Convert Celsius to Fahrenheit
  float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;

  // Take reading on DHT22
  float hum = dht22.readHumidity();
  float tempf_22 = dht22.readTemperature(true);
  
  Blynk.virtualWrite(V0, temperatureF);
  Blynk.virtualWrite(V1, tempf_22);
  Blynk.virtualWrite(V2, hum);
  prevMillis = currMillis;
  wdt_reset();
  }
}

void setup()
{
  analogReference(EXTERNAL);

  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  
  dht22.begin();
  
  Blynk.begin(auth, wifi, ssid, pass);
  timer.setInterval(5000L, timerEvent);
  
  wdt_enable(WDTO_8S);
}

void loop()
{
  if (Blynk.connected()){
  Blynk.run();
  timer.run();
  }
  else {
    while (true){};
  }
}



