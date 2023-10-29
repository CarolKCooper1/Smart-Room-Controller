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
#include <neopixel.h>
#include "Colors.h"

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
const int BULB6=6;
int HUE6;
int color;
int myBulbs[]={1,2,3,4,5,6};
int huePosition;
// void hueFill(int start, int end, int color);

const int BUTTON=D15;// Encoder button
int COLOR;
bool buttonState;
Button myButton(BUTTON);

int POSITION;//encoder set
int prevPosition;
int encPosition;
const int MINPOS=0;
const int MAXPOS=96;
Encoder myEnc(D9,D8);

const int SWITCH=D6;//switches get stitches
bool switchState; 
Button mySwitch(SWITCH);

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
int prevWemoState2;
int prevWemoState3;
int prevWemoState4;

//neopixels
//void pixelFill(int start, int end, int color);
int i;
int j;
int pix16;
int start;
int end;  
const int PIXELCOUNT = 16;
const int LEDDELAY = 50;
void pixelFill(int start, int end, int color);

Adafruit_SSD1306 display(OLED_RESET);//OLED display
Adafruit_SSD1306 display2(OLED_RESET);
Adafruit_BME280 bme;
Adafruit_NeoPixel pixel(PIXELCOUNT, SPI1, WS2812B);

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

pixel.begin();
pixel.setBrightness(20);
pixel.show();

}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
tempF=map(tempC,0.0,100.0,32.0,212.0);//BME maps
inHg=map(pressPa,3386.0,108364.0,1.0,32.0);
//Hue light map to encoder
HUE6=map(POSITION, 0, 96, 0, 6);

POSITION=myEnc.read();

if(HUE6!=huePosition){
  huePosition = HUE6;
}
Serial.printf("hue6 position is%i\n",HUE6);

//encoder position and adding pixel lights to the encoder
if (POSITION!=prevPosition){
    prevPosition = POSITION;
}

if (POSITION<MINPOS){
    POSITION=MINPOS;
    myEnc.write(MINPOS);
}
if (POSITION > MAXPOS){
    POSITION=MAXPOS;
    myEnc.write(MAXPOS);
}
for(i=0; i<=HUE6; i++) {
    //start = i*1;
    //HUE6 = start +1;
    pixelFill(0, HUE6, rainbow[HUE6]); 
}

LIGHT=analogRead(PHOTODIODE);//light sensor

tempC=bme.readTemperature();//BME
pressPa=bme.readPressure();
humidRH=bme.readHumidity();

//OLED
display2.setTextSize(2);
display2.setTextColor(WHITE);
display2.setCursor(10,0);
display2.clearDisplay();
display2.printf("TF %0.1f\n BP %0.2f\n HM %0.2f\n LT %i\n", tempF, inHg, humidRH, LIGHT);
display2.display();

//switch seperating the automatic from the manual
if(mySwitch.isClicked()){
  switchState=!switchState;
}
//manual state
if(switchState){

if(myButton4.isClicked()){//Wemo 4 Humidifier button
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

if(myButton3.isClicked()){//Wemo 3 Fan Button
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

if(myButton2.isClicked()){//Wemo 2 Sculpture Button
  wemoState2=!wemoState2;
}
if(wemoState2){
  wemoWrite(MYWEMO2,wemoState2);
  //Serial.printf("Turning on Wemo %i\n", MYWEMO2);
}
else{
  wemoWrite(MYWEMO2,wemoState2);
  Serial.printf("Turning off Wemo %i|n", MYWEMO2);
}

if(myButton.isClicked()){//encoder button controlling hue light
   buttonState=!buttonState;
}
//encoder button turning the light on and off
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
  setHue(BULB6,true,HueIndigo,100,255);
  // hueFill(0, HUE6, HueRainbow[i]);
    // color++;
}
else{
  setHue(BULB5,false,HueRed,100,255);
  setHue(BULB1,false,HueBlue,100,255);
  setHue(BULB2,false,HueOrange,100,255);
  setHue(BULB3,false,HueGreen,100,255);
  setHue(BULB4,false,HueViolet,100,255);
  setHue(BULB6,false,HueIndigo,100,255);
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

//Auto light coming on automatically under 15
if(LIGHT<15){
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,32);
  display.clearDisplay();
  display.printf("Lights on\n");
  display.display();
  setHue(BULB1,true,HueIndigo,255,255);
  setHue(BULB5,true,HueViolet,255,255);
  setHue(BULB2,true,HueRed,255,255);
  setHue(BULB3,true,HueGreen,255,255);
  setHue(BULB4,true,HueBlue,255,255);
  setHue(BULB6,true,HueOrange,100,255);
}
//Auto light off
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
  setHue(BULB6,false,HueOrange,100,255);
}
//Auto Humidifer on
if(humidRH<30){
  wemoWrite(MYWEMO4,HIGH);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.clearDisplay();
  display.printf("Humidifier On\n");
  display.display();
}
//Humidifier off
if(humidRH>50){
  wemoWrite(MYWEMO4,LOW);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.clearDisplay();
  display.printf("Humidifier off\n");
  display.display();
}
//Automatic fan on
if(tempF>75){
  wemoWrite(MYWEMO3, HIGH);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.clearDisplay();
  display.printf("Fan on\n");
  display.display();
}
//Automatic fan off
if(tempF<72){
  wemoWrite(MYWEMO3, LOW);
}
//Automatic spotlight
if(LIGHT<5){
  wemoWrite(MYWEMO4, HIGH);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.clearDisplay();
  display.printf("Spotlight on\n");
  display.display();
}

}
}
void pixelFill(int start, int end, int color){
  int i;
  for(i=start; i<HUE6; i=i+1){
    pixel.setPixelColor(i, color);
    // delay(LEDDELAY); 
    pixel.show();
    // pixel.clear();
     
  } 
  pixel.show();
}
// void hueFill(int start, int end, int color){
//   if(myButton.isClicked){
//     buttonState!=buttonState;
//   }
//   setHue(HUE6, buttonState, HueViolet, 255, 255);
//    myBulbs[i]=HUE6;
  
// }

