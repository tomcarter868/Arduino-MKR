#include "MQ135.h"                    // MQ-135 library
const int ANALOGPIN = 0;              // Pin to read the MQ-135
MQ135 gasSensor = MQ135(ANALOGPIN);   // Initialise the MQ-135
int count = 0;                        // Count the number of reads
float total = 0;                      // Total up the read values

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);                 // Sets the serial port to 9600
}

void loop() {
  float rzero = gasSensor.getRZero(); // Read the RZero value
  total = total + rzero;              // Total it up
  count = count + 1;                  // Increment the count
  float avg = total / count;          // Calculate the average
  Serial.println(avg);                // Output the average
  delay(1000);                        // Wait 1s
}
