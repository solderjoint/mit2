/*
 * Smolin protocol abstraction layer
 */

#ifndef __COMM_SMOLIN_H
#define __COMM_SMOLIN_H

#include "util/typedefs.h"

/* **************************************************** *
 *                  HEADER SPECIFIC MACROS
 * **************************************************** */
enum smolinEnum {
	CAN_MSGBUF_SIZEY = 8,   // message table y size
	CAN_MSGBUF_SIZEX = 16,  // message table x size
};

/* **************************************************** *
 *                 FUNCTION PROTOTYPES
 * **************************************************** */
int32 SmolinProtocolProcess
(const uint32 header, const uint32 length, uint8 msg[8]);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__COMM_SMOLIN_H
