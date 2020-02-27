/*
 * Header for various frequency domain const manipulation
 */

#ifndef __VAR_FOURIERCONSTS_H
#define __VAR_FOURIERCONSTS_H

#include "util/typedefs.h"

/* **************************************************** *
 *                  SYSTEM-WIDE MACROS
 * **************************************************** */
//#define FOURIER_SAMPLE_MULT    (0.002442599f)  // adc multiplier

/* **************************************************** *
 *             CONSTS MANIPULATION WRAPPERS
 * **************************************************** */
int32 FourierSamplingPassesGet (void) ;
void FourierSamplingPassesSet (const int32 x);

int32 FourierBufferLengthGet (void) ;
void FourierBufferLengthSet (const int32 x);

int32 FourierSamplingRateGet (void) ;
void FourierSamplingRateSet (const int32 x);

float FourierFreqResolutionGet (void);
float FourierFreqStepGet (void);

float FourierPowerMultMinGet (void);
void FourierPowerMultMinSet (const float x);

float FourierPowerMinGet (void);
void FourierPowerMinSet (const float x);

//float FourierSampleMultGet (void);
//void FourierSampleMultSet (const float x);

int32 FourierFreqReferenceGet (void);
void FourierFreqReferenceSet (const int32 x);

int32 FourierFreqRangeMinGet (void);
void FourierFreqRangeMinSet (const int32 x);

int32 FourierFreqRangeMaxGet (void);
void FourierFreqRangeMaxSet (const int32 x);

void FourierConstantsInit (void);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__VAR_FOURIERCONSTS_H
