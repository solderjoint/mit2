﻿/*
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
int32 PeriphCanRateGet (void);
int32 PeriphCanRateSet (const uint32 x);

int32 PeriphSpiRateGet (void);
int32 PeriphSpiRateSet (const uint32 x);

int32 PeriphUartRateGet (void);
int32 PeriphUartRateSet (const uint32 x);

char PeriphConstantsInit (void);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__VAR_PERIPHERAL_H
