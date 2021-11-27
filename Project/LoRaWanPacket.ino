#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_VEML6070.h>
#include <LoRaWanPacket.h>
#include <RadioLib.h>
#include "STM32LowPower.h"

#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme; // INIT BME280
Adafruit_VEML6070 uv = Adafruit_VEML6070(); //INIT UV SENSOR
#define SoilSensor A1 
float sensorValue = 0; 
#define DELAY_UPDATE 10000
SX1272 radio = new Module(10, 2, 19, 3);

//// LORAWAN DEVADD AND KEY DEFINE
const char *devAddr = "AAC44AC5";
const char *nwkSKey = "238792b74801de223b50ddd309b8f139";
const char *appSKey = "744826b8703e23a2e6cb8d79eed484bb";


void setup()
{
  LowPower.begin();
  Serial.begin(9600);
  while (!Serial);

  LoRaWanPacket.personalize(devAddr, nwkSKey, appSKey);
  Serial.print(F("[SX1272] Initializing ... \n"));
  int state = radio.begin(869.8, 125.0, 7);
  bme.begin();
  uv.begin(VEML6070_1_T); 
}

void loop() {
  if (runEvery(DELAY_UPDATE))
  {
    Serial.print(F("[SX1272] Transmitting packet ... "));
    LoRaWanPacket.clear();
    String Output_string = ReadDataFromSensor();
    LoRaWanPacket.print(Output_string);
    if (LoRaWanPacket.encode()) 
    {
      LORA_HEX_PRINTLN(Serial, LoRaWanPacket.buffer(), LoRaWanPacket.length());
      //Serial.println("STRING:");
      String dato = {};
      //byte c  = LoRaWanPacket.buffer()[0];
      do { 
          int32_t i;
        for (i = 0; i < LoRaWanPacket.length(); i++){
          if(LoRaWanPacket.buffer()[i] < 0x10){dato += '0';}
          //Serial.print(LoRaWanPacket.buffer()[i],HEX);
          dato += String(LoRaWanPacket.buffer()[i],HEX); 
        }
     } while (0);
     //Serial.println(dato);
     //Serial.print("\n");
      
     int state = radio.transmit(dato);
     if (state == ERR_NONE) {
    //the packet was successfully transmitted
     Serial.println(F(" success!"));

    // print measured data rate
     Serial.print(F("[SX1272] Datarate:\t"));
     Serial.print(radio.getDataRate());
     Serial.println(F(" bps"));

  } else if (state == ERR_PACKET_TOO_LONG) {
    // the supplied packet was longer than 256 bytes
    Serial.println(F("too long!"));

  } else if (state == ERR_TX_TIMEOUT) {
    // timeout occurred while transmitting packet
    Serial.println(F("timeout!"));
  
  } else {
    // some other error occurred
    Serial.print(F("failed, code "));
    Serial.println(state);

  }
     
    }
   radio.sleep();
//   LowPower.deepSleep(8000);
   LowPower.deepSleep(9500);

//    Serial.print("Temperature = ");
//    Serial.print(bme.readTemperature());
//    Serial.println(" °C");
//
//    Serial.print("Pressure = ");
//
//    Serial.print(bme.readPressure() / 100.0F);
//    Serial.println(" hPa");
//
//    Serial.print("Approx. Altitude = ");
//    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
//    Serial.println(" m");
//
//    Serial.print("Humidity = ");
//    Serial.print(bme.readHumidity());
//    Serial.println(" %");
//
//    Serial.println();
//    Serial.print("UV light level: "); Serial.println(uv.readUV());
//    sensorValue = analogRead(SoilSensor); 
//    Serial.print("Lettura sensore umid. terreno = ");
//    Serial.println(sensorValue); 
  }
  
}

String ReadDataFromSensor() 
{ 
  String DataOut = {};
  float Temp = bme.readTemperature();
  float Pressure = bme.readPressure() / 100.0F;
  float Humidity = bme.readHumidity();
  int Soil = analogRead(SoilSensor);
  int UV = uv.readUV();
  DataOut = "T:" + String(Temp,2) + "$";
  DataOut = DataOut + "P:" + String(Pressure,2) + "$";
  DataOut = DataOut + "H:" + String(Humidity,2) + "$";
  DataOut = DataOut + "S:" + String(Soil) + "$";
  DataOut = DataOut + "U:" + String(UV);
   
  return DataOut;  
} 

boolean runEvery(unsigned long interval)
{
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}
