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
 *                 FUNCTION PROTOTYPES
 * **************************************************** */
int32 SmolinProtocolProcess
(const uint32 header, const uint32 length, uint8 msg[]);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__COMM_SMOLIN_H
