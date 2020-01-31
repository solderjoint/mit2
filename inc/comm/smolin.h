/*
 * Smolin protocol abstraction layer
 */

#ifndef __COMM_SMOLIN_H
#define __COMM_SMOLIN_H

#include "util/typedefs.h"

/* **************************************************** *
 *                  HEADER SPECIFIC MACROS
 * **************************************************** */
//enum smolinEnum {
//	CAN_MSGBUF_SIZEY = 8,   // message table y size
//	CAN_MSGBUF_SIZEX = 8,  // message table x size
//};

enum smolinHeaderEnum {
	HEADER_TEXTPOS  = 0,
	HEADER_REQUEST  = 5,
	HEADER_FASTDATA = 6,
	HEADER_TEXTEND  = 7,
	HEADER_ID_FROM  = 8,
	HEADER_ID_DEST  = 16,
	HEADER_MBUSDATA = 24,
	HEADER_MBUS_FUN = 25,
	HEADER_FULL_MSG = 0x1FFFFFFF,
};

/* **************************************************** *
 *                 FUNCTION PROTOTYPES
 * **************************************************** */
void SmolinProtocolProcessIncoming (void);
void SmolinProtocolProcessOutgoing (void);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__COMM_SMOLIN_H
