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
 *                 FUNCTION PROTOTYPES
 * **************************************************** */
int32 PeripheralCanRateGet (void);
int32 PeripheralCanRateSet (const uint32 val);

int32 PeripheralSpiRateGet (void);
int32 PeripheralSpiRateSet (const uint32 val);

int32 PeripheralUartRateGet (void);
int32 PeripheralUartRateSet (const uint32 val);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__VAR_PERIPHERAL_H
