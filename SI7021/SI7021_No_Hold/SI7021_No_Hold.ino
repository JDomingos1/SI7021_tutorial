 /******************************I2C COMMAND TABLE*****************************
  * Measure Relative Humidity, Hold Master Mode         0xE5
  * Measure Relative Humidity, No Hold Master Mode      0xF5 
  * Measure Temperature, Hold Master Mode               0xE3
  * Measure Temperature, No Hold Master Mode            0xF3
  * Read Temperature Value from Previous RH Measurement 0xE0
  * Reset                                               0xFE  
  * Write RH/T User Register 1                          0xE6
  * Read RH/T User Register 1                           0xE7
  * Write Heater Control Register                       0x51
  * Read Heater Control Register                        0x11
  * Read Electronic ID 1st Byte                         0xFA 0x0F
  * Read Electronic ID 2nd Byte                         0xFC 0xC9
  * Read Firmware Revision                              0x84 0xB8
  * 
  * source datasheet
  */
 

// This library allows you to communicate with I2C.
#include <Wire.h>
 
// SI7021 I2C address is 0x40(64)
#define si7021Addr 0x40

// function for reading values into a array of unsigned int data[2] 
// from the Si7021 with a specific i2c address si7021Addr=0x40
// with one of the commands listed above. Function body at the end of this sketch

void setup(){
  // Initiate the Wire library and join the I2C bus
  Wire.begin();

  // Initiate serial communication
  Serial.begin(115200);

  //reset sensor by sending 0xFE command to the Si7021 address
  Wire.beginTransmission(si7021Addr);
  Wire.endTransmission(); // Write reset command
  delay(15); // Default = 15ms
}

void loop(){

  //sensor returns 2 bytes via I2C. It will be converted to temperature or humidity later
  unsigned int data[2];

  //Send humidity measurement command and get response into the array 'data'
  Wire.beginTransmission(si7021Addr);
  Wire.write(0xF5);
  Wire.endTransmission();
  delay(500);

  // Read 2 bytes of data and save it to _ret_data which points to 'data[2]'
  Wire.requestFrom(si7021Addr, 2);
 
      if(Wire.available() == 2){
      data[0] = Wire.read();
      data[1] = Wire.read();
  }

  // Convert the data
  float humidity  = ((data[0] * 256.0) + data[1]);
  humidity = ((125 * humidity) / 65536.0) - 6;

  // start i2c communication
  Wire.beginTransmission(si7021Addr);

  // Send temperature measurement command
  Wire.write(0xF3);
  Wire.endTransmission();
  delay(500);

  // Read 2 bytes of data and save it to _ret_data which points to 'data[2]'
  Wire.requestFrom(si7021Addr, 2);

      if(Wire.available() == 2){
      data[0] = Wire.read();
      data[1] = Wire.read();
  }

  // Convert the data
  float temp  = ((data[0] * 256.0) + data[1]);
  float celsTemp = ((175.72 * temp) / 65536.0) - 46.85;

  // Output data to serial monitor
  Serial.print("Humidity : ");
  Serial.print(humidity);
  Serial.println(" % RH");
  Serial.print("Celsius : ");
  Serial.print(celsTemp);
  Serial.println(" C");

  delay(1000);
}
