#define SensorPin A0 
float sensorValue = 0; 
void setup() { 
 Serial.begin(9600); 
} 
void loop() { 
 sensorValue = analogRead(SensorPin); 
 Serial.print("Lettura sensore pioggia = ");
 Serial.println(sensorValue); 
 delay(1000); 
} 
