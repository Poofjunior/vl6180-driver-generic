/**
 * \project vl6180-driver
 * \brief   a generic driver providing the most basic functionality 
 *          for the vl6180 proximity sensor. In a nutshell, you 
 *          implement the two functions read(...) and write(...),
 *          on the microcontroller of your choice, and the rest is 
            done for you.
 * \author Joshua Vasquez
 * \date October 8, 2014
*/

#include "vl6180-driver.h"


void initVL6180()
{
    clearModes();
    setALS_Gain(1);
    setSysALS_IntegrationPeriod(100);
}


/**
 * \brief continuous modes necessitate register changes only. Single-shot
 *        mode does not.
 */
void setMode(VL6180_MODE mode)
{
    vl6180-params.mode_ = mode;
    uint8_t readStatus = 0;

    switch (mode) {
        case (RANGE_SINGLE_SHOT):
            clearModes();   // Disable interleaved and continous modes.
            /// no additional tweaks needed at this point.
        case (ALS_SINGLE_SHOT):
            clearModes();   // Disable interleaved and continous modes.
            /// no additional tweaks needed at this point.
        case (ALS_CONTINUOUS):
            clearModes();
            writeOne(I2C, SYSALS_START, 0x03);
            break;
        case (RANGE_CONTINUOUS):
            clearModes();
            writeOne(I2C, SYSRANGE_START, 0x03);
            break;
        case (RANGE_CONTINUOUS_AND_ALS_SINGLE_SHOT):
            clearModes();
            writeOne(I2C, SYSRANGE_START, 0x03);    /// set range mode to cnts.
            /// Leave single-shot setting alone for ALS.
            break;
        case (RANGE_SINGLE_SHOT_AND_ALS_CONTINUOUS):
            clearModes();
            writeOne(I2C, SYSALS_START, 0x03);    /// set ALS mode to cnts.
            /// Leave single-shot setting alone for range.
            break;
        case (INTERLEAVED):
            writeOne(I2C, INTERLEAVEd_MODE_ENABLE, 0x01);    
            /// do stuff;
            break;
        // default not needed since we're using enums!
    }
}

void setALS_Gain( float gain)
{
    writeOne(...);  // TODO: FIXME
}


uint16_t readALS_FromContinuous()
{

}

uint8_t readRangeFromContinuous()
{

}

float singleShotReadALS()
{
    uint16_t newData[2];
    uint16_t rawRegVal;

    setMode(ALS_SINGLE_SHOT);
    writeOne(I2C, SYSALS_START, 0x01);
    while (!newALS_Data());
    read(I2C, RESULT_ALS_VAL, newData);
    rawRegVal = ((uint16_t)newData[0] << 8) & newData[1];

    return ALS_LuxResolution_ * (rawRegVal / ALS_Gain_) * 
           (100 / ALS_IntegrationTime_);
}

/**
 * \brief return the range value in mm
 */
uint8_t singleShotReadRange()
{
    uint8_t newData;

    setMode(RANGE_SINGLE_SHOT);
    writeOne(I2C, SYSRANGE_START, 0x01);
    while (!newRangeData);
    readOne(I2C, RESULT_RANGE_VAL, &newData);
    return newData;
}

void setSysALS_IntegrationPeriod(int ms)
{
    uint8_t encodedMs = ms - 1;
    writeOne(I2C, SYSALS_INTEGRATION_PERIOD, 1, encodedMs);
}

void setSysALS_AnalogGain(float gain)
{
    if (gain <= 1.0)
    {
        vl6180-params.ALS_Gain_ = 1.0;
        writeOne(I2C, SYSALS_INTEGRATION_PERIOD, 1, 6); /// Default gain: 1.0
    }
    else if (gain <= 1.25)
    {
        vl6180-params.ALS_Gain_ = 1.25;
        writeOne(I2C, SYSALS_INTEGRATION_PERIOD, 1, 5); 
    }
    else if (gain <= 1.67)
    {
        vl6180-params.ALS_Gain_ = 1.67;
        writeOne(I2C, SYSALS_INTEGRATION_PERIOD, 1, 4); 
    }
    else if (gain <= 2.5)
    {
        vl6180-params.ALS_Gain_ = 2.5;
        writeOne(I2C, SYSALS_INTEGRATION_PERIOD, 1, 3); 
    }
    else if (gain <= 5.0)
    {
        vl6180-params.ALS_Gain_ = 5.0;
        writeOne(I2C, SYSALS_INTEGRATION_PERIOD, 1, 2); 
    }
    else if (gain <= 10)
    {
        vl6180-params.ALS_Gain_ = 10.0;
        writeOne(I2C, SYSALS_INTEGRATION_PERIOD, 1, 1); 
    }
    else if (gain <= 20)
    {
        vl6180-params.ALS_Gain_ = 20.0;
        writeOne(I2C, SYSALS_INTEGRATION_PERIOD, 1, 0); 
    }
    else if (gain <= 40)
    {
        vl6180-params.ALS_Gain_ = 40.0;
        writeOne(I2C, SYSALS_INTEGRATION_PERIOD, 1, 7); 
    }
    else
    {
        vl6180-params.ALS_Gain_ = 1.0;
        writeOne(I2C, SYSALS_INTEGRATION_PERIOD, 1, 6); /// Default gain: 1.0
    }
}

bool clearModes()
{
    writeOne(I2C, SYSALS_START, 0x00);
    writeOne(I2C, SYSRANGE_START, 0x00);
    writeOne(I2C, INTERLEAVED_MODE_ENABLE, 0);  /// Disable interleaved mode.
}

bool newRangeData()
{
    uint8_t readStatus;
    readOne(I2C, RESULT_INTERRUPT_STATUS, &readStatus);
    
    readStatus &= 0x07;         // mask off upper bits [7:3].
    return (readStatus == 4);   // 4 indicates new sample is ready.
}


bool newALS_Data()
{
    uint8_t readStatus;
    readOne(I2C, RESULT_INTERRUPT_STATUS, &readStatus);
    
    readStatus = (0xC0 & readStatus) >> 3;  // mask off bits [7:6] and [2:0].
    return (readStatus == 4);
}
