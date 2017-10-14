/* *
 *
 * All the parsers for the different packages.
 * To expand the functionality of the serial communication is just to add
 * functions here and add them to the parser_lookup table.
 *
 * */

#include "string.h"
#include "ch.h"
#include "hal.h"
#include "usb_access.h"
#include "flash_save.h"
#include "system_information.h"
#include "kflypacket_generators.h"
#include "slip2kflypacket.h"
#include "subscriptions.h"
#include "crc.h"
#include "pid.h"
#include "rc_input.h"
#include "rc_output.h"
#include "sensor_read.h"
#include "estimation.h"
#include "control.h"
#include "computer_control.h"
#include "motion_capture.h"
#include "kflypacket_parsers.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

static void ParsePing(kfly_parser_t *pHolder);
static void ParseGetRunningMode(kfly_parser_t *pHolder);
static void ParseManageSubscriptions(kfly_parser_t *pHolder);
static void ParseGetSystemStrings(kfly_parser_t *pHolder);
static void ParseGetSystemStatus(kfly_parser_t *pHolder);
static void ParseSetDeviceStrings(kfly_parser_t *pHolder);
static void ParseSaveToFlash(kfly_parser_t *pHolder);
static void ParseEraseFlash(kfly_parser_t *pHolder);
static void ParseMotorOverride(kfly_parser_t *pHolder);
static void ParseGetControllerReferences(kfly_parser_t *pHolder);
static void ParseGetControlSignals(kfly_parser_t *pHolder);
static void ParseSetControllerLimits(kfly_parser_t *pHolder);
static void ParseGetControllerLimits(kfly_parser_t *pHolder);
static void ParseSetArmSettings(kfly_parser_t *pHolder);
static void ParseGetArmSettings(kfly_parser_t *pHolder);
static void ParseGetRateControllerData(kfly_parser_t *pHolder);
static void ParseSetRateControllerData(kfly_parser_t *pHolder);
static void ParseGetAttitudeControllerData(kfly_parser_t *pHolder);
static void ParseSetAttitudeControllerData(kfly_parser_t *pHolder);
static void ParseGetVelocityControllerData(kfly_parser_t *pHolder);
static void ParseSetVelocityControllerData(kfly_parser_t *pHolder);
static void ParseGetPositionControllerData(kfly_parser_t *pHolder);
static void ParseSetPositionControllerData(kfly_parser_t *pHolder);
static void ParseGetChannelMix(kfly_parser_t *pHolder);
static void ParseSetChannelMix(kfly_parser_t *pHolder);
static void ParseGetRCInputSettings(kfly_parser_t *pHolder);
static void ParseSetRCInputSettings(kfly_parser_t *pHolder);
static void ParseGetRCOutputSettings(kfly_parser_t *pHolder);
static void ParseSetRCOutputSettings(kfly_parser_t *pHolder);
static void ParseGetRCValues(kfly_parser_t *pHolder);
static void ParseGetIMUData(kfly_parser_t *pHolder);
static void ParseGetRawIMUData(kfly_parser_t *pHolder);
static void ParseGetIMUCalibration(kfly_parser_t *pHolder);
static void ParseSetIMUCalibration(kfly_parser_t *pHolder);
static void ParseGetEstimationRate(kfly_parser_t *pHolder);
static void ParseGetEstimationAttitude(kfly_parser_t *pHolder);
static void ParseGetEstimationVelocity(kfly_parser_t *pHolder);
static void ParseGetEstimationPosition(kfly_parser_t *pHolder);
static void ParseGetEstimationAllStates(kfly_parser_t *pHolder);
static void ParseResetEstimation(kfly_parser_t *pHolder);
static void ParseGetControlFilters(kfly_parser_t *pHolder);
static void ParseSetControlFilters(kfly_parser_t *pHolder);
static void ParseComputerControlReference(kfly_parser_t *pHolder);
static void ParseMotionCaptureMeasurement(kfly_parser_t *pHolder);

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables and types.                                         */
/*===========================================================================*/



/**
 * @brief Lookup table for all the serial parsers.
 */
