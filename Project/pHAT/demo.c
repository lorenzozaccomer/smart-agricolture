/********************
 * This demo is only supported after RUI firmware version 3.0.0.13.X on RAK811
 * Master Board Uart Receive buffer size at least 128 bytes. 
 ********************/

#include "RAK811.h"
#define WORK_MODE LoRaWAN   //  LoRaWAN or LoRaP2P
#define JOIN_MODE ABP    //  OTAA or ABP
#if JOIN_MODE == OTAA
String DevEui = "60C5A8FFFE7986A0"; // -> Changed
String AppEui = "60C5A8FFFE7986A0"; // I'm using this one
String AppKey = "7808088192B3B0653DB92C662589578D";  // -> Changed
#else JOIN_MODE == ABP
String NwkSKey = "60C5A8FFFE7986A0AC1F09FFF8680811";  // -> Changed
String AppSKey = "60C5A8FFFE7986A0AC1F09FFF8680811";  // -> Changed
String DevAddr = "FE7986A0";  // -> Changed
#endif

#define DebugSerial Serial
#define LoRaSerial Serial4 

char buffer[60] = {0};
char res[15];
char hi[3] = {'h','i', '\0'};

bool InitLoRaWAN(void);
RAK811 RAKLoRa(LoRaSerial,DebugSerial);


void setup() {
  DebugSerial.begin(115200);
  while(!DebugSerial)
    ;
  DebugSerial.println(F("********************************************************"));
  DebugSerial.println(F("LoRaWAN"));
  DebugSerial.println(F("********************************************************"));

  LoRaSerial.begin(115200); //set LoRaSerial baudrate:This baud rate has to be consistent with  the baud rate of the WisNode device.
  while(LoRaSerial.available())
  {
    LoRaSerial.read(); 
  }

  RAKLoRa.rk_setWorkingMode(0);
//  if(!RAKLoRa.rk_setWorkingMode(0))  //set WisNode work_mode to LoRaWAN.
//  {
//    DebugSerial.println(F("set work_mode failed, please reset module."));
//    while(1);
//  }
  
  RAKLoRa.rk_getVersion();  //get RAK811 firmware version
  DebugSerial.println(RAKLoRa.rk_recvData());  //print version number

  DebugSerial.println(F("Start init RAK811 parameters..."));

  if (!InitLoRaWAN())  //init LoRaWAN
  {
    DebugSerial.println(F("Init error,please reset module.")); 
    while(1);
  }

  DebugSerial.println(F("Start to join LoRaWAN..."));
  while(!RAKLoRa.rk_joinLoRaNetwork(60))  //Joining LoRaNetwork timeout 60s
  {
    DebugSerial.println();
    DebugSerial.println(F("Rejoin again after 5s..."));
    delay(5000);
  }
  DebugSerial.println(F("Join LoRaWAN success"));

  if(!RAKLoRa.rk_isConfirm(0))  //set LoRa data send package type:0->unconfirm, 1->confirm
  {
    DebugSerial.println(F("LoRa data send package set error,please reset module.")); 
    while(1);    
  }
}

void loop() {
  DebugSerial.println(F("Start send data..."));

  int resistance = (int)((analogRead(A1)+250) * (3.3/1023.0) * 100); //123.45;
  itoa(resistance, res, 10);
  sprintf(buffer, "%s:%s", hi, res);

  DebugSerial.println(resistance/100.);
  
  if (RAKLoRa.rk_sendDataASCII(1, buffer, strlen(buffer)))
  {    
    for (unsigned long start = millis(); millis() - start < 60000L;)
    {
      String ret = RAKLoRa.rk_recvData();
      if(ret != "")
      { 
        DebugSerial.println(ret);
      }
    }
  }
}

bool InitLoRaWAN(void)
{
  #if JOIN_MODE == OTAA
  if(RAKLoRa.rk_setJoinMode(JOIN_MODE))  //set join_mode:OTAA
  {
    if(RAKLoRa.rk_setRegion(1))  //set region AU915
    {
      if (RAKLoRa.rk_initOTAA(DevEui, AppEui, AppKey))
      {
        DebugSerial.println(F("RAK811 init OK!"));  
        return true;    
      }
    }
  }
  return false;
  #else
  if(RAKLoRa.rk_setJoinMode(JOIN_MODE))  //set join_mode:ABP
  {
    if(RAKLoRa.rk_setRegion(1))  //set region EU868, alterado de 5 para 1
    {
      if (RAKLoRa.rk_initABP(DevAddr, NwkSKey, AppSKey))  //set ABP mode parameters
      {
        DebugSerial.println(F("RAK811 init OK!"));  
        return true;    
      }
    }
  }
  #endif
}
```