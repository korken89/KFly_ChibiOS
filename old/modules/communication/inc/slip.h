#ifndef __SLIP_H
#define __SLIP_H

#include "circularbuffer.h"

/*===========================================================================*/
/* Module global definitions.                                                */
/*===========================================================================*/

/**
 * @brief   SLIP reserved END byte.
 */
#define SLIP_END                0xC0
/**
 * @brief   SLIP reserved ESC byte, used for escaping sequences.
 */
#define SLIP_ESC                0xDB
/**
 * @brief   SLIP ESC_END byte, used after ESC to indicate an escaped END byte.
 */
#define SLIP_ESC_END            0xDC
/**
 * @brief   SLIP ESC_ESC byte, used after ESC to indicate an escaped ESC byte.
 */
#define SLIP_ESC_ESC            0xDD

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Possible states of the SLIP state machine.
 */
typedef enum
{
    /**
     * @brief   Waiting for the starting byte.
     */
    SLIP_STATE_AWAITING_START = 1,
    /**
     * @brief   Receiving data.
     */
    SLIP_STATE_RECEIVING,
    /**
     * @brief   Waiting for escape character..
     */
    SLIP_STATE_AWAITING_ESC
} slip_state_t;

/**
 * @brief   The structure to keep track of the states and data through the
 *          SLIP state machine.
 */
typedef struct _slip_parser
{
    /**
     * @brief   Pointer to the buffer storing the data.
     */
    uint8_t *buffer;
    /**
     * @brief   The size of the buffer.
     */
    uint16_t buffer_size;
    /**
     * @brief   The current location in the buffer.
     */
    uint16_t buffer_count;
    /**
     * @brief   Buffer overrun counter.
     */
    uint32_t buffer_overrun;
    /**
     * @brief   The number of receive errors.
     */
    uint32_t rx_error;
    /**
     * @brief   The number of correctly received packets.
     */
    uint32_t rx_success;
    /**
     * @brief   Current state of the state machine.
     */
    slip_state_t state;
    /**
     * @brief    Pointer to the parser to parse the data after a
     *           successful transfer.
     */
    void (*parser)(struct _slip_parser *);
} slip_parser_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

bool GenerateSLIP(uint8_t *data,
                  const uint32_t size,
                  circular_buffer_t *cb);
bool GenerateSLIP_HBT(uint8_t *head,
                      const uint32_t h_size,
                      uint8_t *body,
                      const uint32_t b_size,
                      uint8_t *tail,
                      const uint32_t t_size,
                      circular_buffer_t *cb);
bool GenerateSLIP_MultiChunk(uint8_t *ptr_list[],
                             uint32_t length_list[],
                             const uint32_t size,
                             circular_buffer_t *cb);
void InitSLIPParser(slip_parser_t *p,
                    uint8_t *buffer,
                    const uint16_t buffer_size,
                    void (*parser)(slip_parser_t *));
void ResetSLIPParser(slip_parser_t *p);
void ParseSLIP(uint8_t data, slip_parser_t *p);

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/


#endif