static const kfly_data_parser_t parser_lookup[128] = {
    NULL,                             /* 0:   Cmd_None                        */
    NULL,                             /* 1:   Cmd_ACK                         */
    ParsePing,                        /* 2:   Cmd_Ping                        */
    NULL,                             /* 3:   Cmd_DebugMessage                */
    ParseGetRunningMode,              /* 4:   Cmd_GetRunningMode              */
    ParseManageSubscriptions,         /* 5:   Cmd_ManageSubscriptions         */
    NULL,                             /* 6:                                   */
    NULL,                             /* 7:                                   */
    NULL,                             /* 8:                                   */
    NULL,                             /* 9:                                   */
    NULL,                             /* 10:                                  */
    NULL,                             /* 11:                                  */
    NULL,                             /* 12:                                  */
    NULL,                             /* 13:                                  */
    NULL,                             /* 14:                                  */
    NULL,                             /* 15:                                  */
    ParseGetSystemStrings,            /* 16:  Cmd_GetSystemStrings            */
    ParseGetSystemStatus,             /* 17:  Cmd_GetSystemStatus             */
    ParseSetDeviceStrings,            /* 18:  Cmd_SetDeviceStrings            */
    ParseSaveToFlash,                 /* 19:  Cmd_SaveToFlash                 */
    ParseEraseFlash,                  /* 20:  Cmd_EraseFlash                  */
    NULL,                             /* 21:                                  */
    NULL,                             /* 22:                                  */
    ParseMotorOverride,               /* 23:  Cmd_MotorOverride               */
    ParseGetControllerReferences,     /* 24:  Cmd_GetControllerReferences     */
    ParseGetControlSignals,           /* 25:  Cmd_GetControlSignals           */
    ParseGetControllerLimits,         /* 26:  Cmd_GetControllerLimits         */
    ParseSetControllerLimits,         /* 27:  Cmd_SetControllerLimits         */
    ParseGetArmSettings,              /* 28:  Cmd_GetArmSettings              */
    ParseSetArmSettings,              /* 29:  Cmd_SetArmSettings              */
    ParseGetRateControllerData,       /* 30:  Cmd_GetRateControllerData       */
    ParseSetRateControllerData,       /* 31:  Cmd_SetRateControllerData       */
    ParseGetAttitudeControllerData,   /* 32:  Cmd_GetAttitudeControllerData   */
    ParseSetAttitudeControllerData,   /* 33:  Cmd_SetAttitudeControllerData   */
    ParseGetVelocityControllerData,   /* 34:  Cmd_GetVelocityControllerData   */
    ParseSetVelocityControllerData,   /* 35:  Cmd_SetVelocityControllerData   */
    ParseGetPositionControllerData,   /* 36:  Cmd_GetPositionControllerData   */
    ParseSetPositionControllerData,   /* 37:  Cmd_SetPositionControllerData   */
    NULL,                             /* 38:                                  */
    ParseGetChannelMix,               /* 39:  Cmd_GetChannelMix               */
    ParseSetChannelMix,               /* 40:  Cmd_SetChannelMix               */
    ParseGetRCInputSettings,          /* 41:  Cmd_GetRCInputSettings          */
    ParseSetRCInputSettings,          /* 42:  Cmd_SetRCInputSettings          */
    ParseGetRCOutputSettings,         /* 43:  Cmd_GetRCOutputSettings         */
    ParseSetRCOutputSettings,         /* 44:  Cmd_SetRCOutputSettings         */
    ParseGetRCValues,                 /* 45:  Cmd_GetRCValues                 */
    ParseGetIMUData,                  /* 46:  Cmd_GetIMUData                  */
    ParseGetRawIMUData,               /* 47:  Cmd_GetRawIMUData               */
    ParseGetIMUCalibration,           /* 48:  Cmd_GetIMUCalibration           */
    ParseSetIMUCalibration,           /* 49:  Cmd_SetIMUCalibration           */
    ParseGetEstimationRate,           /* 50:  Cmd_GetEstimationRate           */
    ParseGetEstimationAttitude,       /* 51:  Cmd_GetEstimationAttitude       */
    ParseGetEstimationVelocity,       /* 52:  Cmd_GetEstimationVelocity       */
    ParseGetEstimationPosition,       /* 53:  Cmd_GetEstimationPosition       */
    ParseGetEstimationAllStates,      /* 54:  Cmd_GetEstimationAllStates      */
    ParseResetEstimation,             /* 55:  Cmd_ResetEstimation             */
    ParseGetControlFilters,           /* 56:  Cmd_GetControlFilters           */
    ParseSetControlFilters,           /* 57:  Cmd_SetControlFilters           */
    NULL,                             /* 58:                                  */
    NULL,                             /* 59:                                  */
    NULL,                             /* 60:                                  */
    NULL,                             /* 61:                                  */
    NULL,                             /* 62:                                  */
    NULL,                             /* 63:                                  */
    NULL,                             /* 64:                                  */
    NULL,                             /* 65:                                  */
    NULL,                             /* 66:                                  */
    NULL,                             /* 67:                                  */
    NULL,                             /* 68:                                  */
    NULL,                             /* 69:                                  */
    NULL,                             /* 70:                                  */
    NULL,                             /* 71:                                  */
    NULL,                             /* 72:                                  */
    NULL,                             /* 73:                                  */
    NULL,                             /* 74:                                  */
    NULL,                             /* 75:                                  */
    NULL,                             /* 76:                                  */
    NULL,                             /* 77:                                  */
    NULL,                             /* 78:                                  */
    NULL,                             /* 79:                                  */
    NULL,                             /* 80:                                  */
    NULL,                             /* 81:                                  */
    NULL,                             /* 82:                                  */
    NULL,                             /* 83:                                  */
    NULL,                             /* 84:                                  */
    NULL,                             /* 85:                                  */
    NULL,                             /* 86:                                  */
    NULL,                             /* 87:                                  */
    NULL,                             /* 88:                                  */
    NULL,                             /* 89:                                  */
    NULL,                             /* 90:                                  */
    NULL,                             /* 91:                                  */
    NULL,                             /* 92:                                  */
    NULL,                             /* 93:                                  */
    NULL,                             /* 94:                                  */
    NULL,                             /* 95:                                  */
    NULL,                             /* 96:                                  */
    NULL,                             /* 97:                                  */
    NULL,                             /* 98:                                  */
    NULL,                             /* 99:                                  */
    NULL,                             /* 100:                                 */
    NULL,                             /* 101:                                 */
    NULL,                             /* 102:                                 */
    NULL,                             /* 103:                                 */
    NULL,                             /* 104:                                 */
    NULL,                             /* 105:                                 */
    NULL,                             /* 106:                                 */
    NULL,                             /* 107:                                 */
    NULL,                             /* 108:                                 */
    NULL,                             /* 109:                                 */
    NULL,                             /* 110:                                 */
    NULL,                             /* 111:                                 */
    NULL,                             /* 112:                                 */
    NULL,                             /* 113:                                 */
    NULL,                             /* 114:                                 */
    NULL,                             /* 115:                                 */
    NULL,                             /* 116:                                 */
    NULL,                             /* 117:                                 */
    NULL,                             /* 118:                                 */
    NULL,                             /* 119:                                 */
    NULL,                             /* 120:                                 */
    NULL,                             /* 121:                                 */
    NULL,                             /* 122:                                 */
    NULL,                             /* 123:                                 */
    NULL,                             /* 124:                                 */
    NULL,                             /* 125:                                 */
    ParseComputerControlReference,    /* 126: Cmd_ComputerControlReference    */
    ParseMotionCaptureMeasurement     /* 127: Cmd_MotionCaptureMeasurement    */
};

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/**
 * @brief                   A generic function to save data. Locks the RTOS
 *                          while saving.
 *
 * @param[in] save_location Pointer to the location where the data shall be
 *                          saved.
 * @param[in] buffer        Pointer to the buffer where to save from.
 * @param[in] data_length   Number of bytes to save
 */
