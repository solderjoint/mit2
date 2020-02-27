/*
 * Header for various voltage const manipulation
 */

#ifndef __VAR_VOLTAGECONSTS_H
#define __VAR_VOLTAGECONSTS_H

#include "util/typedefs.h"

/* **************************************************** *
 *                  SYSTEM-WIDE MACROS
 * **************************************************** */

/* **************************************************** *
 *             CONSTS MANIPULATION WRAPPERS
 * **************************************************** */
int32 VoltageACAdcCountsGet (void);
void VoltageACAdcCountsSet (const int32 x);
float VoltageACMaxGet (void);
void VoltageACMaxSet (const float x);
float VoltageACMultGet (void);

int32 VoltageDCAdcCountsGet (void);
void VoltageDCAdcCountsSet (const int32 x);
float VoltageDCMaxGet (void);
void VoltageDCMaxSet (const float x);
float VoltageDCMultGet (void);

/* **************************************************** */
float VoltageSpikeGet (void);
void VoltageSpikeSet (const float x);

float VoltageUnderflowGet (void);
void VoltageUnderflowSet (const float x);

float VoltageOverflowGet (void);
void VoltageOverflowSet (const float x);

float VoltageNormalGet (void);
void VoltageNormalSet (const float x);

char VoltageConstantsInit (void);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__VAR_VOLTAGECONSTS_H
