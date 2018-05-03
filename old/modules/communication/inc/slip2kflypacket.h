#ifndef __STATEMACHINE_H
#define __STATEMACHINE_H

#include "circularbuffer.h"
#include "slip.h"

/*===========================================================================*/
/* Module global definitions.                                                */
/*===========================================================================*/

#define ACK_BIT                       (0x80)
#define SERIAL_RECIEVE_BUFFER_SIZE    (256)
#define SERIAL_TRANSMIT_BUFFER_SIZE   (1024)

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Port type identifier for serial communication.
 */
typedef enum PACKED_VAR
{
    /**
     * @brief   USB identifier.
     */
    PORT_USB = 0,
    /**
     * @brief   AUX1 identifier.
     */
    PORT_AUX1 = 1,
    /**
     * @brief   AUX2 identifier.
     */
    PORT_AUX2 = 2,
    /**
     * @brief   AUX3 identifier.
     */
    PORT_AUX3 = 3,
    /**
     * @brief   AUX4 (CAN) identifier.
     */
    PORT_AUX4 = 4
} external_port_t;

/**
 * @brief   All the commands for the serial protocol.
 */
typedef enum PACKED_VAR
{
    /**
     * @brief   The zero command is not allowed.
     */
    Cmd_None                        = 0,
    /**
     * @brief   ACK command.
     */
    Cmd_ACK                         = 1,
    /**
     * @brief   Ping command.
     */
    Cmd_Ping                        = 2,
    /**
     * @brief   Send debug message command.
     */
    Cmd_DebugMessage                = 3,
    /**
     * @brief   Get running mode command.
     */
    Cmd_GetRunningMode              = 4,
    /**
     * @brief   Manage Subscriptions command.
     */
    Cmd_ManageSubscriptions         = 5,

    /*===============================================*/
    /* Info, ID and Save commands.                   */
    /*===============================================*/

    /**
     * @brief   Get system information.
     */
    Cmd_GetSystemStrings            = 16,
    /**
     * @brief   Get system information.
     */
    Cmd_GetSystemStatus             = 17,
    /**
     * @brief   Set the user definable Type/ID strings.
     */
    Cmd_SetDeviceStrings            = 18,
    /**
     * @brief   Save all settings to flash.
     */
    Cmd_SaveToFlash                 = 19,
    /**
     * @brief   Erases the settings flash.
     */
    Cmd_EraseFlash                  = 20,

    /*===============================================*/
    /* Controller specific commands.                 */
    /*===============================================*/

    /**
     * @brief   Motor override command for ESC calibration.
     */
    Cmd_MotorOverride               = 23,
    /**
     * @brief   Get controller references (quaternion and rate).
     */
    Cmd_GetControllerReferences     = 24,
    /**
     * @brief   Get control signals (actuator desired).
     */
    Cmd_GetControlSignals           = 25,
    /**
     * @brief   Get controller settings.
     */
    Cmd_GetControllerLimits         = 26,
    /**
     * @brief   Set controller settings.
     */
    Cmd_SetControllerLimits         = 27,
    /**
     * @brief   Get Arm Settings.
     */
    Cmd_GetArmSettings              = 28,
    /**
     * @brief   Set Arm Settings.
     */
    Cmd_SetArmSettings              = 29,
    /**
     * @brief   Get rate controller data.
     */
    Cmd_GetRateControllerData       = 30,
    /**
     * @brief   Set rate controller data.
     */
    Cmd_SetRateControllerData       = 31,
    /**
     * @brief   Get attitude controller data.
     */
    Cmd_GetAttitudeControllerData   = 32,
    /**
     * @brief   Set attitude controller data.
     */
    Cmd_SetAttitudeControllerData   = 33,
    /**
     * @brief   Get velocity controller data.
     */
    Cmd_GetVelocityControllerData   = 34,
    /**
     * @brief   Set velocity controller data.
     */
    Cmd_SetVelocityControllerData   = 35,
    /**
     * @brief   Get position controller data.
     */
    Cmd_GetPositionControllerData   = 36,
    /**
     * @brief   Set position controller data.
     */
    Cmd_SetPositionControllerData   = 37,
    /**
     * @brief   Get channel mixer.
     */
    Cmd_GetChannelMix               = 39,
    /**
     * @brief   Set channel mixer.
     */
    Cmd_SetChannelMix               = 40,

    /*===============================================*/
    /* RC specific commands.                         */
    /*===============================================*/

    /**
     * @brief   Get RC input settings.
     */
    Cmd_GetRCInputSettings          = 41,
    /**
     * @brief   Set RC input settings.
     */
    Cmd_SetRCInputSettings          = 42,
    /**
     * @brief   Get RC output settings.
     */
    Cmd_GetRCOutputSettings         = 43,
    /**
     * @brief   Set RC output settings.
     */
    Cmd_SetRCOutputSettings         = 44,
    /**
     * @brief   Get RC values.
     */
    Cmd_GetRCValues                 = 45,

    /*===============================================*/
    /* Sensor specific commands.                     */
    /*===============================================*/

    /**
     * @brief   Get calibrated IMU data.
     */
    Cmd_GetIMUData                  = 46,
    /**
     * @brief   Get raw IMU data.
     */
    Cmd_GetRawIMUData               = 47,
    /**
     * @brief   Get IMU calibration.
     */
    Cmd_GetIMUCalibration           = 48,
    /**
     * @brief   Set IMU calibration.
     */
    Cmd_SetIMUCalibration           = 49,

    /*===============================================*/
    /* Estimation specific commands.                 */
    /*===============================================*/

    /**
     * @brief   Get rate estimation.
     */
    Cmd_GetEstimationRate           = 50,
    /**
     * @brief   Get attitude estimation.
     */
    Cmd_GetEstimationAttitude       = 51,
    /**
     * @brief   Get velocity estimation.
     */
    Cmd_GetEstimationVelocity       = 52,
    /**
     * @brief   Get position estimation.
     */
    Cmd_GetEstimationPosition       = 53,
    /**
     * @brief   Get all states.
     */
    Cmd_GetEstimationAllStates      = 54,
    /**
     * @brief   Reset estimation.
     */
    Cmd_ResetEstimation             = 55,

    /*===============================================*/
    /* Filter specific commands.                     */
    /*===============================================*/

    /**
     * @brief   Get control filters.
     */
    Cmd_GetControlFilters           = 56,
    /**
     * @brief   Set control filters.
     */
    Cmd_SetControlFilters           = 57,

    /*===============================================*/
    /* Computer control specific commands.           */
    /*===============================================*/

    /**
     * @brief   New control reference from the PC side.
     */
    Cmd_ComputerControlReference    = 126,

    /*===============================================*/
    /* Motion Capture frame specific commands.       */
    /*===============================================*/

    /**
     * @brief   New motion capture measurement.
     */
    Cmd_MotionCaptureMeasurement    = 127

} kfly_command_t;

