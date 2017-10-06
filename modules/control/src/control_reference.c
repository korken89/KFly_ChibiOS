/* *
 *
 *
 * */

#include "ch.h"
#include "hal.h"
#include "control_reference.h"
#include "arming.h"
#include "rc_input.h"

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

/**
 * @brief       Converts RC inputs to control action depending on the current
 *              flight mode.
 * @note        Only for usage in manual control mode.
 *
 * @param[out] ref          Reference output.
 * @param[in] rate_lim      Rate limits around roll (x), pitch (y) and yaw (z).
 * @param[in] attitude_lim  Attitude limits around roll (x) and pitch (y).
 */
void RCInputsToControlAction(control_reference_t *ref,
                             const vector3f_t *rate_lim,
                             const vector3f_t *attitude_lim)
{
    /* Get the zero integrals function.  */
    void vZeroControlIntegrals(void);

    /* Read out the throttle reference and check if it is bellow the minimum
     * throttle. Used to indicate an armed system by rotating the propellers. */
    float throttle = RCInputGetInputLevel(RCINPUT_ROLE_THROTTLE);

    if (throttle < fGetArmedMinThrottle())
    {
        throttle = fGetArmedMinThrottle();

        /* If the throttle is bellow the minimum armed throttle, zero integrals
         * so there is no windup problem. */
        vZeroControlIntegrals();
    }

    if (ref->mode == FLIGHTMODE_RATE)
    {
        ref->rate_reference.x =
            rate_lim->x * RCInputGetInputLevel(RCINPUT_ROLE_ROLL);
        ref->rate_reference.y =
            rate_lim->y * RCInputGetInputLevel(RCINPUT_ROLE_PITCH);
        ref->rate_reference.z =
            rate_lim->z * RCInputGetInputLevel(RCINPUT_ROLE_YAW);

        ref->actuator_desired.throttle = throttle;
    }
    else if (ref->mode == FLIGHTMODE_ATTITUDE_EULER)
    {
        ref->attitude_reference_euler.x =
            attitude_lim->x * RCInputGetInputLevel(RCINPUT_ROLE_ROLL);
        ref->attitude_reference_euler.y =
            attitude_lim->y * RCInputGetInputLevel(RCINPUT_ROLE_PITCH);
        ref->rate_reference.z =
            rate_lim->z * RCInputGetInputLevel(RCINPUT_ROLE_YAW);

        ref->actuator_desired.throttle = throttle;
    }
    else
    {
        /* The other modes (quaternion etc) are not supported here yet. */
        ref->actuator_desired.throttle = 0.0f;
    }
}

