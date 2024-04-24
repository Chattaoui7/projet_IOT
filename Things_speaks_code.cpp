/*
  WriteMultipleFields
  
  Description: Writes values to fields 1,2,3,4 and status in a single ThingSpeak update every 20 seconds.
  
  Hardware: ESP8266 based boards
  
  !!! IMPORTANT - Modify the secrets.h file for this project with your network connection and ThingSpeak channel details. !!!
  
  Note:
  - Requires ESP8266WiFi library and ESP8622 board add-on. See https://github.com/esp8266/Arduino for details.
  - Select the target hardware from the Tools->Board menu
  - This example is written for a network using WPA encryption. For WEP or WPA, change the WiFi.begin() call accordingly.
  
  ThingSpeak ( https://www.thingspeak.com ) is an analytic IoT platform service that allows you to aggregate, visualize, and 
  analyze live data streams in the cloud. Visit https://www.thingspeak.com to sign up for a free account and create a channel.  
  
  Documentation for the ThingSpeak Communication Library for Arduino is in the README.md folder where the library was installed.
  See https://www.mathworks.com/help/thingspeak/index.html for the full ThingSpeak documentation.
  
  For licensing information, see the accompanying license file.
  
  Copyright 2020, The MathWorks, Inc.
*/
#include <ESP8266WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros
#include "DHTesp.h"

char ssid[] = "INPT-Test";   // your network SSID (name) 
char pass[] = "iinnpptt";   // your network password

//String GAS_ID = "AKfycbxCOugAJqVHd5ojlS_n1De2IxKZC3xoeIKxpucG_po"; //--> spreadsheet script ID
//const char* host = "script.google.com"; // only google.com not https://google.com

//#define UPDATE_INTERVAL_HOUR  (0)
//#define UPDATE_INTERVAL_MIN   (0)
//#define UPDATE_INTERVAL_SEC   (30)

//#define UPDATE_INTERVAL_MS    ( ((UPDATE_INTERVAL_HOUR*60*60) + (UPDATE_INTERVAL_MIN * 60) + UPDATE_INTERVAL_SEC ) * 1000 )

WiFiClient  client;
DHTesp dht;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;
const int DHT_PIN = 2; // DHT22 sensor GPIO Pin
//int Temperature;
//int Humidity;

/*
void update_google_sheet()
{
    Serial.print("connecting to ");
    Serial.println(host);
  
    // Use WiFiClient class to create TCP connections
    WiFiClientSecure Client;
    const int httpPort = 443; // 80 is for HTTP / 443 is for HTTPS!
    
    Client.setInsecure(); // this is the magical line that makes everything work
    
    if (!Client.connect(host, httpPort)) { //works!
      Serial.println("connection failed");
      return;
    }
    //----------------------------------------Processing data and sending data
    String url = "/macros/s/" + GAS_ID + "/exec?temperature=";
   
    url += String(Temperature);
    
    url += "&humidity=";
    url += String(Humidity);
    
    Serial.print("Requesting URL: ");
    Serial.println(url);
  
    // This will send the request to the server
    Client.print(String("POST ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" + 
                 "Connection: close\r\n\r\n");
    Serial.println();
    Serial.println("closing connection");  
}
*/

void setup() {
  Serial.begin(115200);  // Initialize serial
  dht.setup(DHT_PIN, DHTesp::DHT11); // Initialize the DHT11 sensor
  WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
 while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Wifi not connected"); // Print a message if WiFi is not connected
  }
  Serial.println("Wifi connected !"); // Print a message if WiFi is connected 
  WiFi.mode(WIFI_STA); 
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}
/*
unsigned long time_ms;
unsigned long time_1000_ms_buf;
unsigned long time_sheet_update_buf;
unsigned long time_dif;
*/
void loop() {
  // Read temperature and humidity from the DHT22 sensor
  TempAndHumidity data = dht.getTempAndHumidity();
  //Temperature = data.temperature;
  //Humidity = data.humidity;
  //time_ms = millis();
  //time_dif = time_ms - time_1000_ms_buf;
  //if ( time_dif >= 1000 )// 1sec 
  // Set the value of field 1 in the ThingSpeak channel to the temperature
  ThingSpeak.setField(1,data.temperature);
  // Set the value of field 2 in the ThingSpeak channel to the humidity 
  ThingSpeak.setField(2,data.humidity);
  //int status = ThingSpeak.writeFields(SECRET_CH_ID,SECRET_WRITE_APIKEY); 
  Serial.println("Temp: " + String(data.temperature, 2) + "°C"); // Print the temperature value with 2 decimal places
  Serial.println("Humidity: " + String(data.humidity, 1) + "%"); // Print the humidity value with 1 decimal place
  
  if(ThingSpeak.writeFields(SECRET_CH_ID,SECRET_WRITE_APIKEY) == 200){
    Serial.println("Data pushed successfully"); // Print a message if the data was successfully pushed to ThingSpeak
  }else{
    Serial.println("Push error" + String(ThingSpeak.writeFields(SECRET_CH_ID,SECRET_WRITE_APIKEY))); // Print an error message with the HTTP status code if there was an error pushing the data
  }
  /*
  time_ms = millis();
  time_dif = time_ms - time_sheet_update_buf;  
  if ( time_dif >= UPDATE_INTERVAL_MS ) // Specific period
  {
    time_sheet_update_buf = time_ms;
    update_google_sheet();
  }
  */
  Serial.println("---"); // Print a separator line
  delay(500); // Delay for 10 seconds
}
