/**
 * \project vl6180x-driver
 * \brief   a generic driver providing the most basic functionality 
 *          for the vl6180x proximity sensor. In a nutshell, you 
 *          implement the two functions read(...) and write(...),
 *          on the microcontroller of your choice, and the rest is 
            done for you.
 * \author Joshua Vasquez
 * \date October 8, 2014
*/

#include "vl6180x-driver.h"

struct vl6180x_SensorParams vl6180x_Params = 
{
    0x29,   // device address (cannot be changed)
    RANGE_SINGLE_SHOT,// TODO: starting mode is actually something else.
    1.0,                // default ALS_gain
    100                 // default ALS_IntegrationTime_
};


void initVL6180X()
{
    clearModes();
    setALS_Gain(1);
    setSysALS_IntegrationPeriod(100);
}


/**
 * \brief continuous modes necessitate register changes only. Single-shot
 *        mode does not.
 */
void setMode(VL6180X_MODE mode)
{
    vl6180x_Params.mode_ = mode;

    switch (mode) {
        case (RANGE_SINGLE_SHOT):
            clearModes();   // Disable interleaved and continous modes.
            /// no additional tweaks needed at this point.
        case (ALS_SINGLE_SHOT):
            clearModes();   // Disable interleaved and continous modes.
            /// no additional tweaks needed at this point.
        case (ALS_CONTINUOUS):
            clearModes();
            writeOne(I2C, vl6180x_Params.deviceAddress_, SYSALS_START, 0x03);
            break;
        case (RANGE_CONTINUOUS):
            clearModes();
            writeOne(I2C, vl6180x_Params.deviceAddress_, SYSRANGE_START, 0x03);
            break;
        case (RANGE_CONTINUOUS_AND_ALS_SINGLE_SHOT):
            clearModes();
            /// set range mode to cnts.
            writeOne(I2C, vl6180x_Params.deviceAddress_, SYSRANGE_START, 0x03);    
            /// Leave single-shot setting alone for ALS.
            break;
        case (RANGE_SINGLE_SHOT_AND_ALS_CONTINUOUS):
            clearModes();
            /// set ALS mode to cnts.
            writeOne(I2C, vl6180x_Params.deviceAddress_, SYSALS_START, 0x03);    
            /// Leave single-shot setting alone for range.
            break;
        case (INTERLEAVED):
            writeOne(I2C, vl6180x_Params.deviceAddress_, 
                     INTERLEAVED_MODE_ENABLE, 0x01);
            /// do stuff;
            break;
        // default not needed since we're using enums!
    }
}

void setALS_Gain( float gain)
{
    // TODO: FIXME
}


/// TODO: implement this function
uint16_t readALS_FromContinuous()
{
    return 0;
}

/// TODO: implement this function.
uint8_t readRangeFromContinuous()
{
    return 0;
}

float singleShotReadALS()
{
    uint8_t newData[2];
    uint16_t rawRegVal;

    setMode(ALS_SINGLE_SHOT);
    writeOne(I2C, vl6180x_Params.deviceAddress_, SYSALS_START, 0x01);
    while (!newALS_Data());
    read16(I2C, vl6180x_Params.deviceAddress_, RESULT_ALS_VAL, 2, newData);
    rawRegVal = ((uint16_t)newData[0] << 8) & newData[1];

    return ALS_LuxResolution_ * (rawRegVal / vl6180x_Params.ALS_Gain_) * 
           (100 / vl6180x_Params.ALS_IntegrationTime_);
}

/**
 * \brief return the range value in mm
 */
uint8_t singleShotReadRange()
{
    uint8_t newData;

    setMode(RANGE_SINGLE_SHOT);
    writeOne(I2C, vl6180x_Params.deviceAddress_, SYSRANGE_START, 0x01);
    while (!newRangeData());
    read16(I2C, vl6180x_Params.deviceAddress_, RESULT_RANGE_VAL, 1, &newData);
    return newData;
}

/**
 * \brief TODO: what does this actually do?
 */
void setSysALS_IntegrationPeriod(int ms)
{
    uint8_t encodedMs = ms - 1;
    write16(I2C, vl6180x_Params.deviceAddress_, SYSALS_INTEGRATION_PERIOD, 
            1, &encodedMs);
}

/**
 * \brief set the absolute light sensor's analog gain.
 */
void setSysALS_AnalogGain(float gain)
{
    uint8_t gainReg; 
    if (gain <= 1.0)
    {
        vl6180x_Params.ALS_Gain_ = 1.0;
        gainReg = 0x06;
    }
    else if (gain <= 1.25)
    {
        vl6180x_Params.ALS_Gain_ = 1.25;
        gainReg = 0x05;
    }
    else if (gain <= 1.67)
    {
        vl6180x_Params.ALS_Gain_ = 1.67;
        gainReg = 0x04;
    }
    else if (gain <= 2.5)
    {
        vl6180x_Params.ALS_Gain_ = 2.5;
        gainReg = 0x03;
    }
    else if (gain <= 5.0)
    {
        vl6180x_Params.ALS_Gain_ = 5.0;
        gainReg = 0x02;
    }
    else if (gain <= 10)
    {
        vl6180x_Params.ALS_Gain_ = 10.0;
        gainReg = 0x01;
    }
    else if (gain <= 20)
    {
        vl6180x_Params.ALS_Gain_ = 20.0;
        gainReg = 0x00;
    }
    else if (gain <= 40)
    {
        vl6180x_Params.ALS_Gain_ = 40.0;
        gainReg = 0x07;
    }
    else
    {
        vl6180x_Params.ALS_Gain_ = 1.0;
        gainReg = 0x06;/// Default gain: 1.0
    }
        write16(I2C, vl6180x_Params.deviceAddress_, SYSALS_ANALOGUE_GAIN, 
                1, &gainReg); 
}

void clearModes()
{
    writeOne(I2C, vl6180x_Params.deviceAddress_, SYSALS_START, 0x00);
    writeOne(I2C, vl6180x_Params.deviceAddress_, SYSRANGE_START, 0x00);
    /// Disable interleaved mode.
    writeOne(I2C, vl6180x_Params.deviceAddress_, INTERLEAVED_MODE_ENABLE, 0x00);  
}

uint8_t newRangeData()
{
    uint8_t readStatus;
    read16(I2C, vl6180x_Params.deviceAddress_,  RESULT_INTERRUPT_STATUS_GPIO, 
           1, &readStatus);
    // Check if range data bitfield matches "new sample" code.
    return (readStatus & NEW_SAMPLE_READY);
}


uint8_t newALS_Data()
{
    uint8_t readStatus;
    read16(I2C, vl6180x_Params.deviceAddress_, RESULT_INTERRUPT_STATUS_GPIO, 
           1, &readStatus);
    // Check if als data bitfield matches "new sample" code.   
    return ((readStatus) >> RESULT_INT_ALS_GPIO) & NEW_SAMPLE_READY;
}
