/* *
 *
 * Serial Line Internet Protocol support
 *
 * */

#include "ch.h"
#include "hal.h"
#include "crc.h"
#include "circularbuffer.h"
#include "slip.h"

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


/*===============================================================*/
/* Expansion of circular buffers required by the serial protocol */
/*===============================================================*/

/**
 * @brief               Writes an SLIP_END to the buffer without encodeing it.
 *
 * @param[in/out] cb    Pointer to the circular buffer.
 * @param[in/out] count Pointer to tracking variable for the buffer.
 */
static void CircularBuffer_SLIPWriteENDNoInc(circular_buffer_t *cb,
                                             int32_t *count)
{
    /* Check if we have an error from previous write */
    if (*count >= 0)
    {
        /* Check if we have 2 bytes free, in case of data = reserved */
        if ((CircularBuffer_SpaceLeft(cb) - *count) >= 1)
        {
            cb->buffer[(cb->head + *count) % cb->size] = SLIP_END;
            *count += 1;
        }
        else
            *count = -1;
    }
}

/**
 * @brief               Writes a chunk of data to the circular buffer and
 *                      encodes it with the SLIP protocol on the fly.
 *
 * @param[out]    cb    Pointer to the circular buffer.
 * @param[in]     data  Pointer to the data to be written.
 * @param[in]     size  Size of the data.
 * @param[in/out] count Pointer to tracking variable for the buffer.
 */
static void CircularBuffer_SLIPWriteChunkNoInc(circular_buffer_t *cb,
                                               uint8_t *data,
                                               const uint32_t size,
                                               int32_t *count)
{
    uint32_t i, p_sp;
    const uint32_t space = CircularBuffer_SpaceLeft(cb);

    /* Check if we have an error from previous write */
    if (*count >= 0)
    {
        /* Check if we have 2 times the data packet size free, then we can
         * encode the entire buffer directly without worries. */
        if ((space - *count + 1) / 2 >= size)
        {
            for (i = 0; i < size; i++)
            {
                if ((data[i] != SLIP_END) && (data[i] != SLIP_ESC))
                {
                    cb->buffer[(cb->head + *count) % cb->size] = data[i];
                    *count += 1;
                }
                else if (data[i] == SLIP_END)
                {
                    cb->buffer[(cb->head + *count) % cb->size] = SLIP_ESC;
                    *count += 1;
                    cb->buffer[(cb->head + *count) % cb->size] = SLIP_ESC_END;
                    *count += 1;
                }
                else
                {
                    cb->buffer[(cb->head + *count) % cb->size] = SLIP_ESC;
                    *count += 1;
                    cb->buffer[(cb->head + *count) % cb->size] = SLIP_ESC_ESC;
                    *count += 1;
                }
            }
        }

        /* Worst case buffer will not fit, start writing and hope there are not
         * many special cases as the "no special case" is guaranteed to fit. */
        else
        {
            /* Keep the pointer count. */
            p_sp = 0;
            i = 0;

            /* Loop while there is space left and there is data left. */
            while((p_sp < size) && (i < space))
            {
                if ((data[p_sp] != SLIP_END) && (data[p_sp] != SLIP_ESC))
                {
                    cb->buffer[(cb->head + *count) % cb->size] = data[p_sp];
                    *count += 1;
                }
                else if (data[p_sp] == SLIP_END)
                {
                    cb->buffer[(cb->head + *count) % cb->size] = SLIP_ESC;
                    *count += 1;
                    cb->buffer[(cb->head + *count) % cb->size] = SLIP_ESC_END;
                    *count += 1;
                    i++;
                }
                else
                {
                    cb->buffer[(cb->head + *count) % cb->size] = SLIP_ESC;
                    *count += 1;
                    cb->buffer[(cb->head + *count) % cb->size] = SLIP_ESC_ESC;
                    *count += 1;
                    i++;
                }

                /* Increment the buffer index. */
                p_sp++;
            }

            /* Check if the operation failed. */
            if (p_sp < size)
            {
                *count = -1;
            }
        }
    }
}

/*================================*/
/* Generate message functionality */
/*================================*/

/**
 * @brief               Encodes a chunk of data with the full SLIP protocol.
 *
 * @param[in]     data  Pointer to the data to be encoded.
 * @param[in]     size  Size of the data.
 * @param[out]    cb    Pointer to the circular buffer where the data will be
 *                      stored.
 */
bool GenerateSLIP(uint8_t *data,
                  const uint32_t size,
                  circular_buffer_t *cb)
{
    int32_t count = 0;

    /* Check if the "best case" will fit. */
    if (CircularBuffer_SpaceLeft(cb) < (size + 2))
        return HAL_FAILED;

    /* Add start byte. */
    CircularBuffer_SLIPWriteENDNoInc(cb, &count);

    /* Encode and send the data. */
    CircularBuffer_SLIPWriteChunkNoInc(cb, data, size, &count);

    /* Add stop byte. */
    CircularBuffer_SLIPWriteENDNoInc(cb, &count);

    /* Increment the buffer pointer */
    return CircularBuffer_Increment(cb, count);
}

/**
 * @brief               Encodes multiple chunks of data with the full SLIP
 *                      protocol. Assumes a header, body and tail from of the
 *                      data.
 *
 * @param[in]   head    Pointer to the data to be encoded.
 * @param[in]   h_size  Size of the data.
 * @param[in]   body    Pointer to the data to be encoded.
 * @param[in]   b_size  Size of the data.
 * @param[in]   tail    Pointer to the data to be encoded.
 * @param[in]   t_size  Size of the data.
 * @param[out]  cb      Pointer to the circular buffer where the data will be
 *                      stored.
 */
