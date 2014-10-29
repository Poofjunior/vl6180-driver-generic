/// This header must be recognized by C compiler for apds-9130-driver
#ifdef __cplusplus  
extern "C" {
#endif
#ifndef EMBEDDED_COMMON_H
#define EMBEDDED_COMMON_H
/**
 * embeddedCommon.h
 * \author Joshua Vasquez
 * \date October 10, 2014
 */
#include <stdint.h>

/// Keywords
typedef enum {UART, I2C, SPI, PARALLEL, WISHBONE, CANBUS, JTAG} PERIPHERAL;

/// common functions that need to be implemented by specific embedded
/// platforms:


/**
 * \brief read a numBytes bytes back from a sensor starting at startReg.
 * \details deviceAddress depends on the PERIPHERAL. (i.e: device is the 
 *          7-bit address for i2c and the GPIO pin for SPI )
 */
void read(PERIPHERAL periph, uint8_t deviceAddress, uint8_t startReg, 
          uint8_t numBytes, uint8_t* data);
/**
 * \brief identical to read(...) function except that the address width is
 *        16-bits wide, instead of 8.
 *
 */
void read16(PERIPHERAL periph, uint8_t deviceAddress, uint16_t startReg, 
          uint8_t numBytes, uint8_t* data);


void write(PERIPHERAL periph, uint8_t deviceAddress, uint8_t startReg, 
           uint8_t numBytes, uint8_t* data);

/**
 * \brief identical to write(...) function except that the address width is
 *        16-bits wide, instead of 8.
 *
 */
void write16(PERIPHERAL periph, uint8_t deviceAddress, uint16_t startReg, 
           uint8_t numBytes, uint8_t* data);


void delayMS( uint16_t ms);

void readOne(PERIPHERAL periph, uint8_t deviceAddress, 
             uint8_t startReg, uint8_t* data);
void writeOne(PERIPHERAL periph, uint8_t deviceAddress, 
              uint8_t startReg, uint8_t data);



#endif // EMBEDDED_COMMON_H
#ifdef __cplusplus
}
#endif
