/* 
*  Simple remote relay control with Arduino & the CC3000 chip
*  Part of the code is based on the work done by Adafruit on the CC3000 chip
*  Writtent by Marco Schwartz for Open Home Automation
*/

// Include required libraries
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
#include "utility/debug.h"
#include<stdlib.h>

// Define CC3000 chip pins
#define ADAFRUIT_CC3000_IRQ   3
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
 
const int relay_pin = 8; // Relay pin

const unsigned long
  dhcpTimeout     = 60L * 1000L, // Max time to wait for address from DHCP
  connectTimeout  = 15L * 1000L, // Max time to wait for server connection
  responseTimeout = 15L * 1000L; // Max time to wait for data from server
uint32_t t;

// WiFi network (change with your settings !)
#define WLAN_SSID       "FreeboxS"       // cannot be longer than 32 characters!
#define WLAN_PASS       "billylechat"
#define WLAN_SECURITY   WLAN_SEC_WPA2 // This can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2

// Create CC3000 instances
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIV2);                                
Adafruit_CC3000_Client client;
                                         
// Local server IP, port, and repository (change with your settings !)
uint32_t ip = cc3000.IP2U32(192,168,0,14);
int port = 80;
String repository = "/arduino-relay-wifi/";

void setup() {
   
  pinMode(relay_pin,OUTPUT);
  Serial.begin(115200);
  
  // Initialise the CC3000 module
  if (!cc3000.begin())
  {
    while(1);
  }

  // Connect to  WiFi network
  cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY);
  Serial.println("Connected to WiFi network!");
    
  // Check DHCP
  Serial.print(F("Requesting address from DHCP server..."));
  for(t=millis(); !cc3000.checkDHCP() && ((millis() - t) < dhcpTimeout); delay(1000));
  if(cc3000.checkDHCP()) {
    Serial.println(F("OK"));
  } else {
    Serial.println(F("failed"));
    return;
  }

}

void loop() {
  
   // Send request to get relay state
   String request = "GET "+ repository + "server.php HTTP/1.0";
   String result = send_request(request);
  
   // Change relay state accordingly
   if(result.endsWith("Off"))
   {
     digitalWrite(relay_pin,LOW);
   }
      
   if(result.endsWith("On"))
   {
      digitalWrite(relay_pin,HIGH);
   }
}

// Function to send a TCP request and get the result as a string
String send_request (String request) {
  
    String result;
     
    // Connect    
    Serial.println("Starting connection to server...");
    Adafruit_CC3000_Client client = cc3000.connectTCP(ip, port);
    
    // Send request
    if (client.connected()) {
      client.println(request);      
      client.println(F(""));
      Serial.println("Connected & Data sent");
    } 
    else {
      Serial.println(F("Connection failed"));    
    }

    while (client.connected()) {
      while (client.available()) {

      // Read answer
      char c = client.read();
      result = result + c;
      }
    }
    Serial.println("Closing connection");
    Serial.println("");
    client.close();
    
    return result;
    
}
