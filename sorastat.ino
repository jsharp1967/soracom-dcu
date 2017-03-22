//#include "U8glib.h"
#include "Ucglib.h" // Colour Library
#include <SPI.h>
#include <Wire.h>

// I2C Configuration
#define SLAVE_ADDRESS 0x04

// For large Jaycar 128x64 LCD Display
//U8GLIB_ST7920_128X64_4X u8g(10);
// For 1.8 inch 160x128 Colour Display
Ucglib_ST7735_18x128x160_SWSPI ucg(/*sclk=*/ 13, /*data=*/ 11, /*cd=*/ 9 , /*cs=*/ 10, /*reset=*/ 8);

// Status strings to display on screen
String XRstatus = "Offline";  // Status of the XR5000 - Online/Offline
String XRStatus2 = "";
String SATstatus = "Offline"; // Status of the satellite connection - Online/Offline
String SORAstat = "Offline";  // Status of the software on the PI - Online/Offline
String SORAstat2 = "";

String PROCState = "PASSIVE";  // Transmission state Passive/Uploading/Downloading
String RSState = "NULL";
String Splash = "SORACOM";   // Splash Screen on startup
String IPAddress = "X.X.X.X";  // IP Addresss of the PI
String dcuName = "NOT SET"; // Identify of this DCU
String heartBeat = "+";
  
String ardTemp ="XX.XX";

String MEssage ="--------------------------------";

// Button labels to display on screen
String button1 = "B-1";
String button2 = "B-2";
String button3 = "B-3";
String button4 = "B-4";

void setup(void)
{
  Serial.begin(115200);
  Serial.println("sorastat DCU booting up");

  // Screen Setup
  ucg.begin(UCG_FONT_MODE_SOLID);
  // Reset screen
  reset();
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
}
void loop(void)
{
  delay(100);
}

// callback for received data
void receiveData(int byteCount)
{
  String command;
  String cmd;
  String data;
  String status;
  int pulse = 0;
  
  while(Wire.available())
  {
    char c = Wire.read();
    command +=c;
  }
  //Serial.println(command);
  data = command.substring(2,command.length());
  status = command.substring(2,4);
  //Serial.println(status);
  cmd = command.substring(0,2);

  if (cmd == "HB")
  {
    // Heartbeat message
    if (status == "60")
      heartBeat = ". ";
    else if (status == "61")
      heartBeat = " .";
    drawHeartBeat();
  }
  if (cmd == "ID")
  {
    // Reset - data message
    dcuName = " " + data + " DCU";
    drawDCUName();
  }
  if (cmd == "RE")
  {
    // Reset
    reset();
  }
  if (cmd == "XR")
  {
    // XR Status - status message
   if (status == "On")
   {
      ucg.setColor(0, 255, 0);
      XRstatus = "XR:CONNECTED OK";
      XRStatus2 = "XR:WIFI ON    ";
   }
   if (status == "Of")
   {
      ucg.setColor(255, 0, 0);
      XRstatus = "XR:NOT CONNECTED";
      XRStatus2 ="XR:ACTIVATE WIFI";
   }
          
    drawXRStatus();
    ucg.setColor(255, 255, 255);

  }
  if (cmd == "SO")
  {
    // Soracom status - status message
    if (status == "On")
    {
      ucg.setColor(0, 255, 0);
      SORAstat = "LINK:ONLINE   ";
      SORAstat2 ="LINK:READY    ";
    }
    if (status == "Of")
    {
      ucg.setColor(255, 0, 0);
      SORAstat = "LINK:OFFLINE  ";
      SORAstat2 ="LINK:NOT READY";
    }
   
    drawSORAstatStatus();
    ucg.setColor(255, 255, 255);
  }
  
  if (cmd == "PR")
  {
    // Process activity status - data message
    if (status == "01")
      PROCState = "STATUS:READY             ";
    if (status == "02")
      PROCState = "STATUS:READING XR DATA   ";
    if (status == "03")
      PROCState = "STATUS:UPLOAD SESSIONS   ";
    if (status == "04")
      PROCState = "STATUS:DOWNLOADING       ";
    if (status == "05")
      PROCState = "STATUS:ACTIVE            ";
    if (status == "06")
      PROCState = "STATUS:COMPRESSING       ";
    if (status == "07")
      PROCState = "STATUS:UPLOAD STARTED    ";
    if (status == "08")
      PROCState = "STATUS:COMPLETED XR READ ";
    
    drawPROCState();
  }
  
  if (cmd == "RS")
  {
    if (status == "01")
      RSState = "RESULT:XR READ SUCCESS  ";
    if (status == "02")
      RSState = "RESULT:UPLOAD SUCCESS   ";
    if (status == "11")
      RSState = "RESULT:XR READ FAILED    ";
    if (status == "12")
      RSState = "RESULT:UPLOAD FAILED     "; 
      
     drawResultState();
  }
  
  if (cmd == "IP")
  {
    // IP Address - data message
    IPAddress = "IP:" + data;
    drawIP();
  }
  if (cmd == "ME")
  {
    // Message - data message
    MEssage = data;
    drawMessage();
  }
}
