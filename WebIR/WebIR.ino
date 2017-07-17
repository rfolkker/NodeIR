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
// wifi.h will contain the ssid and password
// Comment this out if you want to use it locally
// The wifi.h will not be included in the project
#include "wifi.h"
/*
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
MDNSResponder mdns;
int khz = 38; // 38kHz carrier frequency for both NEC and Samsung
int count = 68;
IRsend irsend(4); //an IR led is connected to GPIO4 (pin D2 on NodeMCU)
short unsigned int genericIRCommand[100]; // This will be used from the Json result

unsigned int powerIRout[68] = {4500,4450,650,1600,600,1650,550,1650,
600,500,600,550,550,550,550,550,550,550,600,1600,650,1600,600,
1650,600,500,550,550,550,550,600,550,500,600,550,550,550,1650,
650,500,550,550,550,550,550,600,500,600,500,600,500,1700,650,
500,500,1700,600,1600,650,1600,600,1600,650,1600,600,1600,650};

  // Insert RAW IR signal for "TV Power"
short unsigned int irTVpwr[68] = {4650,4250, 700,1550, 650,1550, 700,1550, 650,450, 650,500, 600,500, 600,500, 600,550, 550,1700, 550,1650, 600,1650, 550,550, 600,500, 600,550, 550,550, 600,500, 600,550, 550,1650, 600,550, 550,550, 600,500, 600,550, 550,550, 600,500, 600,1650, 600,500, 600,1650, 550,1700, 550,1650, 600,1650, 550,1650, 600,1650, 600,0};  // SAMSUNG E0E040BF

  // Insert RAW IR signal for "TV Source"
unsigned int irTVsrc[] = {4600,4300, 700,1550, 650,1550, 650,1600, 650,450, 650,450, 600,550, 550,550, 600,500, 600,1650, 550,1650, 600,1650, 550,550, 600,500, 600,550, 550,550, 550,550, 600,1650, 550,550, 550,550, 600,500, 600,500, 600,550, 550,550, 600,500, 600,550, 550,1650, 550,1700, 550,1650, 600,1600, 600,1650, 600,1600, 600,1650, 550};  // SAMSUNG E0E0807F
  
  // Insert RAW IR signal for "TV Mute"
unsigned int irTVmute[] = {4650,4250, 700,1550, 650,1550, 700,1550, 650,450, 650,500, 600,500, 600,500, 600,500, 600,1650, 600,1600, 600,1650, 550,550, 600,500, 600,550, 550,550, 600,500, 600,1650, 550,1650, 600,1650, 550,1650, 600,550, 550,550, 550,550, 600,500, 600,550, 550,550, 550,550, 600,500, 600,1650, 550,1650, 600,1650, 550,1650, 600};  // SAMSUNG E0E0F00F
  
  // Insert RAW IR signal for "TV Volume Down"
unsigned int irTVvdn[] = {4650,4250, 700,1550, 650,1550, 700,1550, 650,450, 650,450, 650,450, 600,550, 550,550, 600,1650, 550,1650, 550,1650, 600,550, 550,550, 550,550, 600,500, 600,500, 600,1650, 600,1600, 600,500, 600,1650, 550,550, 600,500, 600,500, 600,550, 550,550, 600,500, 600,1650, 550,550, 550,1650, 600,1650, 550,1650, 600,1650, 550};  // SAMSUNG E0E0D02F
  
  // Insert RAW IR signal for "TV Volume Up"
unsigned int irTVvup[] = {4600,4300, 650,1600, 650,1550, 650,1600, 600,500, 600,550, 600,500, 600,550, 550,550, 550,1700, 550,1650, 600,1650, 550,550, 600,500, 600,550, 550,550, 600,500, 600,1650, 600,1650, 550,1650, 600,550, 550,550, 600,500, 600,550, 550,550, 600,500, 600,550, 550,550, 600,1600, 600,1650, 600,1650, 550,1650, 600,1650, 600};  // SAMSUNG E0E0E01F
  
  // Insert RAW IR signal for "TV Channel Up"
unsigned int irTVchup[] = {4650,4250, 700,1550, 650,1600, 650,1550, 650,500, 600,500, 600,500, 650,500, 600,500, 600,1650, 550,1650, 600,1650, 600,500, 600,500, 600,550, 550,550, 600,550, 550,550, 550,1650, 600,550, 600,500, 600,1650, 550,550, 600,500, 600,550, 550,1650, 600,550, 550,1650, 600,1650, 600,500, 600,1650, 600,1600, 600,1650, 600};  // SAMSUNG E0E048B7
  
  // Insert RAW IR signal for "TV Channel Down"
unsigned int irTVchdn[] = {4600,4350, 650,1550, 650,1600, 650,1600, 600,500, 600,500, 600,550, 550,550, 600,550, 550,1650, 600,1650, 550,1700, 550,550, 550,550, 600,500, 600,550, 550,550, 600,500, 600,550, 550,550, 550,550, 600,1650, 600,500, 600,500, 600,550, 550,1650, 600,1650, 600,1650, 550,1650, 600,550, 550,1650, 600,1650, 600,1650, 550};  // SAMSUNG E0E008F7
  
  // Insert RAW IR signal for "Receiver Power"
unsigned int irRECpwr[] = {9050,4350, 650,500, 600,1600, 600,500, 650,500, 600,1600, 600,550, 600,1600, 600,1650, 550,550, 600,500, 600,1600, 650,1600, 600,500, 600,1650, 600,1600, 600,500, 600,1650, 600,1600, 600,550, 600,1600, 600,500, 600,550, 600,1600, 600,1600, 650,500, 600,500, 600,1600, 650,500, 600,1600, 600,1650, 600,500, 600,500, 600};  // NEC 4B36D32C

  // Insert RAW IR signal for "Receiver Power On"
unsigned int irRECpwrON[] = {9000,4400, 600,550, 600,1600, 600,500, 600,550, 600,1600, 600,500, 600,1600, 650,1600, 600,1600, 600,500, 650,1600, 600,1600, 600,500, 650,1600, 600,1600, 600,500, 600,550, 600,500, 600,1600, 600,550, 600,500, 600,500, 650,500, 600,500, 600,1600, 650,1600, 600,500, 600,1600, 650,1600, 600,1600, 600,1600, 600,1600, 650};  // NEC 4BB620DF
  
  // Insert RAW IR signal for "Receiver Power Off"
unsigned int irRECpwrOFF[] = {9000,4400, 600,550, 550,1650, 600,550, 550,550, 600,1650, 550,550, 600,1650, 550,1650, 600,550, 550,550, 550,1650, 600,1650, 600,550, 550,1650, 600,1650, 550,550, 600,1650, 550,1650, 600,1650, 600,500, 600,550, 550,550, 600,1650, 550,550, 600,500, 600,550, 550,550, 550,1700, 550,1650, 600,1650, 550,550, 600,1650, 550};  // NEC 4B36E21D

  // Insert RAW IR signal for "Receiver Mute"
unsigned int irRECmute[] = {9000,4400, 650,450, 650,1600, 600,500, 600,500, 650,1600, 600,500, 600,1650, 600,1600, 600,1600, 650,500, 600,1600, 650,1600, 600,500, 600,1600, 650,1600, 600,500, 600,1650, 600,500, 600,1600, 650,500, 600,500, 600,500, 600,500, 650,500, 600,500, 600,1600, 650,500, 600,1600, 600,1600, 650,1600, 600,1650, 600,1600, 600};  // NEC 4BB6A05F
  
  // Insert RAW IR signal for "Receiver Volume Down"
unsigned int irRECvdn[] = {9150,4250, 750,350, 700,1550, 700,400, 700,450, 650,1550, 700,450, 600,1600, 650,1600, 600,1650, 600,500, 600,1650, 600,1600, 600,550, 600,1600, 600,1650, 600,500, 600,1650, 600,1600, 650,500, 600,500, 600,500, 650,500, 600,500, 600,500, 600,550, 600,500, 600,1650, 600,1600, 600,1650, 600,1650, 600,1600, 600,1650, 600};  // NEC 4BB6C03F
  
  // Insert RAW IR signal for "Receiver Volume Up"
unsigned int irRECvup[] = {9050,4400, 650,500, 600,1600, 600,550, 600,500, 600,1650, 600,500, 600,1600, 650,1600, 600,1600, 600,550, 600,1600, 600,1600, 650,500, 600,1600, 650,1600, 600,500, 600,550, 600,1600, 600,550, 600,500, 600,550, 600,500, 600,550, 600,500, 600,1600, 650,500, 600,1600, 600,1650, 600,1600, 600,1650, 600,1600, 600,1600, 600};  // NEC 4BB640BF

  // Insert RAW IR signal for "Receiver Source CBL/SAT"
unsigned int irRECsrc[] = {8950,4450, 600,500, 600,1650, 600,500, 600,500, 600,1650, 600,500, 600,1600, 600,1650, 600,1600, 600,550, 600,1600, 600,1650, 600,500, 600,1600, 600,1650, 600,500, 600,500, 600,1650, 600,1600, 600,1650, 600,500, 600,500, 600,500, 650,500, 600,1600, 600,500, 600,550, 600,500, 600,1600, 600,1650, 600,1600, 600,1650, 600};  // NEC 4BB6708F

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);


void setup() {
  Serial.begin(115200);
  delay(10);

  irsend.begin();
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
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
  JsonObject JsonParser;
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
  
  Serial.println(req);
  while(client.available())
  {
    line = client.readStringUntil('\r');
    if(line.indexOf(":") == -1){
      break; // end of the header has been reached
    }
    webHeader.Add(line);
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

  JsonResult = JsonParser.Parse(body);
  Serial.println("Json Result:");
  for(int counter=0; counter<JsonResult.GetSize();counter++){
    Serial.println(JsonResult.GetKey(counter)+": "+JsonResult.GetValue(counter));
  }
  Serial.println("\r============");
//  if(req.indexOf("/irOut") !=-1){
//    irsend.sendRaw(powerIRout, 1, khz);
//  }
  // Match the request
  if (req.indexOf("/irTVpwr") != -1){
      irsend.sendRaw(irTVpwr, 68, 38);   
      Serial.println("IRreq irTVpwr sent");
  }
  else if(req.indexOf("/irCommand") !=-1){
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
  else {
    Serial.println("invalid request");
    Serial.println(req);
    // client.stop();
    // return;
  }
  
  client.flush();
   
  // Send the response to the client
  //client.print(s);
  client.print("HTTP/1.1 200 OK\r\n");
  delay(1);
  Serial.println("Client Disconnected");
  Serial.println();
  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}


