#include "GY521.h"

float x = 0, y = 0, z = 0; //x,y and z readings initialised to 0
float oldx, oldy, oldz; //'old' readings to use for comparison
const int buzzer = 9; //buzzer to arduino pin 9
const int red = 2, green = 3, blue = 4; //PWM pins for LED
const int badRate = 2000; //The time between steps we will associate with 'bad'
const int warnRate = 1000; //The time between steps we will associate with 'getting bad'
unsigned long oldTime,newTime,elapsedTime; //Variables for calculating time between steps.

GY521 sensor(0x68);

void descending() { //A descending tone for signalling
  tone(buzzer, 1000); // Send 1KHz sound signal...
  delay(200);
  tone(buzzer, 500); // Send 500Hz sound signal...
  delay(200);
  tone(buzzer, 200); // Send 200Hz sound signal...
  delay(200);
  noTone(buzzer);     // Stop sound...
}

void ascending() {//A descending tone for signalling
  tone(buzzer, 200); // Send 200Hz sound signal...
  delay(200);
  tone(buzzer, 500); // Send 500Hz sound signal...
  delay(200);
  tone(buzzer, 1000); // Send 1KHz sound signal...
  delay(200);
  noTone(buzzer);     // Stop sound...
}

void buzz() { //The warning 'buzz'
  tone(buzzer, 1000); // Send 1KHz sound signal...
  delay(200);        // ...for 1 sec
  noTone(buzzer);     // Stop sound...
}

void setup(void) {
  Serial.begin(9600); //Try to connect to the serial monitor
  delay(1000);
  beeps();
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output
  pinMode(red, OUTPUT); // Set red LED pin
  pinMode(green, OUTPUT); // Set green LED pin
  pinMode(blue, OUTPUT); // Set blue LED pin
  descending(); //Notify we're beginning setup
  Wire.begin(); //Connect to the accelerometer bus
  Serial.println("Serial started");
  newTime = millis(); //Record the current uptime
  while (1) { //Keep trying
    if (sensor.wakeup()) { //Is there an accel connection?
      Serial.println("Accellerometer connected");
      break; //and leave the loop
    } else {
      Serial.println("Accellerometer is not connected");
    }
    delay(2000); //Wait 2s for everythign to settle down
  }
  sensor.setAccelSensitivity(0);  //  2g
  ascending();
  descending(); //Notify that setup is complete.
}

void beeps() {
  buzz();
  delay(1000);
  buzz();
  delay(1000);
  buzz();
  delay(1000);
  buzz();
  delay(1000);
  buzz();
}

void RGB_color(int red_light_value, int green_light_value, int blue_light_value) { 
  //Write an RGB colour to the RGB LED
  analogWrite(red, red_light_value);
  analogWrite(green, green_light_value);
  analogWrite(blue, blue_light_value);
}

void step() { //A step has happened
  oldTime = newTime; //Save the old time
  newTime = millis(); //Get the new time
  elapsedTime = newTime - oldTime; //Figure out the difference
  Serial.print(elapsedTime); 
  Serial.println();
  if (elapsedTime > badRate) { //Step was too long?
      buzz(); //Buzzing
      Serial.println("Step - bad rate"); //Notify
      RGB_color(255,0,0); //Red light!
  } else if (elapsedTime > warnRate) { //Almost too long!
      Serial.println("Step - warning rate"); //Notify
      RGB_color(255,255,0); //Yellow light
  }
  else { //All good
      Serial.println("Step - good"); //Notify
      RGB_color(0,255,0); //Green light
  }
  delay(100); //Wait so as to avoid double-steps
}



void loop(void) {
  oldx = x; //Save the old values
  oldy = y;
  oldz = z;
  sensor.read();
  x = sensor.getAccelX();
  y = sensor.getAccelY();
  z = sensor.getAccelZ();
  Serial.print(oldx);
  Serial.print(" - ");
  Serial.print(x);
  Serial.print(" | ");
  Serial.print(oldy);
  Serial.print(" - ");
  Serial.print(y);
  Serial.print(" | ");
  Serial.print(oldz);
  Serial.print(" - ");
  Serial.print(z);
  Serial.print(" | ");
  Serial.println();
  if (oldx < 0 && x > 0) { //If it changed in x from negative to positive G
    Serial.println("X Step");
    step(); //have a step
  }
//  if (oldy < 0 && y > 0) { //If it changed in x from negative to positive G
//    Serial.println("Y Step");
//    step(); //have a step
//  }
//  if (oldz < 0 && z > 0) { //If it changed in x from negative to positive G
//    Serial.println("Z Step");
//    step(); //have a step
//  }

  delay(50); //Wait before checking again.
}