/*
 * Author: R. Kierstead <ron@rottenronnie.ca>
 *  This sketch sends sensor data via HTTP GET requests to variable you define as "host"
 *  Compiled and tested on NODEMCU DEVKIT v1 ESP8266-12E
 * Portions of code found here: https://alselectro.wordpress.com/2016/11/29/esp8266-wifi-library-on-arduino-ide/#comment-3551
 */

//Enable WIFI for the ESP8266
#include <ESP8266WiFi.h>

//WIFI Network Details - to connect to WiFi enter your SSID and password here:
const char* ssid = "Skynet 2.4GHz";
const char* password = "terminator";

//Time Interval to set delay in milliseconds before MySQL db is updated again.
const int updateInterval = 900000; //1min = 60000ms 15secs = 15000ms 15min = 900000ms 1hr = 3600000ms

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();

  //START NETWORK SETUP
  //disconnect any previous connections
  WiFi.disconnect();
  delay(1000);

  //scan for number of nearby networks & print SSIDs
  Serial.println();
  Serial.println("=====================================");
  Serial.print("Networks found 802.11g 2.4GHz       :");
  Serial.println(WiFi.scanNetworks());
  delay(500);
  Serial.println("List of available Access Points...  :");

//Declare the var "n" to print out a list of WiFi APs  
int n = WiFi.scanNetworks();

  for (int i = 0; i < n; i++) {
    Serial.println(WiFi.SSID(i));
  }
  Serial.println();

  //connect to preferred SSID
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();

  // print SSID name,local IP allotted ,MAC address & signal strength
  Serial.println();
  Serial.print("Connected to SSID          : ");
  Serial.println(WiFi.SSID());
  Serial.print("DHCP IP address            : ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC Address of ESP         : ");
  Serial.println(WiFi.macAddress());
  Serial.print("Signal strength is         : ");
  Serial.print(WiFi.RSSI());
  Serial.println(" db");
  Serial.println();
}
//END NETWORK SETUP

//webserver that will accept the HTTP GET data
const char* host = "192.168.0.109";

//Declare the port used to connect to a webserver
const int httpPort = 80;

//Declare the temperature sensor variables for temp and humidity used in the GET statement
//int celsius = 5;
int humidity = 5;


//Declare var "value"
int value = 0;

void loop() {
  // put your main code here, to run repeatedly:

  delay(5000);  //Wait 5 seconds
  ++value;

  //Use WifFiClient class to create TCP connections
  WiFiClient client;

  //Begin connecting to the webserver
  Serial.print(value);
  Serial.print(" - Connecting to ");
  Serial.println(host);

  if(client.connect(host,httpPort)) {
    Serial.println("Connected. Sending data.");
    Serial.print("Temperature ");
    Serial.print(value);
    Serial.print(" Deg. C., Relative Humidity ");
    Serial.print(humidity);
    Serial.print(" %");
    Serial.println();

    //Declare a URL for the request I should propbably do this as variables to make it easier for modified use. 
    
    String url = "/iot/add_data.php?";
    url += "celsius=";
    url += (value);
//    url += (celsius);
    url += "&humidity=";
    url += (humidity);

    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" + 
                 "Connection: close\r\n\r\n");
    //Print the information returned by the webserver...
    String line = client.readStringUntil('\r');
    Serial.println(line);
    Serial.print("Data sent. Connection closed for ");
    Serial.print(updateInterval);
    Serial.println("ms");
    Serial.println();
  }  
  
  while (client.available() == 0) {
    unsigned long timeout = millis();
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

delay(updateInterval);
}

