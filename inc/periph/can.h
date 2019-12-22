#ifndef __CAN_COMM_H
#define __CAN_COMM_H

#include "util/typedefs.h"

/* **************************************************** *
 *                  HEADER SPECIFIC MACROS
 * **************************************************** */
#define CAN_MSGBUF_SIZEX   (16)
#define CAN_MSGBUF_SIZEY   (8)

/* **************************************************** *
 *               GLOBAL VARIABLE SECTION
 * **************************************************** */
struct can_buf_t {
	uint8 msg[8];
} ;

/* **************************************************** *
 *                 FUNCTION PROTOTYPES
 * **************************************************** */
void CanTransmissionInit (const uint32_t rate);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__CAN_COMM_H
