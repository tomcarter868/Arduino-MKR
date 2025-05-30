#include "GY521.h"                    // Accel library
#include <WiFiNINA.h>                 // WiFi library
#include <SPI.h>                      // SPI library

int buttonPin = 13;                   // Trigger button
GY521 sensor(0x68);                   // Sensor object
float x = 0, y = 0, z = 0;            // Accel readings
const char* MY_SSID = "Police Surveillance Vehicle AB15";       // Your WiFi SSID
const char* MY_PWD = "abcdefgh";        // Your wiFi password

const int SSLPORT = 443;              // The port for secure connections
const char WEBSITE[] = "script.google.com"; // Server address to connect to
const char* DEPLOYMENTID = "AKfycbyjDPwH2hxQ9KcewTsNDM-L8Mb9MVIvaOp3YAl2ToTJmrd60CfdDfKOrqjLzW6-TodkxQ";

WiFiSSLClient client;                 // Initialise the client
int status = WL_IDLE_STATUS;          // The WiFi status

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);       // Enable the built in LED
  pinMode(buttonPin,INPUT_PULLUP);    // Setup the button
  Serial.begin(9600);                 // Begin serial
  Wire.begin();                       // Connect to the accelerometer bus
  delay(5000);                        // wait 10s as may not be connected to serial
  while (1) {
    if (sensor.wakeup()) {            //Is there an accel connection?
      Serial.println("Accellerometer connected");
      break; //and leave the loop
    } else {
      Serial.println("Accellerometer is not connected");
    }
  }
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(MY_SSID);
    status = WiFi.begin(MY_SSID, MY_PWD);
    delay(10000);
  }
  Serial.println("Connected");        // Flash the built in LED to show everything is ready.
  for (int i = 0; i <10; i++) {
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(200);                       // wait 
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    delay(200);                       // wait 
  }
}

void loop() {
  int buttonPressed = digitalRead(buttonPin);
  double readings[200];
  if (buttonPressed == LOW) {
    for (int counter = 0; counter < 200; counter++) {
      sensor.read();
      x = sensor.getAccelX();
      y = sensor.getAccelY();
      z = sensor.getAccelZ();
      Serial.print(x);
      Serial.print("\t");
      Serial.print(y);
      Serial.print("\t");
      Serial.print(z);
      Serial.print("\t");
      double gforce = sqrt(x*x+y*y*z*z);
      Serial.println(gforce);
      readings[counter] = gforce;
      delay(50);
    }
    String URL = (String) "/macros/s/"+DEPLOYMENTID+"/exec?" + "data=";
    for (int counter = 0; counter < 200; counter++) {
      URL += readings[counter];
      URL += ",";
    }
    httpRequest(URL);
  }
}

void httpRequest(String URL) {
   // close any connection before sending a new request.
  // This will free the socket on the Nina module
  client.stop();
  // if there's a successful connection:
  if (client.connect(WEBSITE, SSLPORT)) {
    Serial.println("connecting...");
    // send the HTTP PUT request:
    client.println("GET "+URL+" HTTP/1.1");
    client.println("Host: "+(String) WEBSITE);
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println();
    Serial.println("Data sent.");
    // note the time that the connection was made:
  } else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
}