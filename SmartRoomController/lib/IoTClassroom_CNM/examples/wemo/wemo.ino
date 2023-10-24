/*
 * Project wemo.ino
 * Description: Example code for using wemo outlets in IoT Classroom
 * Author: Brian Rashap
 * Date: 06-FEB-2023
 */

#include "IoTClassroom_CNM.h"
const int MYWEMO=0;
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
  delay(1000);
  Serial.printf("\n\n");
}

void loop(){
    Serial.printf("Turning on Wemo# %i\n",MYWEMO);
    wemoWrite(MYWEMO,HIGH);
    delay(5000);
    Serial.printf("Turning off Wemo# %i\n",MYWEMO);
    wemoWrite(MYWEMO,LOW);
    delay(5000);
}