/*
 * Header for peripheral communication rate set
 */

#ifndef __VAR_PERIOD_H
#define __VAR_PERIOD_H

#include "util/typedefs.h"

/* **************************************************** *
 *                  SYSTEM-WIDE MACROS
 * **************************************************** */

/* **************************************************** *
 *                 FUNCTION PROTOTYPES
 * **************************************************** */
int32 PeriodSemaphoreFreqGet (void);
int32 PeriodSemaphoreFreqSet (const uint32 val);

void PeriodLineVoltCheckInit (void);
int32 PeriodLineVoltCheckGet (void);
int32 PeriodLineVoltCheckSet (const uint32 val);

void PeriodLineVoltUpdateInit (void);
int32 PeriodLineVoltUpdateGet (void);
int32 PeriodLineVoltUpdateSet (const uint32 val);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__VAR_PERIOD_H
