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

const int BULB1=1;// Hue lights varible set
const int BULB2=2;
const int BULB3=3;
const int BULB4=4;
const int BULB5=5;
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

const int SWITCH=D6;//switches get stitches
bool switchState; 
Button mySwitch(SWITCH);

IoTTimer onTimer;
IoTTimer offTimer;
unsigned int currentTime;

//wemo
const int MYWEMO2=2;
const int MYWEMO3=3;
const int MYWEMO4=4;
const int BUTTON2=D5;
const int BUTTON3=D3;
const int BUTTON4=D7;
Button myButton2 (BUTTON2);
Button myButton3 (BUTTON3);
Button myButton4 (BUTTON4);
bool wemoState3;
bool wemoState2;
bool wemoState4;

Adafruit_SSD1306 display(OLED_RESET);//OLED display
Adafruit_SSD1306 display2(OLED_RESET);
Adafruit_BME280 bme;

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(SEMI_AUTOMATIC);


void setup() {
Serial.begin(9600);
while(!Serial);
waitFor(Serial.isConnected, 15000);
pinMode(PHOTODIODE, INPUT);

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
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
display2.begin(SSD1306_SWITCHCAPVCC, 0x3D);

}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
tempF=map(tempC,0.0,100.0,32.0,212.0);//BME maps
inHg=map(pressPa,3386.0,108364.0,1.0,32.0);

HUE6=map(POSITION, 0, 96, 1, 5);
POSITION=myEnc.read();

LIGHT=analogRead(PHOTODIODE);//light sensor

currentTime=millis();

tempC=bme.readTemperature();//BME
pressPa=bme.readPressure();
humidRH=bme.readHumidity();

//OLED
// onTimer.startTimer(5000);
display2.setTextSize(2);
display2.setTextColor(WHITE);
display2.setCursor(10,0);
display2.clearDisplay();
display2.printf("TF %0.1f\n BP %0.2f\n HM %0.2f\n LT %i\n", tempF, inHg, humidRH, LIGHT);
display2.display();

//switch seperating the automatic from the manual
if (mySwitch.isClicked()){
  switchState=!switchState;
}
//manual state
if(switchState){

if(myButton4.isClicked()){//Wemo 4
  wemoState4=!wemoState4;
}
if(wemoState4){
  wemoWrite(MYWEMO4,wemoState4);
  Serial.printf("Turning on Wemo %i\n", MYWEMO4);
}
else{
  wemoWrite(MYWEMO4,wemoState4);
  Serial.printf("Turning off Wemo %i\n", MYWEMO4);
}

if(myButton3.isClicked()){//Wemo 0
  wemoState3=!wemoState3;
}
if(wemoState3){
  wemoWrite(MYWEMO3,wemoState3);
  Serial.printf("Turning on Wemo %i\n", MYWEMO3);
}
else{
  wemoWrite(MYWEMO3,wemoState3);
  Serial.printf("Turning off Wemo %i\n", MYWEMO3);
}

if(myButton2.isClicked()){//Wemo 2
  wemoState2=!wemoState2;
}
if(wemoState2){
  wemoWrite(MYWEMO2,wemoState2);
  Serial.printf("Turning on Wemo %i\n", MYWEMO2);
}
else{
  wemoWrite(MYWEMO2,wemoState2);
  Serial.printf("Turning off Wemo %i|n", MYWEMO2);
}

if(myButton.isClicked()){//encoder button controlling hue light
   buttonState=!buttonState;
}
if(buttonState){
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 32);
  display.clearDisplay();
  display.printf("Hue light is on\n");
  display.display();
  setHue(BULB5,true,HueRed,100,255);
  setHue(BULB1,true,HueBlue,100,255);
  setHue(BULB2,true,HueOrange,100,255);
  setHue(BULB3,true,HueGreen,100,255);
  setHue(BULB4,true,HueViolet,100,255);
    //color++;
}
else{
  setHue(BULB5,false,HueRed,100,255);
  setHue(BULB1,false,HueBlue,100,255);
  setHue(BULB2,false,HueOrange,100,255);
  setHue(BULB3,false,HueGreen,100,255);
  setHue(BULB4,false,HueViolet,100,255);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,32);
  display.clearDisplay();
  display.printf("Hue light is off\n");
  display.display();
}
}

if(!switchState){
//automatic state

//hue light coming on automatically under 20 and going off over 200
if(LIGHT<20){
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,32);
  display.clearDisplay();
  display.printf("Too Dark, Lights on\n");
  display.display();
  setHue(BULB1,true,HueIndigo,255,255);
  setHue(BULB5,true,HueViolet,255,255);
  setHue(BULB2,true,HueRed,255,255);
  setHue(BULB3,true,HueGreen,255,255);
  setHue(BULB4,true,HueBlue,255,255);

if(LIGHT>200){
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,32);
  display.clearDisplay();
  display.printf("Too Bright, Lights off\n");
  display.display();
  setHue(BULB1,false,HueIndigo,100,255);
  setHue(BULB5,false,HueViolet,255,255);
  setHue(BULB2,false,HueRed,255,255);
  setHue(BULB3,false,HueGreen,255,255);
  setHue(BULB4,false,HueBlue,255,255);
}

if(humidRH<30){
  wemoWrite(MYWEMO4,HIGH);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.clearDisplay();
  display.printf("Humidity Low\n");
  display.display();
}
if(humidRH>50){
  wemoWrite(MYWEMO4,LOW);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.clearDisplay();
  display.printf("Humidity High\n");
  display.display();
}
if(tempF>75){
  wemoWrite(MYWEMO3, HIGH);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.clearDisplay();
  display.printf("Fan on\n");
  display.display();
}
if(tempF<65){
  wemoWrite(MYWEMO3, LOW);
}
}
}

