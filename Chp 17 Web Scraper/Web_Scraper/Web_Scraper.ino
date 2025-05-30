#include <WiFiNINA.h>                 // WiFi library
#include <SPI.h>                      // SPI library
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
//const char* MY_SSID = "SECRET";       // Your WiFi SSID
//const char* MY_PWD = "SECRET";        // Your wiFi password
const char* MY_SSID = "YOURSSIDE";       // Your WiFi SSID
const char* MY_PWD = "YOURPWD";        // Your wiFi password
const int btn = 8;                      // Button pin

const int SSLPORT = 443;              // The port for secure connections
char WEBSITE[] = "ictsoeasy.co.uk";  //Host of the website
//char PATH[] = "/bad-jokes/bad-jokes-f.php"; //Path of webpage
String PATH_START = "/badjokes/bad-jokes-f.php?t=";

WiFiSSLClient client;                 // Initialise the client
int status = WL_IDLE_STATUS;          // The WiFi status
String htmlContent = "";
String prompt = "";
String punchline = "";

void setup() {
  Serial.begin(9600);                 // Sets the serial port to 9600
  delay(2000);//delay(5000);                        // 5s delay for serial to start
  lcd.init();                         // Initialize the lcd 
  lcd.backlight();                    // Turn the backlight on
  lcd.setCursor(5,0);                 // Set cursor col 3 row 0
  lcd.print("Wait!");                 // Output wait

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }
  
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(MY_SSID);
    status = WiFi.begin(MY_SSID, MY_PWD);
    delay(2000);//delay(10000);
  }
  lcd.setCursor(5,1);                 // Set cursor col 2 row 0
  lcd.print("Ready!");                // Output wready
  Serial.println("Connected");        // Confirm connection
  printCurrentNet();                  // Output network
  printWiFiData();                    //  details
}

void loop() {
  if (digitalRead(btn) == HIGH) {     // Button is pressed
    Serial.println("\nStarting connection to server...");
    httpRequest();                    // Request the data
    delay(1000);                      // Delay - just in case.
  }
}

void httpRequest() {
  // close any connection before sending a new request.
  // This will free the socket on the Nina module
  Serial.println("Stopping client.");
  client.stop();

  client.setTimeout(2000); // Extend the timeout in case the site is slow
  // If there is a successful connection
  if (client.connect(WEBSITE, SSLPORT)) {
    Serial.println("connecting...");
    // We build the path from the path itself then add on ?t=
    // and a changing number to stop the file getting cached.
    String PATH = PATH_START + String(millis()); 
    // Request the daa
    client.println("GET "+ PATH +" HTTP/1.1");
    client.println("Host: "+(String) WEBSITE);
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println();
    Serial.println("Finished connection");
    delay(1000); // Delay for the data to arrive
    htmlContent = "";
    // Echo the full html data for checking
    while (client.available()) {
      char c = client.read();
      htmlContent += c;
    }
    Serial.println(htmlContent);
    // Extract the prompt and punchline
    getPromptAndPunchline(htmlContent);
    // Display the joke
    displayJoke();
  } else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
}

void scrollMessage(int row, String message, int delayTime, int totalColumns) {
  for (int i=0; i < totalColumns; i++) {
    message = " " + message;  
  } 
  message = message + " "; 
  for (int position = 0; position < message.length(); position++) {
    lcd.setCursor(0, row);
    lcd.print(message.substring(position, position + totalColumns));
    delay(delayTime);
  }
}

void displayJoke() {
    lcd.clear();
    scrollMessage(0, prompt, 250, 16);
    scrollMessage(1, punchline, 250, 16);
}

void getPromptAndPunchline(String html) {
      //Get the prompt and punchline
    int start = htmlContent.indexOf(">")+1;
    prompt = htmlContent.substring(start);
    int end = prompt.indexOf("<");
    punchline = prompt.substring(end+8);
    prompt = prompt.substring(0,end);
    end = punchline.indexOf("<");
    punchline = punchline.substring(0,end);
    //Display on serial for debugging
    Serial.print("Prompt: ");
    Serial.println(prompt);
    Serial.print("Punchline: ");
    Serial.println(punchline);
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
