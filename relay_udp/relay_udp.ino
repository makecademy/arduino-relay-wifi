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
char relayBuffer[1]; // Relay buffer

const unsigned long
  dhcpTimeout     = 60L * 1000L, // Max time to wait for address from DHCP
  connectTimeout  = 15L * 1000L, // Max time to wait for server connection
  responseTimeout = 15L * 1000L; // Max time to wait for data from server
uint32_t t;

// WiFi network (change with your settings !)
#define WLAN_SSID       "yourNetwork"        // cannot be longer than 32 characters!
#define WLAN_PASS       "yourPassword"
#define WLAN_SECURITY   WLAN_SEC_WPA2 // This can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2

// Create CC3000 instances
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIV2);                                
Adafruit_CC3000_Client client;
                                         
// Local server IP, port, and repository (change with your settings !)
uint32_t ip = cc3000.IP2U32(192,168,0,1);
int port = 9999;
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
  relayBuffer[0] = 'r'; 
  send_request_udp();
  
  // Change relay state accordingly
  if (relayBuffer[0] == '0') {
    digitalWrite(relay_pin,LOW);
  }
  
   if (relayBuffer[0] == '1') {
    digitalWrite(relay_pin,HIGH);
  }
  
}

// Function to send a UDP request and get the result as a string
void send_request_udp () {
  
  unsigned long startTime;
    
  Serial.println(F("\r\nAttempting connection..."));
    startTime = millis();
    do {
      client = cc3000.connectUDP(ip, port);
    } while((!client.connected()) &&
            ((millis() - startTime) < connectTimeout));

    if(client.connected()) {
      Serial.print(F("connected!\r\nIssuing request..."));
      
      // Assemble and issue request packet
      client.write(relayBuffer, sizeof(relayBuffer));
      
      Serial.print(F("\r\nAwaiting response..."));
      startTime = millis();
      while((!client.available()) &&
            ((millis() - startTime) < responseTimeout));
      if(client.available()) {
        client.read(relayBuffer, sizeof(relayBuffer));
        Serial.println(relayBuffer);
      }

      client.close();
    }
    
    Serial.println("Time to connect, send & read:");
    Serial.println(millis() - startTime);
         
}
