/*
  LoRaWanPacket
  This code encode the LoRaWan payload using the keys.
  created 16 03 2019
  by Luiz H. Cassettari
*/

#include <LoRaWanPacket.h>
#include <RadioLib.h>

/*
 * To set the DevAddr you must use only HEXADECIMALE character
 * i.e. 0, ... 9, A, ..., F
 * 
 */

#define DELAY_UPDATE 2000

SX1272 radio = new Module(10, 2, 19, 3);

// LoraWan Constant
const char *devAddr = "AAABB14A";
const char *nwkSKey = "238792b74801de223b50ddd309b8f139";
const char *appSKey = "744826b8703e23a2e6cb8d79eed484bb";

void setup()
{
  Serial.begin(9600);
  while (!Serial);

  LoRaWanPacket.personalize(devAddr, nwkSKey, appSKey);
  Serial.print(F("[SX1272] Initializing ... "));
  int state = radio.begin(869.8, 125.0, 12);
  
  if (state == ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }
}

void loop() {
  if (runEvery(DELAY_UPDATE))
  {
    Serial.print(F("[SX1272] Reading data ... \n"));
    LoRaWanPacket.clear();
    String DataString = GenerateRandomDataSensor();
    String EncodedString = LoraPacketEncoder(DataString);
    Serial.print(F("[SX1272] Transmitting packet ... \n"));
    DataTransmitter(EncodedString);
  }
}

String GenerateRandomDataSensor()
{
    String DataOutput = {};
    float temp = random(-40,85);
    float humidity = random(0,100);
    float pressure = random(300,1100);
    float uv = random(320,410);
    float humidity_soil = random(0,100);

    DataOutput = "T:" + String(temp) + "$";
    DataOutput = DataOutput + "H:" + String(humidity) + "$";
    DataOutput = DataOutput + "P:" + String(pressure) + "$";
    DataOutput = DataOutput + "U:" + String(uv) + "$";
    DataOutput = DataOutput + "S:" + String(humidity_soil);

    return DataOutput;
}

String LoraPacketEncoder(String Data_in)
{
  String Data_out = {};
  
  LoRaWanPacket.clear();
  LoRaWanPacket.print(Data_in);
  if (LoRaWanPacket.encode())
  {
    LORA_HEX_PRINTLN(Serial, LoRaWanPacket.buffer(), LoRaWanPacket.length());
    for (int i = 0; i < LoRaWanPacket.length(); i++){
      if(LoRaWanPacket.buffer()[i] < 0x10){Data_out += '0';}
      Data_out += String(LoRaWanPacket.buffer()[i],HEX);
    }
  }

  return Data_out;
}


/*
 * @name: DataTransmitter
 * 
 * @input: string
 * 
 * This function will transmit the message on the world,
 * and will check if is all ok
 * 
 * @return: void 
 */

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
