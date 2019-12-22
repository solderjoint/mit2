/*
 * Smolin protocol abstraction layer
 */

#ifndef __COMM_SMOLIN_H
#define __COMM_SMOLIN_H

#include "util/typedefs.h"

/* **************************************************** *
 *                  HEADER SPECIFIC MACROS
 * **************************************************** */

/* **************************************************** *
 *               GLOBAL VARIABLE SECTION
 * **************************************************** */
struct smolin_header_t {
	uint32 _reserved:8;
	uint32 receiver:8;
	uint32 sender:8;
	uint32 text_end:1;
	uint32 quickdata:1;
	uint32 request:1;
	uint32 text_pos:4;
};

/* **************************************************** *
 *                 FUNCTION PROTOTYPES
 * **************************************************** */
int32 SmolinProtocolProcess
(const uint32 header, const uint32 length, const uint8 msg[8]);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__COMM_SMOLIN_H
