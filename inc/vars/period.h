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
void PeriodCounterIncrement (void);
uint32 PeriodCounterGet (void);

int32 PeriodSemaphoreFreqGet (void);
int32 PeriodSemaphoreFreqSet (const uint32 val);

int32 PeriodCommCheckGet (void);
int32 PeriodCommCheckSet (const uint32 val);

int32 PeriodLineEndpointCheckGet (void);
int32 PeriodLineEndpointCheckSet (const uint32 val);

int32 PeriodLineVoltCheckGet (void);
int32 PeriodLineVoltCheckSet (const uint32 val);

int32 PeriodLineVoltUpdateGet (void);
int32 PeriodLineVoltUpdateSet (const uint32 val);

int32 PeriodLineStateUpdateGet (void);
int32 PeriodLineStateUpdateSet (const uint32 val);

char PeriodConstantsInit (void);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__VAR_PERIOD_H
