/* *
 *
 * Working layer for circular buffers
 *
 * */

#include "ch.h"
#include "hal.h"
#include "crc.h"
#include "circularbuffer.h"

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

/**
 * @brief           Check if a number is a power of 2.
 *
 * @param[in] x     The number to be checked.
 * @return          True if it is a power of 2.
 */
static inline bool isPowerOfTwo(const size_t x)
{
    return ((x != 0) && !(x & (x - 1)));
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief                   Initializes a circular buffer except the mutex.
 *
 * @param[in] Cbuff         Pointer to the circular buffer.
 * @param[in] buffer        Pointer to where the circular buffer data is stored.
 * @param[in] buffer_size   Size of the circular buffer in bytes, must be a
 *                          power of 2.
 */
void CircularBuffer_Init(circular_buffer_t *Cbuff,
                         uint8_t *buffer,
                         const size_t buffer_size)
{
    Cbuff->head = 0;
    Cbuff->tail = 0;

    if (isPowerOfTwo(buffer_size))
    {
        Cbuff->size = buffer_size;
        Cbuff->buffer = buffer;
        Cbuff->mask = buffer_size -1;
    }
    else
    {
        Cbuff->size = 0;
        Cbuff->buffer = 0;
        Cbuff->mask = 0;
    }
}

/**
 * @brief               Initializes the circular buffer mutex.
 *
 * @param[in] Cbuff     Pointer to the circular buffer.
 */
void CircularBuffer_InitMutex(circular_buffer_t *Cbuff)
{
    chMtxObjectInit(&Cbuff->write_lock);
}

/**
 * @brief               Writes a chunk of data to a circular buffer.
 * @note                This algorithm assumes you have checked that the
 *                      data will fit inside the buffer.
 *
 * @param[in/out] Cbuff Pointer to the circular buffer.
 * @param[in] data      Pointer to the data being written.
 * @param[in] count     Size of the data being written in bytes.
 */
void CircularBuffer_WriteChunk(circular_buffer_t *Cbuff,
                               uint8_t *data,
                               const size_t count)
{
    size_t i, head, from_bot, to_top;

    head = Cbuff->head;
    to_top = Cbuff->size - Cbuff->head;


    if (to_top < count)
    {   /* If we need to wrap around during the write */
        from_bot = count - to_top;

        /* First we fill to the top */
        for (i = 0; i < to_top; i++)
            Cbuff->buffer[head + i] = data[i];

        /* Then we fill the rest */
        for (i = 0; i < from_bot; i++)
            Cbuff->buffer[i] = data[to_top + i];

        Cbuff->head = from_bot; /* The end value of head was pre-calculated */
    }
    else
    {   /* No wrap around needed, chunk will fit in the space left to the top */
        for (i = 0; i < count; i++)
            Cbuff->buffer[head + i] = data[i];

        Cbuff->head = (Cbuff->head + count); /* There will be no wrap around */
    }
}


/**
 * @brief               Reads a chunk of data from a circular buffer.
 *
 * @param[in/out] Cbuff Pointer to the circular buffer.
 * @param[out] data     Pointer to write the data.
 * @param[in] count     Number of bytes to read.
 */
void CircularBuffer_ReadChunk(circular_buffer_t *Cbuff,
                              uint8_t *data,
                              size_t count)
{
    (void)Cbuff;
    (void)data;
    (void)count;
}


/**
 * @brief               Generates a pointer to the tail byte and returns a size
 *                      which for how many bytes can be read from the circular
 *                      buffer.
 *
 * @param[in/out] Cbuff Pointer to the circular buffer.
 * @param[out] size     Pointer to the size holder.
 * @return              Pointer to the buffer with offset.
 */
uint8_t *CircularBuffer_GetReadPointer(circular_buffer_t *Cbuff,
                                       size_t *size)
{
    uint8_t *p;

    p = (Cbuff->buffer + Cbuff->tail);

    if (Cbuff->head < Cbuff->tail)
        *size = Cbuff->size - Cbuff->tail;
    else
        *size = Cbuff->head - Cbuff->tail;

    return p;
}

