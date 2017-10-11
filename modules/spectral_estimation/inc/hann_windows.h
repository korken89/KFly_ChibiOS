#ifndef __HANN_WINDOWS_H
#define __HANN_WINDOWS_H

#include "ch.h"
#include "hal.h"
#include "kfly_defs.h"

/*===========================================================================*/
/* Module global definitions.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if SPECTRAL_FFT_SIZE == 32
extern const float hann_window[32];
#elif SPECTRAL_FFT_SIZE == 64
extern const float hann_window[64];
#elif SPECTRAL_FFT_SIZE == 128
extern const float hann_window[128];
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif
