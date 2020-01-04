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
void QuickOutputPrepare (uint8 buf[8]);
void QuickInputProcess (const uint8 buf[8]);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__COMM_QUICK_H
