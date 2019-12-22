/*
 * Header for various voltage const manipulation
 */

#ifndef __VAR_VOLTAGECONSTS_H
#define __VAR_VOLTAGECONSTS_H

#include "util/typedefs.h"

/* **************************************************** *
 *                  SYSTEM-WIDE MACROS
 * **************************************************** */
#define VOLT_DCMULT     (0.011724475f)  // convert 0..2048ADC to 0..24VDC
#define VOLT_ACMULT     (0.002442599f)  // convert 0..2048ADC to 0..5VAC
#define VOLT_SPIKE      (4.00f) // minimum voltage diff to crash
#define VOLT_NORMAL     (4.00f) // normal conditions voltage
#define VOLT_OVERFLOW   (22.5f) // overflow voltage value [V]
#define VOLT_UNDERFLOW  (0.50f) // underflow voltage value [V]

/* **************************************************** *
 *             CONSTS MANIPULATION WRAPPERS
 * **************************************************** */
float VoltageACMultGet (void);
void VoltageACMultSet (const float x);

float VoltageDCMultGet (void);
void VoltageDCMultSet (const float x);

float VoltageSpikeGet (void);
void VoltageSpikeSet (const float x);

float VoltageUnderflowGet (void);
void VoltageUnderflowSet (const float x);

float VoltageOverflowGet (void);
void VoltageOverflowSet (const float x);

float VoltageNormalGet (void);
void VoltageNormalSet (const float x);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__VAR_VOLTAGECONSTS_H
