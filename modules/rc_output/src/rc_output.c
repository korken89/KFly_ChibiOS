/* *
 *
 * Abstraction Layer for RC Outputs
 *
 * */

#include "ch.h"
#include "hal.h"
#include "rc_output.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief Holder for the RC output configuration structure.
 */
static const RCOutput_Configuration *rcoutput_cfg = NULL;

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief           Initializes RC outputs
 *
 * @param[in] cfg   Pointer to configuration structure
 * @return          MSG_OK if the initialization was successful
 */
msg_t RCOutputInit(const RCOutput_Configuration *cfg)
{
    msg_t status = MSG_OK;

    if (cfg->pwmcfg == NULL        ||
        cfg->pwmp_highbank == NULL ||
        cfg->pwmp_lowbank == NULL)
        osalSysHalt("RC Output configuration error.");

    /* Save the configuration structure. */
    rcoutput_cfg = cfg;

    pwmStart(rcoutput_cfg->pwmp_lowbank, rcoutput_cfg->pwmcfg);
    pwmStart(rcoutput_cfg->pwmp_highbank, rcoutput_cfg->pwmcfg);

    /* Initialize with lowest output width */
    pwmEnableChannel(rcoutput_cfg->pwmp_lowbank, 0, 0);
    pwmEnableChannel(rcoutput_cfg->pwmp_lowbank, 1, 0);
    pwmEnableChannel(rcoutput_cfg->pwmp_lowbank, 2, 0);
    pwmEnableChannel(rcoutput_cfg->pwmp_lowbank, 3, 0);

    pwmEnableChannel(rcoutput_cfg->pwmp_highbank, 0, 0);
    pwmEnableChannel(rcoutput_cfg->pwmp_highbank, 1, 0);
    pwmEnableChannel(rcoutput_cfg->pwmp_highbank, 2, 0);
    pwmEnableChannel(rcoutput_cfg->pwmp_highbank, 3, 0);

    return status;
}

/**
 * @brief   Disables all outputs, used when force disable of outputs is needed.
 */
void RCOutputDisableI(void)
{
    if (rcoutput_cfg != NULL)
    {
        pwmDisableChannelI(rcoutput_cfg->pwmp_lowbank, 0);
        pwmDisableChannelI(rcoutput_cfg->pwmp_lowbank, 1);
        pwmDisableChannelI(rcoutput_cfg->pwmp_lowbank, 2);
        pwmDisableChannelI(rcoutput_cfg->pwmp_lowbank, 3);

        pwmDisableChannelI(rcoutput_cfg->pwmp_highbank, 0);
        pwmDisableChannelI(rcoutput_cfg->pwmp_highbank, 1);
        pwmDisableChannelI(rcoutput_cfg->pwmp_highbank, 2);
        pwmDisableChannelI(rcoutput_cfg->pwmp_highbank, 3);
    }
}

/**
 * @brief               Set output channel width in microseconds.
 *
 * @param[in] sel       Channel selector.
 * @param[in] width_us  New width in microseconds.
 * @return              MSG_OK if the change was successful or MSG_RESET if the
 *                      RC outputs are not initialized.
 */
msg_t RCOutputSetChannelWidthUs(RCOutput_Channel_Selector sel,
                                pwmcnt_t width_us)
{
    static const uint32_t pwmchannellut[8] = {0, 1, 2, 3, 3, 2, 1, 0};

    if (rcoutput_cfg == NULL)
        return MSG_RESET;

    if (sel > RCOUTPUT_CHANNEL_8)
        return MSG_RESET;

    if (sel <= RCOUTPUT_CHANNEL_4)
        pwmEnableChannel(rcoutput_cfg->pwmp_lowbank,
                         pwmchannellut[sel],
                         width_us);
    else
        pwmEnableChannel(rcoutput_cfg->pwmp_highbank,
                         pwmchannellut[sel],
                         width_us);

    return MSG_OK;
}

/**
 * @brief               Set relative output channel width
 * @details             Set the width from 1 to 2 milliseconds using 0% to 100%.
 *
 * @param[in] sel       Channel selector.
 * @param[in] width     New width in 0.0 to 1.0.
 * @return              MSG_OK if the change was successful or MSG_RESET if the
 *                      RC outputs are not initialized.
 */
msg_t RCOutputSetChannelWidthRelativePositive(RCOutput_Channel_Selector sel,
                                              float width)
{
    if (rcoutput_cfg == NULL)
        return MSG_RESET;

    if (sel > RCOUTPUT_CHANNEL_8)
        return MSG_RESET;

    /* Bound the width from 0 % to 100 % */
    if (width < 0.0f)
        width = 0.0f;
    else if (width > 1.0f)
        width = 1.0f;

    /* Convert to us and send */
    return RCOutputSetChannelWidthUs(sel,
                                     (pwmcnt_t)(width * 1000.0f + 1000.0f));
}

/**
 * @brief               Set relative output channel width.
 * @details             Set the width from 1 to 2 milliseconds using -100% to
 *                      100%.
 *
 * @param[in] sel       Channel selector.
 * @param[in] width     New width in -1.0 to 1.0.
 * @return              MSG_OK if the change was successful or MSG_RESET if the
 *                      RC outputs are not initialized.
 */
msg_t RCOutputSetChannelWidthRelative(RCOutput_Channel_Selector sel,
                                      float width)
{
    if (rcoutput_cfg == NULL)
        return MSG_RESET;

    if (sel > RCOUTPUT_CHANNEL_8)
        return MSG_RESET;

    /* Bound the width from -100% to 100% */
    if (width < -1.0f)
        width = -1.0f;
    else if (width > 1.0f)
        width = 1.0f;

    /* Convert to us and send */
    return RCOutputSetChannelWidthUs(sel,
                                     (pwmcnt_t)(width * 500.0f + 1500.0f));
}

/**
 * @brief           Change the output rate of one output bank.
 *
 * @param[in] sel   Bank selector.
 * @param[in] rate  New output rate.
 * @return          MSG_OK if the change was successful or MSG_RESET if the RC
 *                  outputs are not initialized.
 */
msg_t RCOutputSetChannelPeriod(RCOutput_Bank_Selector sel,
                               RCOutput_Rate_Selector rate)
{
    if (rcoutput_cfg == NULL)
        return MSG_RESET;

    if ((sel > RCOUTPUT_BANK_5_8) ||
        (rate != RCOUTPUT_400HZ) ||
        (rate != RCOUTPUT_50HZ))
        return MSG_RESET;

    if (sel == RCOUTPUT_BANK_1_4)
        pwmChangePeriod(rcoutput_cfg->pwmp_lowbank, (pwmcnt_t)rate);
    else
        pwmChangePeriod(rcoutput_cfg->pwmp_highbank, (pwmcnt_t)rate);

    return MSG_OK;
}
