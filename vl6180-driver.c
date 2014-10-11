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


void init()
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


/// Registers:
const uint8_t IDENTIFICATION_MODEL_ID                   = 0x000;
const uint8_t IDENTIFICATION_MODEL_REV_MAJOR            = 0x001;
const uint8_t IDENTIFICATION_MODEL_REV_MINOR            = 0x002;
const uint8_t IDENTIFICATION_MODULE_REV_MAJOR           = 0x003;
const uint8_t IDENTIFICATION_MODULE_REV_MINOR           = 0x004;
const uint8_t IDENTIFICATION_DATE_HI                    = 0x006;
const uint8_t IDENTIFICATION_DATE_LOW                   = 0x007;
const uint8_t IDENTIFICATION_TIME                       = 0x008;
const uint8_t SYSTEM_MODE_GPIO0                         = 0x010;
const uint8_t SYSTEM_MODE_GPIO1                         = 0x011;
const uint8_t SYSTEM_HISTORY_CTRL                       = 0x012;
const uint8_t SYSTEM_INTERRUPT_CONFIG_GPIO              = 0x014;
const uint8_t SYSTEM_INTERRUPT_CLEAR                    = 0x015;
const uint8_t SYSTEM_FRESH_OUT_OF_RESET                 = 0x016;
const uint8_t SYSTEM_GROUPED_PARAMETER_HOLD             = 0x017;
const uint8_t SYSRANGE_START                            = 0x018;
const uint8_t SYSRANGE_THRESH_HIGH                      = 0x019;
const uint8_t SYSRANGE_THRESH_LOW                       = 0x01A;
const uint8_t SYSRANGE_INTERMEASUREMENT_PERIOD          = 0x01B;
const uint8_t SYSRANGE_MAX_CONVERGENCE_TIME             = 0x01C;
const uint8_t SYSRANGE_CROSSTALK_COMPENSATION_RATE      = 0x01E;
const uint8_t SYSRANGE_CROSSTALK_VALID_HEIGHT           = 0x021;
const uint8_t SYSRANGE_EARLY_CONVERGENCE_ESTIMATE       = 0x022;
const uint8_t SYSRANGE_PART_TO_PART_RANGE_OFFSET        = 0x024;
const uint8_t SYSRANGE_RANGE_IGNORE_VALID_HEIGHT        = 0x025;
const uint8_t SYSRANGE_RANGE_IGNORE_THRESHOLD           = 0x026;
const uint8_t SYSRANGE_MAX_AMBIENT_LEVEL_MULT           = 0x02C;
const uint8_t SYSRANGE_RANGE_CHECK_ENABLES              = 0x02D;
const uint8_t SYSRANGE_RECALIBRATE                      = 0x02E;
const uint8_t SYSRANGE_REPEAT_RATE                      = 0x031;
const uint8_t SYSALS_START                              = 0x038;
const uint8_t SYSALS_THRESH_HIGH                        = 0x03A;
const uint8_t SYSALS_THERSH_LOW                         = 0x03C;
const uint8_t SYSALS_INTERMEASUREMENT_PERIOD            = 0x03E;
const uint8_t SYSALS_ANALOGUE_GAIN                      = 0x03F;
const uint8_t SYSALS_INTEGRATION_PERIOD                 = 0x040;
const uint8_t RESULT_RANGE_STATUS                       = 0x04D;
const uint8_t RESULT_ALS_STATUS                         = 0x04E;
const uint8_t RESULT_INTERRUPT_STATUS_GPIO              = 0x04F;
const uint8_t RESULT_ALS_VAL                            = 0x050;
const uint8_t RESULT_HISTORY_BUFFER_0                   = 0x052;
const uint8_t RESULT_HISTORY_BUFFER_1                   = 0x054;
const uint8_t RESULT_HISTORY_BUFFER_2                   = 0x056;
const uint8_t RESULT_HISTORY_BUFFER_3                   = 0x058;
const uint8_t RESULT_HISTORY_BUFFER_4                   = 0x05A;
const uint8_t RESULT_HISTORY_BUFFER_5                   = 0x05C;
const uint8_t RESULT_HISTORY_BUFFER_6                   = 0x05E;
const uint8_t RESULT_HISTORY_BUFFER_7                   = 0x060;
const uint8_t RESULT_RANGE_VAL                          = 0x062;
const uint8_t RESULT_RANGE_RAW                          = 0x064;
const uint8_t RESULT_RANGE_RETURN_RATE                  = 0x066;
const uint8_t RESULT_RANGE_REFERENCE_RATE               = 0x068;
const uint8_t RESULT_RANGE_RETURN_SIGNAL_COUNT          = 0x06C;
const uint8_t RESULT_RANGE_REFERENCE_SIGNAL_COUNT       = 0x070;
const uint8_t RESULT_RANGE_RETURN_AMB_COUNT             = 0x074;
const uint8_t RESULT_RANGE_REFERENCE_AMB_COUNT          = 0x078;
const uint8_t RESULT_RANGE_RETURN_CONV_TIME             = 0x07C;
const uint8_t RESULT_RANGE_REFERENCE_CONV_TIME          = 0x080;
const uint8_t READOUT_AVERAGING_SAMPLE_PERIOD           = 0x10A;
const uint8_t FIRMWARE_BOOTUP                           = 0x119;
const uint8_t FIRMWARE_RESULT_SCALER                    = 0x120;
const uint8_t I2C_SLAVE_DEVICE_ADDRESS                  = 0x212;
const uint8_t INTERLEAVED_MODE_ENABLE                   = 0x2A3;

#endif //VL6180_DRIVER_H

