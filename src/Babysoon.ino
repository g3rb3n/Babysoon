#include <NMEA.h>
#include <HardwareSerial.h>
#include <io/I2CBus.h>
#include <screenbuffer/ByteUpdateScreenBuffer.h>
#include <devices/SSD1306.h>
#include <Arduino.h>
#include <Wire.h>
#include "MPU9250.h"
#include "esp_deep_sleep.h"
//#include <SimpleBLE.h>

using namespace g3rb3n;

HardwareSerial gpsPort(2);
NMEA gps(&gpsPort);

MPU9250 mpu(0x68, 21, 22);

SSD1306 oled(new I2CBus(0x3C, 400000), new ByteUpdateScreenBuffer(128, 64, 1));

#define GPIO_INPUT_IO_TRIGGER 2
//SimpleBLE ble;

#define LINE_NOW 0
#define LINE_FIX 1
#define LINE_LAT 2
#define LINE_LON 3
#define LINE_ALT 4
#define LINE_RT  5
#define LINE_UPTIME 5
#define LINE_CPU 6
#define LINE_I2C 7

#define powerGPSPin 19
#define mpuIntPin 2
#define WOM_THRESHOLD 10

gpio_num_t wakeupPin = (gpio_num_t)mpuIntPin;

long uptime;
long nextFix = 0;


void setup()
{
  Serial.begin(230400);
  Serial.println();
  Serial.println("Babysoon");
  
  esp_deep_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_AUTO);
  //gpio_pulldown_dis_en(GPIO_INPUT_IO_TRIGGER);
  //gpio_pullup_dis(GPIO_INPUT_IO_TRIGGER);
  pinMode(GPIO_INPUT_IO_TRIGGER, INPUT_PULLDOWN);
  esp_deep_sleep_enable_ext0_wakeup(wakeupPin, 1); 
  
  while(!mpu.connected())
  {
    Serial.print("Waiting for MPU9250 ");
    Serial.print("MPU9250 responded with : ");
    Serial.print(mpu.identification());
    Serial.println();
  }
  mpu.wakeOnMotion(WOM_THRESHOLD);
  
  pinMode(mpuIntPin, INPUT);
  
  pinMode(powerGPSPin, OUTPUT);
  
  oled.begin();
  oled.flipVertical(true);
  oled.flipHorizontal(true);
  oled.clear();
  oled.print("Babysoon");
  oled.display();
  
  //ble.begin("Babysoon");
  
  gps.onData([](const String& data){
    //Serial.println(data);
  });
  gps.onFix([](const Fix& fix){
    displayFix(fix);
    Serial.print("Fix:");
    Serial.print(fix.latitude,7);
    Serial.print(" , ");
    Serial.print(fix.longitude,7);
    Serial.print(" , ");
    Serial.print(fix.quality);
    Serial.print(" , ");
    Serial.print(fix.satallites);
    Serial.println();
    if(fix.quality > 0)
    {
      nextFix = millis() + 10000;
      powerGPS(false);
      deepSleep();
    }
  });
  gpsPort.begin(9600);

  oled.clear();
  oled.setCursor(LINE_CPU,0);
  oled.print("CPU @ ");
  oled.print(F_CPU / 1000000);
  oled.print("MHz");
  oled.setCursor(LINE_I2C,0);
  oled.print("I2C @ ");
  oled.print(Wire.getClock() / 1000);
  oled.print("kHz");
}

void displayFix(const Fix& fix)
{
  oled.setCursor(0,0);
  oled.print("Time: ");
  oled.print(fix.timeString());
  if(fix.quality > 0)
  {
    oled.setCursor(1,0);
    oled.print("Lat : ");
    oled.print(String(fix.latitude, 7));
    oled.setCursor(2,0);
    oled.print("Long: ");
    oled.print(String(fix.longitude, 7));
    oled.setCursor(3,0);
    oled.print("Alt : ");
    oled.print(String(fix.altitude, 1));
    oled.print("m ");
    oled.print(String(fix.height, 1));
    oled.print("m  ");
  }
  oled.setCursor(4,0);
  oled.print("Sat : ");
  oled.print(String(fix.satallites));
  oled.print(" ");
  oled.print("Dil : ");
  oled.print(String(fix.dilution, 1));
  oled.display();  
}

void deepSleep()
{
  oled.setCursor(7, 0);
  oled.print("MCU Deep sleep");
  oled.display();

  Serial.println("MCU Deep sleep");
  esp_deep_sleep_start();  
}


bool gpsOn = false;
void loop()
{
//  Serial.println(millis());
  long now = millis() / 1000;
  if (uptime != now)
  {
    uptime = now;
    oled.setCursor(LINE_UPTIME,0);
    oled.print("Uptime: ");
    oled.print(uptime);
    oled.print(" s");
    oled.display();
    //Serial.println(uptime);
    //Serial.println(digitalRead(mpuIntPin));
  }
    
  if (nextFix < millis() && !gpsOn)
  {
    powerGPS(true);
  }
  gps.handle();
}

void powerGPS(bool on)
{
  oled.setCursor(6, 0);
  if (on)
    oled.println("GPS on");
  else
    oled.println("GPS off");
  oled.display();

  if (on)
    Serial.println("Power GPS on");
  else
    Serial.println("Power GPS off");

  gpsOn = on;
  digitalWrite(powerGPSPin, on);
}
