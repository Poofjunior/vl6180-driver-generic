#ifndef VL6180_DRIVER_H
#define VL6180_DRIVER_H

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

#include <stdint.h>
#include "embeddedCommon.h"  /// contains read(...) and write(...) fns.
/// Sensor Modes:
enum VL6180_MODE{ RANGE_SINGLE_SHOT, ALS_SINGLE_SHOT, ALS_CONTIUOUS,
                  RANGE_CONTINUOUS_AND_ALS_SINGLE_SHOT,
                  RANGE_SINGLE_SHOT_AND_ALS_CONTINUOUS,
                  INTERLEAVED}; 
/// Important VL6180 settings
struct vl6180-params {
    const uint8_t deviceAddress_ = 0x29; // 7-bit address in struct to prevent
                                         // name clashes for other sensors.
    VL6180_MODE mode_;
    float ALS_Gain_;
    float ALS_IntegrationTime_;
    /// other stuff here as needed.
} vl6180-params;

/// Prototypes:
void init();
void setMode(VL6180_MODE mode);
void setALS_Gain( float gain);

uint16_t readALS_FromContinuous();
uint8_t readRangeFromContinuous();

float singleShotReadALS();
uint8_t singleShotReadRange();


void setSysALS_IntegrationPeriod(int ms);

/// helper funcs:
void clearModes();  /// Clear current mode to prepare to change modes.
bool newRangeData();
bool newALS_Data();

/// Constants:
const float ALS_LuxResolution_ = 0.32;

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

