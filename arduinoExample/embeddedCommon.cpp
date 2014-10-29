/*
 * embeddedCommon.cpp
 * Arduino Implementation
 */
#include "embeddedCommon.h"
#include <Arduino.h>
#include <Wire.h>

void delayMS(uint16_t ms)
{
    return delay(ms);
}

void read(PERIPHERAL periph, uint8_t deviceAddress, uint8_t startReg, 
          uint8_t numBytes, uint8_t* data)
{
    switch (periph) {
        case UART:
        case I2C:
            Wire.beginTransmission(deviceAddress);
            Wire.write(startReg);
            Wire.endTransmission();
            Wire.requestFrom(deviceAddress, numBytes);
            for (uint8_t i = 0; i < numBytes; ++i)
            {
                while(!Wire.available());   // wait for next byte
                data[i] = Wire.read();
            }
            break;
        case SPI:
        case PARALLEL:
        case WISHBONE:
        case CANBUS:
        case JTAG:
            return;
    }

}

void write(PERIPHERAL periph, uint8_t deviceAddress, uint8_t startReg, 
           uint8_t numBytes, uint8_t* data)
{
    switch (periph) {
        case UART:
        case I2C:
            Wire.beginTransmission(deviceAddress);
            Wire.write(startReg);
            for (uint8_t i = 0; i < numBytes; ++i)
                Wire.write(data[i]);
            Wire.endTransmission();
            break;
        case SPI:
        case PARALLEL:
        case WISHBONE:
        case CANBUS:
        case JTAG:
            return;
    }

}

void readOne(PERIPHERAL periph, uint8_t deviceAddress,                          
             uint8_t startReg, uint8_t* data)                                   
{                                                                               
    return read(periph, deviceAddress, startReg, 1, data);                      
}                                                                               

void writeOne(PERIPHERAL periph, uint8_t deviceAddress,                         
              uint8_t startReg, uint8_t data)                                   
{                                                                               
    return write(periph, deviceAddress, startReg, 1, &data);                    
}                                                                               
