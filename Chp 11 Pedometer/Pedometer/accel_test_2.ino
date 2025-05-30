#include "GY521.h"

float x = 0, y = 0, z = 0; // x,y and z readings initialised to 0
float old_x = 0, old_y = 0, old_z = 0; // old x, y and z readings to find change
String x_diff = "", y_diff = "", z_diff = ""; // x, y and z differences
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
  old_x = x; // Store old X acceleration
  old_y = y; // Store old Y acceleration
  old_z = z; // Store old Z acceleration
  x = sensor.getAccelX(); // Get X acceleration
  y = sensor.getAccelY(); // Get Y acceleration
  z = sensor.getAccelZ(); // Get Z acceleration
  x_diff = String(x - old_x); // Calculate X difference
  y_diff = String(y - old_y); // Calculate Y difference
  z_diff = String(z - old_z); // Calculate Z difference
  if (x_diff.length() == 4) {
    x_diff = " "+x_diff;
  }
  if (y_diff.length() == 4) {
    y_diff = " "+y_diff;
  }
  if (z_diff.length() == 4) {
    z_diff = " "+z_diff;
  }
  
  Serial.print("\t");
  Serial.print(x);
  Serial.print("\t");
  Serial.print(x_diff);        // Output the data on a single line
  Serial.print("\t|\t");
  Serial.print(y);
  Serial.print("\t");
  Serial.print(y_diff);
  Serial.print("\t|\t");
  Serial.print(z);
  Serial.print("\t");
  Serial.print(z_diff);
  Serial.print("\t|\t");
  Serial.println();
  delay(50); //Wait before checking again.
}