static inline void GenericSaveData(uint8_t *save_location,
                                   uint8_t *buffer,
                                   const uint32_t data_length)
{
    /* Lock while saving the data. */
    osalSysLock();

    /* Save the data. */
    memcpy(save_location, buffer, data_length);

    osalSysUnlock();
}

/**
 * @brief                   A generic function to save controller data and
 *                          control limits.
 *
 * @param[in] pi_offset     The offset to the correct set of PI controllers.
 * @param[in] limit_offset  The offset to the correct part of the limits
 *                          structure.
 * @param[in] limit_count   Number of bytes to write to the limits structure.
 * @param[in] data          Input data so save.
 */
static void ParseGenericSetControllerData(pid_data_t pids[3],
                                          uint8_t *data)
{

    /* Lock while saving the data */
    osalSysLock();

    /* Cast the control data to an array of pi_data_t
    to access each PI controller */

    /* Write only the PI coefficients */
    for (int i = 0; i < 3; i++)
    {
        memcpy((uint8_t *)&pids[i].gains,      // Settings save location
               &data[i * PID_PARAMETERS_SIZE], // Offset in the data package
               PID_PARAMETERS_SIZE);           // Settings size
    }

    osalSysUnlock();
}

/**
 * @brief               Parses a Ping command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParsePing(kfly_parser_t *pHolder)
{
    GenerateMessage(Cmd_Ping, pHolder->port);
}

/**
 * @brief               Parses a GetRunningMode command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseGetRunningMode(kfly_parser_t *pHolder)
{
    GenerateMessage(Cmd_GetRunningMode, pHolder->port);
}

/**
 * @brief               Parses a ManageSubscriptions command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseManageSubscriptions(kfly_parser_t *pHolder)
{
    vParseManageSubscription(pHolder->buffer,
                             pHolder->data_length,
                             pHolder->port);
}

/**
 * @brief               Parses a GetSystemStrings command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseGetSystemStrings(kfly_parser_t *pHolder)
{
    GenerateMessage(Cmd_GetSystemStrings, pHolder->port);
}

/**
 * @brief               Parses a GetSystemStrings command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseGetSystemStatus(kfly_parser_t *pHolder)
{
    GenerateMessage(Cmd_GetSystemStatus, pHolder->port);
}

/**
 * @brief               Parses a SetDeviceStrings command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseSetDeviceStrings(kfly_parser_t *pHolder)
{
    UNUSED(pHolder);

    typedef struct
    {
        const char vehicle_name[VEHICLE_NAME_SIZE];
        const char vehicle_type[VEHICLE_TYPE_SIZE];
    } type_name_conv_t;

    /* Check so the wasn't to big a string received */
    if (pHolder->data_length != VEHICLE_NAME_SIZE + VEHICLE_TYPE_SIZE)
        return;

    type_name_conv_t *conv;
    conv = (type_name_conv_t *)pHolder->buffer;

    SetSystemNameType(conv->vehicle_name, conv->vehicle_type);
}

