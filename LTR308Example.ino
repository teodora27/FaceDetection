#include <LTR308.h>
#include <Wire.h>
#include "BluetoothSerial.h"

// Create an LTR308 object, here called "light":
LTR308 light;

// Global variables:
unsigned char gain = 0;  // Gain setting, values = 0-4 
unsigned char integrationTime = 0;  // Integration ("shutter") time, values 0 - 4
unsigned char measurementRate = 3;  // Interval between DATA_REGISTERS update, values 0 - 7, except 4
unsigned char ID;
unsigned char control;

String device_name = "ESP32-Victor";

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Port Profile for Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

void setup() {
  // Initialize the Serial port:
  Serial.begin(9600);
  Serial.println();
  SerialBT.begin(device_name);
  Serial.println("LTR-308ALS example sketch");

  // Initialize the LTR308 library
  delay(100);

  // You can pass nothing to light.begin() for the default I2C address (0x53)
  light.begin();

  // Get factory ID from sensor:
  if (light.getPartID(ID)) {
    Serial.print("Got Sensor Part ID: 0X");
    Serial.print(ID, HEX);
    Serial.println();
  } else {
    byte error = light.getError();
    printError(error);
  }
  
  // Power up the sensor
  if (light.setPowerUp()) {
    Serial.print("Powering up...");
    Serial.println();
  } else {
    byte error = light.getError();
    printError(error);
  }
  delay(10);

  if (light.getPower(control)) {
    Serial.print("Control byte is: 0X");
    Serial.print(control, HEX);
    Serial.println();
  } else {
    byte error = light.getError();
    printError(error);
  }
  
  // Setting Gain
  Serial.println("Setting Gain...");
  if (light.setGain(gain)) {
    light.getGain(gain);
    Serial.print("Gain Set to 0X");
    Serial.print(gain, HEX);
    Serial.println();
  } else {
    byte error = light.getError();
    printError(error);
  }

  // Set timing
  Serial.println("Set timing...");
  if (light.setMeasurementRate(integrationTime, measurementRate)) {
    light.getMeasurementRate(integrationTime, measurementRate);
    Serial.print("Timing Set to ");
    Serial.print(integrationTime, HEX);
    Serial.println();

    Serial.print("Meas Rate Set to ");
    Serial.print(measurementRate, HEX);
    Serial.println();
  } else {
    byte error = light.getError();
    printError(error);
  }
}

void loop() {
  int ms = 1000;
  delay(ms);
  
  unsigned long rawData;
  if (light.getData(rawData)) {
    Serial.print("Raw Data: ");
    Serial.println(rawData);
    
    double lux;    // Resulting lux value
    boolean good;  // True if sensor is not saturated
    
    good = light.getLux(gain, integrationTime, rawData, lux);
    
    if (lux > 200) {
      String message = "Se poate face recunoastere faciala. " ;
      Serial.println(message + (good ? " (valid data)" : " (BAD)"));
      SerialBT.println(message);
    } else {
      String message = "Nu se poate face recunoastere faciala. " ;
      Serial.println(message);
      SerialBT.println(message);
    }
  } else {
    byte error = light.getError();
    printError(error);
  }
}

void printError(byte error) {
  // If there's an I2C error, this function will print out an explanation.
  Serial.print("I2C error: ");
  Serial.print(error, DEC);
  Serial.print(", ");
  switch(error) {
    case 0:
      Serial.println("success");
      break;
    case 1:
      Serial.println("data too long for transmit buffer");
      break;
    case 2:
      Serial.println("received NACK on address (disconnected?)");
      break;
    case 3:
      Serial.println("received NACK on data");
      break;
    case 4:
      Serial.println("other error");
      break;
    default:
      Serial.println("unknown error");
  }
}
