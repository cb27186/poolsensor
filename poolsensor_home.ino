//Poolsensor Script by Chistian Böö
//August 2024
//Version 1.1

#include <Arduino.h>

#include "EspMQTTClient.h"

#include <OneWire.h>
#include <DallasTemperature.h>

EspMQTTClient client(
  "SSID",
  "PASSWORD",
  "192.168.2.119",  // MQTT Broker server ip
  "mqtt_user",   // MQTT User
  "mqtt_pass",   // MQTT Passwort
  "Poolsensor",     // Client name
  2786              // MQTT port
);
 
#define ONE_WIRE_PIN D2

#define sensivity (3.3 / 1023.0)
 #define maxVoltage 3.3

 OneWire oneWire(ONE_WIRE_PIN);
DallasTemperature sensors(&oneWire);
const float No_Val = 999.99;

char temp[32];
char perc[32];
char sensorv[32];
 
 void setup() {
   Serial.begin(9600);
   //delay(3000);
   sensors.begin();
   delay(3000);
client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
//client.enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overridded with enableHTTPWebUpdater("user", "password").
 // client.enableOTA(); // Enable OTA (Over The Air) updates. Password defaults to MQTTPassword. Port is the default OTA port. Can be overridden with enableOTA("password", port).

}

// This function is called once everything is connected (Wifi and MQTT)
// WARNING : YOU MUST IMPLEMENT IT IF YOU USE EspMQTTClient
void onConnectionEstablished()
{

  // Publish a message to "mytopic/test"
  //start:
  Serial.print("start...");
  delay(3000);
  sensors.requestTemperatures();
  delay(3000);
  float sensorValue = analogRead(A0);
  sensorValue = sensorValue * sensivity;
     //float percentage = (sensorValue / maxVoltage) * 100.0;
     float percentage = 100- (((maxVoltage - sensorValue) / maxVoltage) * 700);
     dtostrf(sensors.getTempCByIndex(0), 10, 2, temp);
     dtostrf(percentage, 10, 2, perc);
     dtostrf(sensorValue, 10, 2, sensorv);
      if (sensors.getTempCByIndex(0) == DEVICE_DISCONNECTED_C) {
    client.publish("Poolsensor/Temp", "Fehler");
    //goto start;
        }
        else {
     client.publish("Poolsensor/Temp", temp);
        }
 // client.publish("Poolsensor/Temp", temp); // You can activate the retain flag by setting the third parameter to true
client.publish("Poolsensor/Akku", perc);
client.publish("Poolsensor/AkkuV", sensorv);
delay(15000);

ESP.deepSleep(1800 * 1000000);
}

 void loop() {
 //  float sensorValue = analogRead(A0);
 //  Serial.print("Raw adc: ");
 //  Serial.println(int(sensorValue));
 //  sensorValue = sensorValue * sensivity;
 //  Serial.print("Voltage: ");
 //  Serial.print(sensorValue);
 //  Serial.println("V");
   // Calculate the percentage level
 //  float percentage = (sensorValue / maxVoltage) * 100.0;
 //  Serial.print("Percentage: ");
 //  Serial.print(percentage);
 //  Serial.println("%");
   //sensors.requestTemperatures();
   //if (sensors.getTempCByIndex(0) == DEVICE_DISCONNECTED_C) {
            //sensors.getTempCByIndex(0) = No_Val;
   //       Serial.println("Fehler");
    //    }
    //    else {
    //      Serial.print(sensors.getTempCByIndex(0));
    //      Serial.println(" 'C");
    //    }
 //     delay(1000);
      client.loop();
      
 }