/**
 * @brief               Parses a SaveToFlash command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseSaveToFlash(kfly_parser_t *pHolder)
{
    (void)pHolder;

    /* Broadcast the Save to Flash event */
    vBroadcastFlashSaveEvent();
}

/**
 * @brief               Parses a EraseFlash command.
 * @note                The serial communication will be locked until the erase
 *                      has finished.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseEraseFlash(kfly_parser_t *pHolder)
{
    (void)pHolder;

    /* Erase the external flash, the communication will be locked until it is
     * done.
     */
    vFlashSave_EraseAll();

    /* Reset the system */
    NVIC_SystemReset();

}

/**
 * @brief               Parses a MotorOverride command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseMotorOverride(kfly_parser_t *pHolder)
{
    vParseMotorOverride(pHolder->buffer, pHolder->data_length);
}

/**
 * @brief               Parses a GetControllerReferences command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseGetControllerReferences(kfly_parser_t *pHolder)
{
    GenerateMessage(Cmd_GetControllerReferences, pHolder->port);
}

/**
 * @brief               Parses a GetControlSignals command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseGetControlSignals(kfly_parser_t *pHolder)
{
    GenerateMessage(Cmd_GetControlSignals, pHolder->port);
}

/**
 * @brief               Parses a SetControllerLimits command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseSetControllerLimits(kfly_parser_t *pHolder)
{
    if (pHolder->data_length == CONTROL_LIMITS_SIZE)
    {
        /* Save the data */
        GenericSaveData((uint8_t *)ptrGetControlLimits(),
                        pHolder->buffer,
                        CONTROL_LIMITS_SIZE);
    }
}

/**
 * @brief               Parses a GetControllerLimits command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseGetControllerLimits(kfly_parser_t *pHolder)
{
    GenerateMessage(Cmd_GetControllerLimits, pHolder->port);
}

/**
 * @brief               Parses a SetArmSettings command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseSetArmSettings(kfly_parser_t *pHolder)
{
    if (pHolder->data_length == CONTROL_ARM_SIZE)
    {
        /* Save the data */
        GenericSaveData((uint8_t *)ptrGetControlArmSettings(),
                        pHolder->buffer,
                        CONTROL_ARM_SIZE);
    }
}

