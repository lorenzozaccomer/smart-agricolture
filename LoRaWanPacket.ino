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
int32_t i;
#define DELAY_UPDATE 2000
SX1272 radio = new Module(10, 2, 19, 3);

//// LORAWAN DEVADD AND KEY DEFINE
const char *devAddr = "AAC44AC6";
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
    Serial.print(F("[SX1272] Reading data ... \n"));
    String Data_string = ReadDataFromSensor();
    String Encoded_string = LoraPacketEncoder(Data_string);
    Serial.print(F("[SX1272] Transmitting packet ... "));
    DataTransmitter(Encoded_string);  
  }
  LowPower.deepSleep(3000);
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

String LoraPacketEncoder(String Data_in)
{
  String Data_out = {};
  LoRaWanPacket.clear();
  LoRaWanPacket.print(Data_in);
  if (LoRaWanPacket.encode())
  {
    LORA_HEX_PRINTLN(Serial, LoRaWanPacket.buffer(), LoRaWanPacket.length());
    for (i = 0; i < LoRaWanPacket.length(); i++){
      if(LoRaWanPacket.buffer()[i] < 0x10){Data_out += '0';}
      Data_out += String(LoRaWanPacket.buffer()[i],HEX);
    }
  }

  return Data_out;
}

void DataTransmitter(String packet)
{
  int state = radio.transmit(packet);
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
  radio.sleep();  
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