bool GenerateSLIP_HBT(uint8_t *head,
                      const uint32_t h_size,
                      uint8_t *body,
                      const uint32_t b_size,
                      uint8_t *tail,
                      const uint32_t t_size,
                      circular_buffer_t *cb)
{
    int32_t count = 0;

    /* Check if the "best case" will fit. */
    if (CircularBuffer_SpaceLeft(cb) < (h_size + b_size + t_size + 2))
        return HAL_FAILED;

    /* Add start byte. */
    CircularBuffer_SLIPWriteENDNoInc(cb, &count);

    /* Encode and send the data. */
    if (head != NULL)
        CircularBuffer_SLIPWriteChunkNoInc(cb, head, h_size, &count);

    if (body != NULL)
        CircularBuffer_SLIPWriteChunkNoInc(cb, body, b_size, &count);

    if (tail != NULL)
        CircularBuffer_SLIPWriteChunkNoInc(cb, tail, t_size, &count);

    /* Add stop byte. */
    CircularBuffer_SLIPWriteENDNoInc(cb, &count);

    /* Increment the buffer pointer */
    return CircularBuffer_Increment(cb, count);
}

/*============================*/
/* SLIP parsing functionality */
/*============================*/

/**
 * @brief                       Initializes the slip_parser_holder_t structure.
 *
 * @param[in/out] pHolder       Pointer to slip_parser_holder_t structure.
 * @param[in]     buffer        Buffer used for intermediate data.
 * @param[in]     buffer_size   Buffer size.
 * @param[in]     parser        Pointer to the parser of the passed data.
 */
void InitSLIPParser(slip_parser_holder_t *pHolder,
                    uint8_t *buffer,
                    const uint16_t buffer_size,
                    void (*parser)(slip_parser_holder_t *))
{
    pHolder->buffer = buffer;
    pHolder->buffer_size = buffer_size;
    pHolder->buffer_count = 0;
    pHolder->buffer_overrun = 0;
    pHolder->rx_error = 0;
    pHolder->rx_success = 0;
    pHolder->state = SLIP_STATE_AWAITING_START;
    pHolder->parser = parser;
}

/**
 * @brief              The entry point of serial data to the statemachine.
 *                     When a finished SLIP packet is detected the parser
 *                     function will be called to handle the next level of
 *                     parsing.
 *
 * @param[in] data     Input data to be parsed.
 * @param[in] pHolder  Pointer to slip_parser_holder_t structure.
 */
void ParseSLIP(uint8_t data, slip_parser_holder_t *pHolder)
{

    /* Check for buffer overrun. */
    if (pHolder->buffer_count >= pHolder->buffer_size)
    {
        pHolder->state = SLIP_STATE_AWAITING_START;
        pHolder->rx_error++;
        pHolder->buffer_count = 0;
    }

    switch (pHolder->state)
    {
    case SLIP_STATE_AWAITING_START:

        if (data == SLIP_END)
        {
            /* Start of data detected, change state and clear buffer. */
            pHolder->state = SLIP_STATE_RECEIVING;
            pHolder->buffer_count = 0;
        }

        break;

    case SLIP_STATE_RECEIVING:

        if (data == SLIP_ESC)
        {
            /* Escape sequence incoming. */
            pHolder->state = SLIP_STATE_AWAITING_ESC;
        }
        else if (data == SLIP_END)
        {
            /* If two END bytes come in a row (payload size is 0), assume
               re-start to not have empty packets and to guarantee that
               the packet structure is not corrupted. */
            if (pHolder->buffer_count > 0)
            {
                /* There is a payload, send it to next parsing stage. */
                pHolder->state = SLIP_STATE_AWAITING_START;
                pHolder->rx_success++;

                pHolder->parser(pHolder);
            }
        }
        else
        {
            /* Check for buffer overrun. */
            if (pHolder->buffer_count >= pHolder->buffer_size)
            {
                pHolder->state = SLIP_STATE_AWAITING_START;
                pHolder->rx_error++;
                pHolder->buffer_overrun++;
            }
            else
                pHolder->buffer[pHolder->buffer_count++] = data;
        }

        break;

    case SLIP_STATE_AWAITING_ESC:

        /* Check for escaped data. */
        if (data == SLIP_ESC_END)
        {
            /* Check for buffer overrun. */
            if (pHolder->buffer_count >= pHolder->buffer_size)
            {
                pHolder->state = SLIP_STATE_AWAITING_START;
                pHolder->rx_error++;
                pHolder->buffer_overrun++;
            }
            else
                pHolder->buffer[pHolder->buffer_count++] = SLIP_END;
        }
        else if (data == SLIP_ESC_ESC)
        {
            /* Check for buffer overrun. */
            if (pHolder->buffer_count >= pHolder->buffer_size)
            {
                pHolder->state = SLIP_STATE_AWAITING_START;
                pHolder->rx_error++;
                pHolder->buffer_overrun++;
            }
            else
                pHolder->buffer[pHolder->buffer_count++] = SLIP_ESC;
        }
        else
        {
            /* Package error, reset parser. */
            pHolder->state = SLIP_STATE_AWAITING_START;
            pHolder->rx_error++;
        }
        break;

    default:

        /* Should never come here. */
        pHolder->state = SLIP_STATE_AWAITING_START;
        pHolder->rx_error++;

    }
}