/**
 * @brief               Parses a GetArmSettings command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseGetArmSettings(kfly_parser_t *pHolder)
{
    GenerateMessage(Cmd_GetArmSettings, pHolder->port);
}

/**
 * @brief               Parses a GetRateControllerData command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseGetRateControllerData(kfly_parser_t *pHolder)
{
    GenerateMessage(Cmd_GetRateControllerData, pHolder->port);
}

/**
 * @brief               Parses a SetRateControllerData command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseSetRateControllerData(kfly_parser_t *pHolder)
{
    if (pHolder->data_length == (3 * PID_PARAMETERS_SIZE))
        ParseGenericSetControllerData(ptrGetControlData()->rate_controller,
                                      pHolder->buffer);
}

/**
 * @brief               Parses a GetAttitudeControllerData command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseGetAttitudeControllerData(kfly_parser_t *pHolder)
{
    GenerateMessage(Cmd_GetAttitudeControllerData, pHolder->port);
}

/**
 * @brief               Parses a SetAttitudeControllerData command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseSetAttitudeControllerData(kfly_parser_t *pHolder)
{
    if (pHolder->data_length == (3 * PID_PARAMETERS_SIZE))
        ParseGenericSetControllerData(ptrGetControlData()->attitude_controller,
                                      pHolder->buffer);
}

/**
 * @brief               Parses a GetVelocityControllerData command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseGetVelocityControllerData(kfly_parser_t *pHolder)
{
    (void) pHolder;
    //GenerateMessage(Cmd_GetVelocityControllerData, pHolder->port);
}

/**
 * @brief               Parses a SetVelocityControllerData command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseSetVelocityControllerData(kfly_parser_t *pHolder)
{
    (void) pHolder;
    //if (pHolder->data_length == (3 * PI_SETTINGS_SIZE))
    //    ParseGenericSetControllerData(VELOCITY_PI_OFFSET,
    //                                  pHolder->buffer);
}

/**
 * @brief               Parses a GetPositionControllerData command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseGetPositionControllerData(kfly_parser_t *pHolder)
{
    (void) pHolder;
    //GenerateMessage(Cmd_GetPositionControllerData, pHolder->port);
}

/**
 * @brief               Parses a SetPositionControllerData command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseSetPositionControllerData(kfly_parser_t *pHolder)
{
    (void) pHolder;
    //if (pHolder->data_length == (3 * PI_SETTINGS_SIZE))
    //    ParseGenericSetControllerData(POSITION_PI_OFFSET,
    //                                  pHolder->buffer);
}

/**
 * @brief               Parses a GetChannelMix command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseGetChannelMix(kfly_parser_t *pHolder)
{
    GenerateMessage(Cmd_GetChannelMix, pHolder->port);
}

/**
 * @brief               Parses a SetChannelMix command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseSetChannelMix(kfly_parser_t *pHolder)
{
    if (pHolder->data_length == OUTPUT_MIXER_SIZE)
    {
        /* Save the data */
        GenericSaveData((uint8_t *)ptrGetOutputMixer(),
                        pHolder->buffer,
                        OUTPUT_MIXER_SIZE);
    }
}

