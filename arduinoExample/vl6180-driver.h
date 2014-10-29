#ifdef __cplusplus
extern "C" {
#endif
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
 * \date October 28, 2014
*/

#include <stdint.h>
#include "embeddedCommon.h"  /// contains read(...) and write(...) fns.
/// Sensor Modes:
typedef enum { RANGE_SINGLE_SHOT, ALS_SINGLE_SHOT, RANGE_CONTINUOUS, 
               ALS_CONTINUOUS, RANGE_CONTINUOUS_AND_ALS_SINGLE_SHOT,
               RANGE_SINGLE_SHOT_AND_ALS_CONTINUOUS,
               INTERLEAVED} VL6180_MODE; 
/// Important VL6180 settings
struct vl6180_SensorParams {
    /*const*/ uint8_t deviceAddress_; // 7-bit address in struct to prevent
                                         // name clashes for other sensors.
    VL6180_MODE mode_;
    float ALS_Gain_;
    float ALS_IntegrationTime_;
};


/// Prototypes:
void initVL6180();
void setMode(VL6180_MODE mode);
void setALS_Gain( float gain);

uint16_t readALS_FromContinuous();
uint8_t readRangeFromContinuous();

float singleShotReadALS();
uint8_t singleShotReadRange();


void setSysALS_IntegrationPeriod(int ms);

/// helper funcs:
void clearModes();  /// Clear current mode to prepare to change modes.
uint8_t newRangeData();
uint8_t newALS_Data();

/// Constants:
const float ALS_LuxResolution_ = 0.32;

/// Registers Addresses:
/// note: declaring these values as static const will create a new set of
///       these variables in EVERY file that #includes this header file.
/// TODO: verify that the c preprocessor only #includes this header file once.
static const uint8_t IDENTIFICATION_MODEL_ID                   = 0x000;
static const uint8_t IDENTIFICATION_MODEL_REV_MAJOR            = 0x001;
static const uint8_t IDENTIFICATION_MODEL_REV_MINOR            = 0x002;
static const uint8_t IDENTIFICATION_MODULE_REV_MAJOR           = 0x003;
static const uint8_t IDENTIFICATION_MODULE_REV_MINOR           = 0x004;
static const uint8_t IDENTIFICATION_DATE_HI                    = 0x006;
static const uint8_t IDENTIFICATION_DATE_LOW                   = 0x007;
static const uint8_t IDENTIFICATION_TIME                       = 0x008;
static const uint8_t SYSTEM_MODE_GPIO0                         = 0x010;
static const uint8_t SYSTEM_MODE_GPIO1                         = 0x011;
static const uint8_t SYSTEM_HISTORY_CTRL                       = 0x012;
static const uint8_t SYSTEM_INTERRUPT_CONFIG_GPIO              = 0x014;
static const uint8_t SYSTEM_INTERRUPT_CLEAR                    = 0x015;
static const uint8_t SYSTEM_FRESH_OUT_OF_RESET                 = 0x016;
static const uint8_t SYSTEM_GROUPED_PARAMETER_HOLD             = 0x017;
static const uint8_t SYSRANGE_START                            = 0x018;
static const uint8_t SYSRANGE_THRESH_HIGH                      = 0x019;
static const uint8_t SYSRANGE_THRESH_LOW                       = 0x01A;
static const uint8_t SYSRANGE_INTERMEASUREMENT_PERIOD          = 0x01B;
static const uint8_t SYSRANGE_MAX_CONVERGENCE_TIME             = 0x01C;
static const uint8_t SYSRANGE_CROSSTALK_COMPENSATION_RATE      = 0x01E;
static const uint8_t SYSRANGE_CROSSTALK_VALID_HEIGHT           = 0x021;
static const uint8_t SYSRANGE_EARLY_CONVERGENCE_ESTIMATE       = 0x022;
static const uint8_t SYSRANGE_PART_TO_PART_RANGE_OFFSET        = 0x024;
static const uint8_t SYSRANGE_RANGE_IGNORE_VALID_HEIGHT        = 0x025;
static const uint8_t SYSRANGE_RANGE_IGNORE_THRESHOLD           = 0x026;
static const uint8_t SYSRANGE_MAX_AMBIENT_LEVEL_MULT           = 0x02C;
static const uint8_t SYSRANGE_RANGE_CHECK_ENABLES              = 0x02D;
static const uint8_t SYSRANGE_RECALIBRATE                      = 0x02E;
static const uint8_t SYSRANGE_REPEAT_RATE                      = 0x031;
static const uint8_t SYSALS_START                              = 0x038;
static const uint8_t SYSALS_THRESH_HIGH                        = 0x03A;
static const uint8_t SYSALS_THERSH_LOW                         = 0x03C;
static const uint8_t SYSALS_INTERMEASUREMENT_PERIOD            = 0x03E;
static const uint8_t SYSALS_ANALOGUE_GAIN                      = 0x03F;
static const uint8_t SYSALS_INTEGRATION_PERIOD                 = 0x040;
static const uint8_t RESULT_RANGE_STATUS                       = 0x04D;
static const uint8_t RESULT_ALS_STATUS                         = 0x04E;
static const uint8_t RESULT_INTERRUPT_STATUS_GPIO              = 0x04F;
    /// bitfield values for RESULT_INTERRUPT_STATUS_GPIO
    static const uint8_t RESULT_INT_RANGE_GPIO                 = 0x00; //[2:0]
    static const uint8_t RESULT_INT_ALS_GPIO                   = 0x03; //[5:3]
    static const uint8_t RESULT_INT_ERROR_GPIO                 = 0x06; //[7:6]
    /// relevant codes for for RESULT_INT_ALS_GPIO and RESULT_INT_RANGE_GPIO
    #define NEW_SAMPLE_READY 4
static const uint8_t RESULT_ALS_VAL                            = 0x050;
static const uint8_t RESULT_HISTORY_BUFFER_0                   = 0x052;
static const uint8_t RESULT_HISTORY_BUFFER_1                   = 0x054;
static const uint8_t RESULT_HISTORY_BUFFER_2                   = 0x056;
static const uint8_t RESULT_HISTORY_BUFFER_3                   = 0x058;
static const uint8_t RESULT_HISTORY_BUFFER_4                   = 0x05A;
static const uint8_t RESULT_HISTORY_BUFFER_5                   = 0x05C;
static const uint8_t RESULT_HISTORY_BUFFER_6                   = 0x05E;
static const uint8_t RESULT_HISTORY_BUFFER_7                   = 0x060;
static const uint8_t RESULT_RANGE_VAL                          = 0x062;
static const uint8_t RESULT_RANGE_RAW                          = 0x064;
static const uint8_t RESULT_RANGE_RETURN_RATE                  = 0x066;
static const uint8_t RESULT_RANGE_REFERENCE_RATE               = 0x068;
static const uint8_t RESULT_RANGE_RETURN_SIGNAL_COUNT          = 0x06C;
static const uint8_t RESULT_RANGE_REFERENCE_SIGNAL_COUNT       = 0x070;
static const uint8_t RESULT_RANGE_RETURN_AMB_COUNT             = 0x074;
static const uint8_t RESULT_RANGE_REFERENCE_AMB_COUNT          = 0x078;
static const uint8_t RESULT_RANGE_RETURN_CONV_TIME             = 0x07C;
static const uint8_t RESULT_RANGE_REFERENCE_CONV_TIME          = 0x080;
static const uint16_t READOUT_AVERAGING_SAMPLE_PERIOD           = 0x10A;
static const uint16_t FIRMWARE_BOOTUP                           = 0x119;
static const uint16_t FIRMWARE_RESULT_SCALER                    = 0x120;
static const uint16_t I2C_SLAVE_DEVICE_ADDRESS                  = 0x212;
static const uint16_t INTERLEAVED_MODE_ENABLE                   = 0x2A3;
#endif //VL6180_DRIVER_H
#ifdef __cplusplus
}
#endif
