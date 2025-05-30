#include "MQ135.h"                    // MQ-135 library
#include <WiFiNINA.h>                 // WiFi library
#include <SPI.h>                      // SPI library

const int ANALOGPIN = 0;              // Pin to read the MQ-135
const float RZERO = 200.24;           // Define the calibration setting
const char* MY_SSID = "SECRET";       // Your WiFi SSID
const char* MY_PWD = "SECRET";        // Your wiFi password

const int SSLPORT = 443;              // The port for secure connections
const char WEBSITE[] = "script.google.com"; // Server address to connect to
const char* DEPLOYMENTID = "AKfycbzwlCsp9KRCtJC56aTbOananTum3Y_JATzTkKWB3vB3_HcplhH7UMG8-SwEDc2vXL_9";
                                      // Deployment ID from your script

MQ135 gasSensor = MQ135(ANALOGPIN, RZERO); // Initialise the MQ-135

WiFiSSLClient client;                 // Initialise the client

int status = WL_IDLE_STATUS;          // The WiFi status
unsigned long lastConnectionTime = 0; // last time you connected to the server, in milliseconds

const unsigned long postingInterval = 10L * 1000L; // delay between updates, in milliseconds


void setup() {
  Serial.begin(9600);                 // Sets the serial port to 9600
  while (!Serial) {
    ; // Wait for serial. Remember - this blocks so you might prefer a delay!
  }

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }
  
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(MY_SSID);
    status = WiFi.begin(MY_SSID, MY_PWD);
    delay(10000);
  }
  Serial.println("Connected");
  printCurrentNet();
  printWiFiData();
}

void loop() {
  if (millis() - lastConnectionTime > postingInterval) {
    float ppm = gasSensor.getPPM();     // Read the parts per million
    Serial.println(ppm);                // Output the parts per million
    // Build the URL string
    String URL = (String) "/macros/s/"+DEPLOYMENTID+"/exec?reading="+ (String) ppm;
    httpRequest(URL);
  }
}

// this method makes a HTTP connection to the server:
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
    lastConnectionTime = millis();
  } else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
}

void printWiFiData() {

  // print your WiFi shield's IP address:

  IPAddress ip = WiFi.localIP();

  Serial.print("IP Address: ");

  Serial.println(ip);

  Serial.println(ip);

  // print your MAC address:

  byte mac[6];

  WiFi.macAddress(mac);

  Serial.print("MAC address: ");

  printMacAddress(mac);

}

void printCurrentNet() {

  // print the SSID of the network you're attached to:

  Serial.print("SSID: ");

  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:

  byte bssid[6];

  WiFi.BSSID(bssid);

  Serial.print("BSSID: ");

  printMacAddress(bssid);

  // print the received signal strength:

  long rssi = WiFi.RSSI();

  Serial.print("signal strength (RSSI):");

  Serial.println(rssi);

  // print the encryption type:

  byte encryption = WiFi.encryptionType();

  Serial.print("Encryption Type:");

  Serial.println(encryption, HEX);

  Serial.println();
}

void printMacAddress(byte mac[]) {

  for (int i = 5; i >= 0; i--) {

    if (mac[i] < 16) {

      Serial.print("0");

    }

    Serial.print(mac[i], HEX);

    if (i > 0) {

      Serial.print(":");

    }

  }

  Serial.println();
}