/**
 * @brief               Parses a GetRCInputSettings command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseGetRCInputSettings(kfly_parser_t *pHolder)
{
    GenerateMessage(Cmd_GetRCInputSettings, pHolder->port);
}

/**
 * @brief               Parses a SetRCInputSettings command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseSetRCInputSettings(kfly_parser_t *pHolder)
{
    vParseSetRCInputSettings(pHolder->buffer, pHolder->data_length);
}

/**
 * @brief               Parses a GetRCInputSettings command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseGetRCOutputSettings(kfly_parser_t *pHolder)
{
    GenerateMessage(Cmd_GetRCOutputSettings, pHolder->port);
}

/**
 * @brief               Parses a SetRCInputSettings command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseSetRCOutputSettings(kfly_parser_t *pHolder)
{
    vParseSetRCOutputSettings(pHolder->buffer, pHolder->data_length);
}

/**
 * @brief               Parses a GetRCValues command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseGetRCValues(kfly_parser_t *pHolder)
{
    GenerateMessage(Cmd_GetRCValues, pHolder->port);
}

/**
 * @brief               Parses a GetIMUData command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseGetIMUData(kfly_parser_t *pHolder)
{
    GenerateMessage(Cmd_GetIMUData, pHolder->port);
}

/**
 * @brief               Parses a GetRawIMUData command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseGetRawIMUData(kfly_parser_t *pHolder)
{
    GenerateMessage(Cmd_GetRawIMUData, pHolder->port);
}

/**
 * @brief               Parses a GetIMUCalibration command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseGetIMUCalibration(kfly_parser_t *pHolder)
{
    GenerateMessage(Cmd_GetIMUCalibration, pHolder->port);
}

/**
 * @brief               Parses a SetIMUCalibration command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseSetIMUCalibration(kfly_parser_t *pHolder)
{
    /* Temporary holder for IMU calibration data */
    static imu_calibration_t imu_calibration;

    /* Save the data into the temporary calibration structure */
    if (pHolder->data_length == SENSOR_IMU_CALIBRATION_SIZE)
    {
        /* Save the data */
        GenericSaveData((uint8_t *)&imu_calibration,
                        pHolder->buffer,
                        SENSOR_IMU_CALIBRATION_SIZE);

        /* Move the calibration data into the sensor structures */
        SetIMUCalibration(&imu_calibration);

        /* Reset estimation */
        ResetEstimation();
    }
}

/**
 * @brief               Parses a GetEstimationRate command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseGetEstimationRate(kfly_parser_t *pHolder)
{
    GenerateMessage(Cmd_GetEstimationRate, pHolder->port);
}

/**
 * @brief               Parses a GetEstimationAttitude command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseGetEstimationAttitude(kfly_parser_t *pHolder)
{
    GenerateMessage(Cmd_GetEstimationAttitude, pHolder->port);
}

/**
 * @brief               Parses a GetEstimationVeocity command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseGetEstimationVelocity(kfly_parser_t *pHolder)
{
    GenerateMessage(Cmd_GetEstimationVelocity, pHolder->port);
}

/**
 * @brief               Parses a GetEstimationPosition command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseGetEstimationPosition(kfly_parser_t *pHolder)
{
    GenerateMessage(Cmd_GetEstimationPosition, pHolder->port);
}

/**
 * @brief               Parses a GetEstimationAllStates command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseGetEstimationAllStates(kfly_parser_t *pHolder)
{
    GenerateMessage(Cmd_GetEstimationAllStates, pHolder->port);
}

/**
 * @brief               Parses a ResetEstimation command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseResetEstimation(kfly_parser_t *pHolder)
{
    (void)pHolder;
}

/**
 * @brief               Parses a GetControlFilters command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseGetControlFilters(kfly_parser_t *pHolder)
{
    GenerateMessage(Cmd_GetControlFilters, pHolder->port);
}

/**
 * @brief               Parses a SetControlFilters command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseSetControlFilters(kfly_parser_t *pHolder)
{
    /* Save the data */
    if (pHolder->data_length == CONTROL_FILTER_SETTINGS_SIZE)
    {
        osalSysLock();

        /* Save the data. */
        memcpy(ptrGetControlFilters(), pHolder->buffer,
               CONTROL_FILTER_SETTINGS_SIZE);

        /* Reinitialize the control filters */
        ControlFiltersInit();

        osalSysUnlock();
    }
}

/**
 * @brief               Parses a Computer Control command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseComputerControlReference(kfly_parser_t *pHolder)
{
    vParseComputerControlPacket(pHolder->buffer, pHolder->data_length);
}

/**
 * @brief               Parses a MotionCaptureMeasurement command.
 *
 * @param[in] pHolder   Message holder containing information
 *                      about the transmission.
 */
static void ParseMotionCaptureMeasurement(kfly_parser_t *pHolder)
{
    vParseMotionCaptureDataPackage(pHolder->buffer, pHolder->data_length);
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief               Return the parser associated to the command.
 *
 * @param[in] command   Command to get the parser for.
 * @return              Pointer to the associated parser.
 */
kfly_data_parser_t GetParser(kfly_command_t command)
{
    return parser_lookup[command];
}
