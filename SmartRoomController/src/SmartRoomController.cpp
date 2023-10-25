/* 
 * Project Smart Room Controller
 * Author: CKCooper
 * Date: 10-23-2023
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_BME280.h"
#include "Adafruit_Sensor.h"
#include "Hue.h"
#include "IoTClassroom_CNM.h"
#include <Encoder.h>

#define OLED_RESET D4
#define XPOS 0;
#define YPOS 1;

int PHOTODIODE=A1;//photodiode variable set
int LIGHT;
int DARK;
int PhotoDiode (int MEASURE);

const char deg =0xB0;//BME variable set
float tempC;
float pressPa;
float humidRH;
float tempF;
float inHg;
bool status;

const int BULB=3;// Hue lights varible set
const int BULB1=5;
int HUE6;
int color;
int myBulbs[]={1,2,3,4,5};
int i;

const int BUTTON=D15;// Encoder button
int COLOR;
bool buttonState;
Button myButton(BUTTON);

int POSITION;//encoder set
int prevPosition;
int encPosition;
const int MINPOS=0;
const int MAXPOS=96;
Encoder myEnc(D9, D8);

const int SWITCH=D6;//switch
bool switchState; 
Button mySwitch(SWITCH);

IoTTimer onTimer;
IoTTimer offTimer;
unsigned int currentTime;


Adafruit_SSD1306 display(OLED_RESET);//OLED display
Adafruit_BME280 bme;

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(SEMI_AUTOMATIC);


void setup() {
Serial.begin(9600);
while(!Serial);
pinMode(PHOTODIODE, INPUT);

// display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
//display.begin(SSD1306_SWITCHCAPVCC, 0x3D);

status = bme.begin(0x76);
    if(status==false){
        Serial.printf("BME at address 0x%02X failed to start", 0x76);
    }
WiFi.on();
WiFi.clearCredentials();
WiFi.setCredentials("IoTNetwork");
WiFi.connect();
while(WiFi.connecting()){
  Serial.printf(".");
}
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {

tempF=map(tempC,0.0,100.0,32.0,212.0);//BME maps
inHg=map(pressPa,3386.0,108364.0,1.0,32.0);

HUE6=map(POSITION, 0, 96, 1, 5);

POSITION=myEnc.read();

display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
tempC= bme.readTemperature();//BME
pressPa=bme.readPressure();
humidRH=bme.readHumidity();

//OLED
//onTimer.startTimer(5000);
display.setTextSize(2);
display.setTextColor(WHITE);
display.setCursor(10,0);
display.clearDisplay();
display.printf("TF %0.1f\n BP %0.2f\n HM %0.2f\n LT %i\n", tempF, inHg, humidRH, LIGHT);
display.display();
//onTimer.startTimer(5000);

currentTime=millis();

//switch seperating the automatic from the manual
if (mySwitch.isClicked()){
  switchState=!switchState;
}
if (switchState){
if (POSITION!=prevPosition){//bind encoder to 5 positions
  prevPosition = POSITION;
}
if (POSITION<MINPOS){
POSITION=MINPOS;
myEnc.write(MINPOS);
}
if (POSITION> MAXPOS){
  POSITION=MAXPOS;
  myEnc.write(MAXPOS);
}

if(myButton.isClicked()){//encoder button controlling hue light
   buttonState=!buttonState;
}
if(buttonState){
  display.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.clearDisplay();
  display.printf("Hue light is on\n");
  display.display();
  setHue(BULB,true,50000,100,255);
  setHue(BULB1,true,20000,100,255);
    //color++;
}
else{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  setHue(BULB,false);
  setHue(BULB1,false);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.clearDisplay();
  display.printf("Hue light is off\n");
  display.display();
  
}
}
if(!switchState){

LIGHT=analogRead(PHOTODIODE);//light sensor
Serial.printf("lightness %i\n", LIGHT);
//delay(1000);

//hue light coming on automatically under 65 and going off over 200
if(LIGHT<65){
  display.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.clearDisplay();
  display.printf("Too Dark, Lights on\n");
  display.display();
  setHue(BULB,true,50000,100,255);
  setHue(BULB1,true,50000,100,255);
}
if (LIGHT>200){
  display.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.clearDisplay();
  display.printf("Too Bright, Lights off\n");
  display.display();
  setHue(BULB, false, 50000, 100, 255);
  setHue(BULB1, false, 50000, 100, 255);
}



}
}
