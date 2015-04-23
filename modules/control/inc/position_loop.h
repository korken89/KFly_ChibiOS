#ifndef __POSITION_LOOP_H
#define __POSITION_LOOP_H

/*===========================================================================*/
/* Module global definitions.                                                */
/*===========================================================================*/
#define POSITION_CONTROL_MESSAGE_SIZE   sizeof(position_control_package_t)

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/
typedef struct {
    uint16_t throttle;
    int16_t roll;
    int16_t pitch;
    int16_t yaw;
} position_control_package_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/
float fGetComputerControlThrottle(void);
float fGetComputerControlRoll(void);
float fGetComputerControlPitch(void);
float fGetComputerControlYaw(void);
void vParseComputerControlPackage(uint8_t *payload, uint8_t size);

#endif
