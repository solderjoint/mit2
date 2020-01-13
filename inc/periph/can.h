#ifndef __CAN_COMM_H
#define __CAN_COMM_H

#include "util/typedefs.h"

/* **************************************************** *
 *                  HEADER SPECIFIC MACROS
 * **************************************************** */
enum _canEnum {
	CAN_RECV_OBJ_ID  = 1,   // default object mesage id
	CAN_SEND_OBJ_ID  = 2,   // default object mesage id
	CAN_INT_PRIORITY = 5,   // medium-low int priority
	CAN_MSGBUF_LEN   = 8,   // standard maximum message length
};

/* **************************************************** *
 *                 FUNCTION PROTOTYPES
 * **************************************************** */
void CanTransmissionInit (const uint32_t rate);
void CanTransmissionAttachInterrupt (void (*foo) (void));

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__CAN_COMM_H
