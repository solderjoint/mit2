/*
 * Smolin protocol abstraction layer
 */

#ifndef __COMM_QUICK_H
#define __COMM_QUICK_H

#include "util/typedefs.h"

/* **************************************************** *
 *                  HEADER SPECIFIC MACROS
 * **************************************************** */

/* **************************************************** *
 *                 FUNCTION PROTOTYPES
 * **************************************************** */
int32 QuickOutputProcess(uint8 buf[8]);
int32 QuickInputProcess(const uint8 buf[8]);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__COMM_QUICK_H
