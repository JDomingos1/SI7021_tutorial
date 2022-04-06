#include <Wire.h>

#define si7021Addr 0x40

void setup(){
  Wire.begin();
  Serial.begin(9600);
  Wire.beginTransmission(si7021Addr);
  Wire.endTransmission();
  delay(300);
}

void loop(){
  unsigned int data[2];

  Wire.beginTransmission(si7021Addr);
  Wire.write(0xF5);
  Wire.endTransmission();
  delay(500);

  Wire.requestFrom(si7021Addr, 2);
 
  if(Wire.available() == 2){
    data[0] = Wire.read();
    data[1] = Wire.read();
  }

  float humidity  = ((data[0] * 256.0) + data[1]);
  humidity = ((125 * humidity) / 65536.0) - 6;

  Wire.beginTransmission(si7021Addr);

  Wire.write(0xF3);
  Wire.endTransmission();
  delay(500);

  Wire.requestFrom(si7021Addr, 2);

  if(Wire.available() == 2){
    data[0] = Wire.read();
    data[1] = Wire.read();
  }

  float temp  = ((data[0] * 256.0) + data[1]);
  float celsTemp = ((175.72 * temp) / 65536.0) - 46.85;
 
  Serial.print("Humidity : ");
  Serial.print(humidity);
  Serial.println(" % RH");
  Serial.print("Celsius : ");
  Serial.print(celsTemp);
  Serial.println(" C");

  delay(1000);
}
