#include "MQ135.h"                    // MQ-135 library
const int ANALOGPIN = 0;              // Pin to read the MQ-135
const float RZERO = 200.24;           // Define the calibration setting

MQ135 gasSensor = MQ135(ANALOGPIN, RZERO); // Initialise the MQ-135

void setup() {
  Serial.begin(9600);                 // Sets the serial port to 9600
}

void loop() {
  float ppm = gasSensor.getPPM();     // Read the parts per million
  Serial.println(ppm);                // Output the parts per million
  delay(1000);                        // Wait 1s
}
