#include <NMEAGPS.h>
#include <HardwareSerial.h>
#include <io/I2CBus.h>
#include <screenbuffer/ByteUpdateScreenBuffer.h>
#include <devices/SSD1306.h>
#include <Arduino.h>
#include <Wire.h>

#define D18 18
#define D19 19
//#define RX D2
//#define TX D4
//SoftwareSerial gpsPort(RX, TX);

using namespace g3rb3n;

HardwareSerial gpsPort(2);
NMEAGPS  gps; // This parses the GPS characters
gps_fix  fix; // This holds on to the latest values

SSD1306 oled(new I2CBus(0x3C, D18, D19), new ByteUpdateScreenBuffer(128, 64, 1));

void setup()
{
  Serial.begin(230400);
  Serial.println();

  oled.begin();
  oled.clear();
  oled.print("Starting");
  oled.display();
  
  gpsPort.begin(9600);


  while (!Serial);
  
  oled.clear();
  oled.setCursor(0,0);
  oled.print("Started");
  oled.setCursor(1,0);
  oled.print("CPU @ ");
  oled.print(F_CPU / 1000000);
  oled.print("MHz");
  oled.setCursor(2,0);
  oled.print("I2C @ ");
  oled.print(Wire.getClock() / 1000);
  oled.print("kHz");
  oled.display();
}


void loop()
{
  oled.setCursor(7,0);
  oled.print(millis());
  oled.display();

  if (gps.available( gpsPort )) 
  {
    fix = gps.read();

    //if (fix.valid.location && fix.valid.altitude)
    Serial.printf("Location: %9.6f %9.6f Altitude %6.2f\n", fix.latitude(), fix.longitude(), fix.altitude() );
    oled.clear();
    oled.setCursor(0,0);
    oled.print("Lat:");
    oled.print(fix.latitude(), 4);
    oled.setCursor(1,0);
    oled.print("Lon:");
    oled.print(fix.longitude(), 4);
    oled.setCursor(2,0);
    oled.print("Lon:");
    oled.print(fix.altitude(),2);
  }
}