/**
 * @brief   The structure to keep track of transfers through the state machine.
 */
typedef struct _kfly_parser
{
    /**
     * @brief   Which port the data came from.
     */
    external_port_t port;
    /**
     * @brief   If an ACK was requested.
     */
    bool ack;
    /**
     * @brief   The location of the data.
     */
    uint8_t *buffer;
    /**
     * @brief   The length of the data.
     */
    uint8_t data_length;
    /**
     * @brief   The number of receive errors with invalid command.
     */
    uint32_t rx_cmd_error;
    /**
     * @brief   The number of receive errors with invalid size.
     */
    uint32_t rx_size_error;
    /**
     * @brief   The number of receive errors with invalid CRC.
     */
    uint32_t rx_crc_error;
    /**
     * @brief   The number of correctly received packets.
     */
    uint32_t rx_success;
    /**
     * @brief    Pointer to the parser to parse the data after a
     *           successful transfer.
     */
    void (*parser)(struct _kfly_parser *);
} kfly_parser_t;

/**
 * @brief   Function pointer definition for the Message Parser lookup table.
 */
typedef void (*kfly_data_parser_t)(struct _kfly_parser *);

/**
 * @brief   Function pointer definition for the Message Generator lookup table.
 */
typedef bool (*kfly_generator_t)(circular_buffer_t *);

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief           Checks if the input is a valid port ID.
 *
 * @param[in] port  Port to be checked.
 * @return          Returns true if it is a valid port ID.
 */
static inline bool isPort(external_port_t port)
{
    if ((port == PORT_USB) ||
        (port == PORT_AUX1) ||
        (port == PORT_AUX2) ||
        (port == PORT_AUX3) ||
        (port == PORT_AUX4))
        return true;
    else
        return false;
}

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

void InitKFlyPacketParser(kfly_parser_t *p,
                          external_port_t port,
                          uint8_t *buffer);
void ParseKFlyPacketFromSLIP(slip_parser_t *slip, kfly_parser_t *p);

#endif
