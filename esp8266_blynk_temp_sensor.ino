/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************

  This example shows how to use ESP8266 Shield (with AT commands)
  to connect your project to Blynk.

  WARNING!
    It's very tricky to get it working. Please read this article:
    http://help.blynk.cc/hardware-and-libraries/arduino/esp8266-with-at-firmware

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
//#define BLYNK_PRINT Serial

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <DHT.h>
#include <DHT_U.h>

#define aref_voltage 3.3

int dht22Pin = 2;
int tmp36Pin = 0;

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

  // Print TMP36 values to Serial
//  Serial.print("TMP36:\tVoltage: ");
//  Serial.print(voltage); Serial.print(" V\tTemperature: ");
//  Serial.print(temperatureF); Serial.println(" F");
//  
//  if (isnan(hum) || isnan(tempf_22)) {
//    Serial.println("Failed to read from DHT sensor!");
//    return;
//  }
//  Serial.print("DHT22:\tTemperature: "); Serial.print(tempf_22);
//  Serial.print(" F\tHumidity: "); Serial.print(hum); Serial.println(" %");
  
  Blynk.virtualWrite(V0, temperatureF);
  Blynk.virtualWrite(V1, tempf_22);
  Blynk.virtualWrite(V2, hum);
}

void setup()
{
  // Debug console
  //Serial.begin(9600);
  
  analogReference(EXTERNAL);

  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  
  dht22.begin();
  
  Blynk.begin(auth, wifi, ssid, pass);
  timer.setInterval(300000L, timerEvent);
}

void loop()
{
  Blynk.run();
  timer.run();
}



