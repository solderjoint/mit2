#ifndef __CAN_COMM_H
#define __CAN_COMM_H

#include "util/typedefs.h"

/* **************************************************** *
 *                  HEADER SPECIFIC MACROS
 * **************************************************** */
enum _canEnum {
	CAN_INT_PRIORITY = 5,   // medium-low int priority
};

/* **************************************************** *
 *                 FUNCTION PROTOTYPES
 * **************************************************** */
void CanTransmissionInit (const uint32_t rate);
void CanTransmissionAttachInterruptOnReceive (void (*foo) (void));
void CanTransmissionAttachInterruptOnSend (void (*foo) (void));

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__CAN_COMM_H
