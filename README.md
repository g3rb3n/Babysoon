# Babysoon


# Components
## MCU 
- Espressif ESP WROOM 32
- Doit ESP32 devkit V1

## GPS
- uBlox NEO-6M-0-001
- GY-NE06MV2

## Accelometer / Gyroscope / Magnetometer
- MPU9255
- MPU-92.65

## OLED
- SSD1306 128x64

# Wiring
```
USB power <-> MCU microUSB

MCU 01 3V3 <-> BRD +
MCU 02 GND <-> BRD -
MCU 06 RX2 <-> GPS TX
MCU 07 TX2 <-> GPS RX
MCU 09 D18 <-> SSD SDA
MCU 10 D19 <-> SSD SCL
BRD    +   <-> GPS VCC
BRD    -   <-> GPS GND 
BRD    +   <-> SSD VDD
BRD    -   <-> SSD GND 
```