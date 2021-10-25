/*
  LoRaWanPacket
  This code encode the LoRaWan payload using the keys.
  created 16 03 2019
  by Luiz H. Cassettari
*/

#include <LoRaWanPacket.h>
#include <RadioLib.h>

SX1272 radio = new Module(10, 2, 19, 3);
const char *devAddr = "abc1234a";
const char *nwkSKey = "238792b74801de223b50ddd309b8f139";
const char *appSKey = "744826b8703e23a2e6cb8d79eed484bb";

void setup()
{
  Serial.begin(9600);
  while (!Serial);

  LoRaWanPacket.personalize(devAddr, nwkSKey, appSKey);
  Serial.print(F("[SX1278] Initializing ... "));
  int state = radio.begin(869.8, 125.0, 7);
  if (state == ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }
}

void loop() {
  if (runEvery(5000))
  {
    Serial.print(F("[SX1272] Transmitting packet ... "));
    LoRaWanPacket.clear();
    LoRaWanPacket.print("T:25:U:1566:X:111");
    if (LoRaWanPacket.encode()) 
    {
      //Serial.println("PACKET:");
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
    // the packet was successfully transmitted
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
  }
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