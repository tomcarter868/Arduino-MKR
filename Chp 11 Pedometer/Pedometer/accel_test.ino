#include "GY521.h"

float x = 0, y = 0, z = 0; // x,y and z readings initialised to 0

GY521 sensor(0x68);

void setup() {
  Serial.begin(9600); // Try to connect to the serial monitor
  delay(5000); // Wait for the serial monitor
  Serial.println("Serial started");
  Wire.begin(); //Connect to the accelerometer bus
  while (1) { // Keep trying
    if (sensor.wakeup()) { // Is there an accel connection?
      Serial.println("Accellerometer connected");
      break; //and leave the loop
    } else {
      Serial.println("Accellerometer is not connected");
    }
  }
  delay(2000); //Wait 2s for everythign to settle down
  sensor.setAccelSensitivity(0);  //  2g
}

void loop() {
  sensor.read(); // Read the sensor data
  x = sensor.getAccelX(); // Get X acceleration
  y = sensor.getAccelY(); // Get Y acceleration
  z = sensor.getAccelZ(); // Get Z acceleration
  Serial.print(x);        // Output the data on a single line
  Serial.print(" | ");
  Serial.print(y);
  Serial.print(" | ");
  Serial.print(z);
  Serial.print(" | ");
  Serial.println();
  delay(500); //Wait before checking again.
}
