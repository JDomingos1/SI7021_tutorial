
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

// data pins for i2c, can be anything for ESP8266 
// for Arduino:
// Board              I2C / TWI pins
// Uno, Ethernet      A4 (SDA),   A5 (SCL)
// Mega2560           20 (SDA),   21 (SCL)
// Leonardo            2 (SDA),    3 (SCL)
// Due                20 (SDA),   21 (SCL)

// function for reading values into a array of unsigned int data[2] 
// from the Si7021 with a specific i2c address si7021Addr=0x40
// with one of the commands listed above. Function body at the end of this sketch
void getSiData(unsigned int *_ret_data, byte _i2c_command);
 
void setup()
{
  // Initiate the Wire library and join the I2C bus
  Wire.begin();

  // Initiate serial communication
  Serial.begin(115200);

  //reset sensor by sending 0xFE command to the Si7021 address
  Wire.beginTransmission(si7021Addr);
  Wire.write(0xFE); // Write reset command
  Wire.endTransmission();
  delay(15); // Default = 15ms

  
}
 
void loop()
{
  //sensor returns 2 bytes via I2C. It will be converted to temperature or humidity later
  unsigned int data[2];
 
  //Send humidity measurement command and get response into the array 'data'
  getSiData(data, 0xE5);
 
  // Convert the data
  float humidity  = ((data[0] * 256.0) + data[1]);
  humidity = ((125 * humidity) / 65536.0) - 6;
 
  // Send temperature measurement command
  getSiData(data, 0xE3);
  
  
  // Convert the data
  float temp  = ((data[0] * 256.0) + data[1]);
  float celsTemp = ((175.72 * temp) / 65536.0) - 46.85;
  float fahrTemp = celsTemp * 1.8 + 32;
 
  // Output data to serial monitor
  Serial.print("Humidity : ");
  Serial.print(humidity);
  Serial.println(" % RH");
  Serial.print("Celsius : ");
  Serial.print(celsTemp);
  Serial.println(" C");

  delay(10000);
}

void getSiData(unsigned int *_ret_data, byte _i2c_command)
{

  // start i2c communication 
  Wire.beginTransmission(si7021Addr);
  
  //send i2c command to sensor
  Wire.write(_i2c_command);

  // we are done with our transmission...close i2c communication
  Wire.endTransmission();
  delay(85);
 
  // Request 2 bytes of data
  Wire.requestFrom(si7021Addr, 2);

  // Read 2 bytes of data and save it to _ret_data which points to 'data[2]'
  if(Wire.available() == 2)
  {
    _ret_data[0] = Wire.read();
    _ret_data[1] = Wire.read();
  }
}
