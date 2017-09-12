#include <NMEA.h>
#include <HardwareSerial.h>
#include <io/I2CBus.h>
#include <screenbuffer/ByteUpdateScreenBuffer.h>
#include <devices/SSD1306.h>
#include <Arduino.h>
#include <Wire.h>
//#include <SimpleBLE.h>

using namespace g3rb3n;

HardwareSerial gpsPort(2);
NMEA gps(&gpsPort);

SSD1306 oled(new I2CBus(0x3C, 400000), new ByteUpdateScreenBuffer(128, 64, 1));

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

long uptime;

void displayFix(const Fix& fix)
{
  oled.setCursor(0,0);
  oled.print("Time: ");
  oled.print(fix.timeString());
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
  oled.setCursor(4,0);
  oled.print("Sat : ");
  oled.print(String(fix.satallites));
  oled.print(" ");
  oled.print("Dil : ");
  oled.print(String(fix.dilution, 1));
  oled.display();  
}

void setup()
{
  Serial.begin(230400);
  Serial.println();
  Serial.println("Babysoon");

  oled.begin();
  oled.flipVertical(true);
  oled.flipHorizontal(true);
  oled.clear();
  oled.print("Babysoon");
  oled.display();
  
  //ble.begin("Babysoon");
  
  gps.onData([](const String& data){
    Serial.println(data);
  });
  gps.onFix([](const Fix& fix){
    displayFix(fix);
    Serial.println("Fix:" + String(fix.latitude,7) + "," + String(fix.longitude,7));
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

void loop()
{
//  Serial.println(millis() / 1000 + clockOffset);
  long now = millis() / 1000;
  if (uptime != now)
  {
    uptime = now;
    oled.setCursor(LINE_UPTIME,0);
    oled.print("Uptime: ");
    oled.print(uptime);
    oled.print(" s");
    oled.display();
  }
  gps.handle();
}
