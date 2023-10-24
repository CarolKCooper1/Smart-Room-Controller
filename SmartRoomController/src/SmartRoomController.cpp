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

#define OLED_RESET D4
#define XPOS 0;
#define YPOS 1;

int PHOTODIODE=A1;//photodiode variable set
int LIGHT;
int DARK;
int PhotoDiode (int MEASURE);

const char deg =0xF8;//BME variable set
float tempC;
float pressPa;
float humidRH;
float tempF;
float inHg;
bool status;



Adafruit_SSD1306 display(OLED_RESET);//OLED display
Adafruit_BME280 bme;

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(SEMI_AUTOMATIC);


void setup() {
Serial.begin(9600);
while(!Serial);
pinMode(PHOTODIODE, INPUT);

display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

status = bme.begin(0x76);
    if(status==false){
        Serial.printf("BME at address 0x%02X failed to start", 0x76);
    }

}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
 
 LIGHT=analogRead(PHOTODIODE);
 Serial.printf("lightness %i\n", LIGHT);
 delay(50);

tempF=map(tempC,0.0,100.0,32.0,212.0);
inHg=map(pressPa,3386.0,108364.0,1.0,32.0);
tempC = bme.readTemperature();
pressPa=bme.readPressure();
humidRH=bme.readHumidity();

  delay (1000);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.clearDisplay();
  display.printf("TF %0.1f\n BP %0.2f\n HM %0.2f\n light %i\n", tempF, inHg, humidRH, LIGHT);
  display.display();
  delay(1000);

}
