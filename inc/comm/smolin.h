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

struct smolin_header_t {
	int32 _reserved:8;
	int32 to:8;
	int32 from:8;
	int32 text_end:1;
	int32 quickdata:1;
	int32 response:1;
	int32 text_pos:4;
};

/* **************************************************** *
 *                 FUNCTION PROTOTYPES
 * **************************************************** */
//int32 SmolinProtocolProcess
//(const uint32 header, const uint32 length, uint8 msg[8]);
void SmolinProtocolProcessIncoming (void);
void SmolinProtocolProcessOutgoing (void);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__COMM_SMOLIN_H
