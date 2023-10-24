#ifndef _WEMO_H_
#define _WEMO_H_

/*
 *  Project: Wemo IoT Library
 *  Description: Library for controlling Wemo Smart Outlets at IOT Classroom
 *  Authors:  Brian Rashap
 *  Date:     06-FEB-2022
 */

#include "application.h"

TCPClient WemoClient;

int wemoPort = 49153;
const char *wemoIP[6] = {"192.168.1.30","192.168.1.31","192.168.1.32","192.168.1.33","192.168.1.34","192.168.1.35"};

// Function Prototypes
void switchON(int wemo);
void switchOFF(int wemo);
void wemoWrite(int outlet, bool wemoState);

// Turn on/off wemo outlets similar to digitalWrite
void wemoWrite(int outlet, bool wemoState) {
  if(wemoState) {
    switchON(outlet);
  }
  else {
    switchOFF(outlet);
  }
}



// turn on specified wemo outlet
void switchON(int wemo) {
  
  String data1;
  
  Serial.printf("Switching On Wemo #%i",wemo);
  data1+="<?xml version=\"1.0\" encoding=\"utf-8\"?><s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Body><u:SetBinaryState xmlns:u=\"urn:Belkin:service:basicevent:1\"><BinaryState>1</BinaryState></u:SetBinaryState></s:Body></s:Envelope>"; // Use HTML encoding for comma's
  if (WemoClient.connect(wemoIP[wemo],wemoPort)) {
        WemoClient.println("POST /upnp/control/basicevent1 HTTP/1.1");
        WemoClient.println("Content-Type: text/xml; charset=utf-8");
        WemoClient.println("SOAPACTION: \"urn:Belkin:service:basicevent:1#SetBinaryState\"");
        WemoClient.println("Connection: keep-alive");
        WemoClient.print("Content-Length: ");
        WemoClient.println(data1.length());
        WemoClient.println();
        WemoClient.print(data1);
        WemoClient.println();
    }

  if (WemoClient.connected()) {
     WemoClient.stop();
  }
}

// turn off wemo outlet specified
void switchOFF(int wemo){
  String data1;
  
  Serial.printf("Switching Off Wemo #%i \n",wemo);
  data1+="<?xml version=\"1.0\" encoding=\"utf-8\"?><s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Body><u:SetBinaryState xmlns:u=\"urn:Belkin:service:basicevent:1\"><BinaryState>0</BinaryState></u:SetBinaryState></s:Body></s:Envelope>"; // Use HTML encoding for comma's
  if (WemoClient.connect(wemoIP[wemo],wemoPort)) {
        WemoClient.println("POST /upnp/control/basicevent1 HTTP/1.1");
        WemoClient.println("Content-Type: text/xml; charset=utf-8");
        WemoClient.println("SOAPACTION: \"urn:Belkin:service:basicevent:1#SetBinaryState\"");
        WemoClient.println("Connection: keep-alive");
        WemoClient.print("Content-Length: ");
        WemoClient.println(data1.length());
        WemoClient.println();
        WemoClient.print(data1);
        WemoClient.println();
    }
   
  if (WemoClient.connected()) {
     WemoClient.stop();
  }
}

#endif // _WEMO_H_