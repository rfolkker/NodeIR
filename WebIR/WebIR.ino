#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRtimer.h>
#include <IRutils.h>
#include <ir_Daikin.h>
#include <ir_Kelvinator.h>
#include <ir_LG.h>
#include <ir_Mitsubishi.h>
#include "Headers.h"
#include "JsonObject.h"
#include "SimpleObjectNotation.h"
#include "InitPageGenerator.h"
// wifi.h will contain the ssid and password
// Comment this out if you want to use it locally
// The wifi.h will not be included in the project
#include "wifi.h"
/*
 * Based off original code from:
 * WiFi IR Blaster by Buddy Crotty
 *  Use an ESP8266 module or dev board to receive HTTP GET request
 *  and then send IR codes to an attached IR LED based on those requests.
 *  This works best with another web server acting as a front end that 
 *  sends cURL requests based on which buttons are pressed. 
 *  cURL format: http://ESP8266/IRcode
 */

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#ifndef WIFI_H
const char* ssid = "MYSSID";
const char* password = "MYPASSWORD";
#endif
String dynamicSSID = "";
String dynamicPWD = "";
String validateURI = "";
String accessToken = "";
MDNSResponder mdns;
int khz = 38; // 38kHz carrier frequency for both NEC and Samsung
int count = 68;
IRsend irsend(4); //an IR led is connected to GPIO4 (pin D2 on NodeMCU)
short unsigned int genericIRCommand[100]; // This will be used from the Json result


// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);


void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();

  irsend.begin();
  WiFi.softAP("IRDoom", "Equivine");
    // Print the IP address
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected");

  Serial.println("Starting default Page trigger");
  dnsServer.start(DNS_PORT, "*", A)
  // Start the server
  server.begin();
}

void switchAccess(){
  Serial.println("Disconnecting Wireless AP");
  char irSSID[100];
  char irPWD[100];
  dnsServer.close();
  WiFi.disconnect(false);
    while (WiFi.status() != WL_DISCONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to "+ dynamicSSID);
  dynamicSSID.toCharArray(irSSID, 100);
  dynamicPWD.toCharArray(irPWD, 100);
  WiFi.begin(irSSID, irPWD);

  // TODO: Add the validation peice for OAuth here

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected");

  // Start the server
  server.begin();
  Serial.println("HTTP Server Started");

  // Print the IP address
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  if (mdns.begin("IRBlasterLR", WiFi.localIP())) {
    Serial.println("MDNS Responder Started");
  }

  Serial.println();
  Serial.println();
}
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  Headers webHeader;
  JsonObject JsonResult;
  
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  String remaining = "";
  String body="";
  String line="";
  String Manufacturer = "";
  uint64_t SignalData = 0;
  String responseString="HTTP/1.1 200 OK\r\n";

  Serial.println(req);
  while(client.available())
  {
    line = client.readStringUntil('\r');
    if(line.indexOf(":") == -1){
      break; // end of the header has been reached
    }
    webHeader.Parse(line);
    // remaining +=line;
    // Serial.println(line);
  }
  
  Serial.println("Header:");
  Serial.println("Host: "+webHeader.GetHost());
  Serial.println("User Agent: "+webHeader.GetUserAgent());
  Serial.println("Accept: " + webHeader.GetAccept());
  Serial.println("Accepted Language(s): "+webHeader.GetAcceptLanguage());
  Serial.println("Accepted Encoding: "+webHeader.GetAcceptEncoding());
  Serial.println("Content Type: "+webHeader.GetContentType());
  Serial.println("Content Length: "+webHeader.GetContentLengthText());
  Serial.println("Connection: "+webHeader.GetConnection());
  Serial.println("\r============");
  while(client.available())
  {
    line = client.readStringUntil('\r');
    body+=line;
  }  
  // trunc the newline character from the body block
  body.remove(0,1);
  client.flush();
  Serial.println("Body:");
  Serial.println(body);
  Serial.println("Size:"+ String(body.length()));
  if(body.length()==webHeader.GetContentLength()){
    Serial.println("Body is valid");
  }
  else{
    Serial.println("Body is invalid");
  }
  Serial.println("\r============");

  JsonResult.Parse(body);
  Serial.println("Json Result:");
  for(int counter=0; counter<JsonResult.GetSize();counter++){
    Serial.println(JsonResult.GetKey(counter)+": "+JsonResult.GetValue(counter));
  }
  Serial.println("\r============");
//  if(req.indexOf("/irOut") !=-1){
//    irsend.sendRaw(powerIRout, 1, khz);
//  }
  // Match the request
  if(req.indexOf("/irInit") != -1){
    // We need to initialize the network
    Serial.println("new request to connect has been initiated");
    if(dynamicSSID.length()<2){
      dynamicSSID = JsonResult.GetValue("irSSID");
      dynamicPWD = JsonResult.GetValue("irPWD");
      accessToken = JsonResult.GetValue("irAuthKey");
      validateURI = JsonResult.GetValue("irAuthPath");
    }
  }
  else if(req.indexOf("/irRawCmd") !=-1){
    // Parse Json object, and use it to configure and send the command
    // StaticJsonBuffer<512> jsonBuffer;
    // JsonObject& root = jsonBuffer.parseObject("{testing: \"value\"}");
    // uint16_t* irCmd = root["command"];
    // int irCount = root["count"];
    // int irFreq = root["frequency"];
    // irsend.sendRaw(irCmd, irCount, irFreq);
    khz = JsonResult.GetValue("irFreq").toInt(); // 38kHz carrier frequency for both NEC and Samsung
    count = JsonResult.GetValue("irCount").toInt();
    JsonResult.GetValueArray("irCmd", genericIRCommand, count);

    Serial.println("Khz: "+ String(khz));
    Serial.println("Frequency Count: "+String(count));
    Serial.println("The Frequency List:");
    for(int counter=0;counter<count;counter++){
      Serial.println(String(genericIRCommand[counter]));
    }
    irsend.sendRaw(genericIRCommand, count, khz);
    Serial.println("IR Command was sent");
    
    // Serial.println(req);
  }
  else if(req.indexOf("/irCmd")){
    Serial.println("System Specific Command recieved");
    Manufacturer = JsonResult.GetManufacturer();
    SignalData = JsonResult.GetData();
    Serial.println("Manufacturer identified as: "+Manufacturer);
    if(Manufacturer == "Samsung"){
      Serial.println("Succesfully identified Samsung, attempting to execute command");
      if(SignalData!=0){
        Serial.println("Command is valid.");
        irsend.sendSAMSUNG(SignalData);
        Serial.println("IR Command ["+JsonResult.GetValue("irData")+"] was sent");
      }
    }
    else if(Manufacturer == "Dish"){
      Serial.println("Succesfully identified Dish, attempting to execute command");
      if(SignalData!=0){
        Serial.println("Command is valid.");
        irsend.sendDISH(SignalData);
        Serial.println("IR Command ["+JsonResult.GetValue("irData")+"] was sent");
      }
    }
  }
  else {
    Serial.println("default request triggered");
    Serial.println(req);
    InitPageGenerator defaultGenerator;
    responseString = defaultGenerator.GetDefaultPageFile();
  }
  
  client.flush();
   
  // Send the response to the client
  //client.print(s);
  client.print(responseString);
  delay(1);
  Serial.println("Client Disconnected");
  Serial.println();
  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}


