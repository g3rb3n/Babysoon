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

## GSM 2G
- SIM800L

## LoraWan
- SX1272 868MHz (EU)
- SX1276
- SX1301

## Accu
- LiFePO4 3000mAh

## RTC
- DS1302
- DS1307
- DS3231

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

# ToDO
- Connect GSM
- Connect SX
- Connect MPU
- Connect Accu
- Connect RTC
- BLE driver
- WOM on MPU
- Wake MCU on interrupt
- Power control of components
- Power components through transistor
- Power components through GPIO?
- Minimize hardware setup
