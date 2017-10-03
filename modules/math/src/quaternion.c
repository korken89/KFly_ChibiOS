/* *
 *
 *
 * */

#include "linear_algebra.h"
#include "quaternion.h"
#include "trigonometry.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/*
 * @brief               Convert Pitch, Roll and Yaw to quaternion
 *
 * @param[in] roll      Roll [rad]
 * @param[in] pitch     Pitch [rad]
 * @param[in] yaw       Yaw [rad]
 * @param[out] q        Pointer to save quaternion
 */
void euler2quat(const float roll,
                const float pitch,
                const float yaw,
                quaternion_t *q)
{
    /* Prepare the angles for conversion to quaternions */
    const float pitch_h = pitch * 0.5f;
    const float roll_h  = roll * 0.5f;
    const float yaw_h   = yaw * 0.5f;

    /* Calculate sines and cosines */
    const float cp = cosf(pitch_h);
    const float sp = sinf(pitch_h);

    const float cr = cosf(roll_h);
    const float sr = sinf(roll_h);

    const float cy = cosf(yaw_h);
    const float sy = sinf(yaw_h);

    /* Create the quaternion */
    q->w = cr * cp * cy + sr * sp * sy;
    q->x = sr * cp * cy - cr * sp * sy;
    q->y = cr * sp * cy + sr * cp * sy;
    q->z = cr * cp * sy - sr * sp * cy;
}
