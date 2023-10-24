/*
 * Project hue.ino
 * Description: Example code for using hue lights in IoT Classroom
 * Author: Brian Rashap
 * Date: 06-FEB-2023
 */

#include "IoTClassroom_CNM.h"
const int BULB=6; 
int color;
SYSTEM_MODE(MANUAL);
//SYSTEM_THREAD(ENABLED); Caution - will allow code to execute before fully connected

void setup() {
  Serial.begin(9600);
  waitFor(Serial.isConnected,15000);

  WiFi.on();
  WiFi.clearCredentials();
  WiFi.setCredentials("IoTNetwork");
  
  WiFi.connect();
  while(WiFi.connecting()) {
    Serial.printf(".");
  }
  Serial.printf("\n\n");
}

void loop() {
  Serial.printf("Setting color of bulb %i to color %06i\n",BULB,HueRainbow[color%7]);
  setHue(BULB,true,HueRainbow[color%7],random(32,255),255);
  color++;
  delay(5000);
}