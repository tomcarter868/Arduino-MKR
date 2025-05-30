#include "MQ135.h"                    // MQ-135 library
const int ANALOGPIN = 0;              // Pin to read the MQ-135
const float RZERO = 201.96;            // Define the calibration setting
const int AWESOME = 400;              // This is better than clean outdoors!
const int GOOD = 1000;                // The limit between good and bad
const int BAD = 2000;                 // The limit between bad and awful

MQ135 gasSensor = MQ135(ANALOGPIN, RZERO); // Initialise the MQ-135

void setup() {
  Serial.begin(9600);                 // Sets the serial port to 9600
}

void loop() {
  float ppm = gasSensor.getPPM();     // Read the parts per million
  Serial.println(ppm);                // Output the parts per million
  if (ppm < AWESOME) {
    Serial.println("Awesome air");
  } else if (ppm < GOOD) {
    Serial.println("Good air");
  } else if (ppm < BAD) {
    Serial.println("Bad air - open a window!");
  } else {
    Serial.println("Awful air - get out now!");
  }
  delay(1000);                        // Wait 1s
}
