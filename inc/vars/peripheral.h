/*
 * Header for peripheral communication rate set
 */

#ifndef __VAR_PERIPHERAL_H
#define __VAR_PERIPHERAL_H

#include "util/typedefs.h"

/* **************************************************** *
 *                  SYSTEM-WIDE MACROS
 * **************************************************** */

/* **************************************************** *
 *             SECURE LINE STATUS VARIABLE
 * **************************************************** */
int32 PeripheralCanRateGet (void);
int32 PeripheralCanRateSet (const int32 val);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__VAR_PERIPHERAL_H
