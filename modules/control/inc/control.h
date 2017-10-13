#ifndef __CONTROL_H
#define __CONTROL_H

#include "control_definitions.h"
#include "control_reference.h"
#include "computer_control.h"
#include "arming.h"

/*===========================================================================*/
/* Module global definitions.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Structure for moving control signals safely without data races.
 */
typedef struct
{
    /**
     * @brief   Current throttle value.
     */
    float throttle;
    /**
     * @brief   Current torque request.
     */
    vector3f_t torque;
    /**
     * @brief   Current motor commands.
     */
    float motor_command[8];
} control_signals_t;

/**
 * @brief   Structure for moving control signals safely without data races.
 */
typedef struct
{
    /**
     * @brief   Current requested attitude.
     */
    quaternion_t attitude;
    /**
     * @brief   Current requested rate.
     */
    vector3f_t rate;
    /**
     * @brief   Current throttle.
     */
    float throttle;
} control_reference_save_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

void ControlInit(void);
void ControlFiltersInit(void);
void vUpdateControlAction(const quaternion_t *q_m,
                          const vector3f_t *omega_m,
                          const float dt);
void vZeroControlIntegrals(void);
control_reference_t *ptrGetControlReferences(void);
control_data_t *ptrGetControlData(void);
control_limits_t *ptrGetControlLimits(void);
output_mixer_t *ptrGetOutputMixer(void);
control_filter_settings_t *ptrGetControlFilters(void);
void GetControlParameters(control_parameters_t *param);
void SetControlParameters(const control_parameters_t *param);
void GetControlSignals(control_signals_t *sig);
void GetControlReference(control_reference_save_t *ref);

#endif
