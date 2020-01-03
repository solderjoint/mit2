/*
 * Fast Fourier Transform for 1024 complex float points
 */

#ifndef __SAMPLING_H
#define __SAMPLING_H

#include <complex.h>
#include <math.h>

#include "util/typedefs.h"

/* **************************************************** *
 *                HEADER SPECIFIC MACROS
 * **************************************************** */
typedef float complex cf32_t;

#define SAMPLING_OVERSAMPLES  (4)  // N of oversamples
#define SAMPLING_BUFFERMULT   (0.00244141f)  // volt result mult

/* **************************************************** *
 *             FOURIER FUNCTION PROTOTYPES
 * **************************************************** */
int32 SamplingInit (cf32_t buffer[]);
int32 SamplingFinish (void);

int32 SamplingBufferIsFull(void);
void SamplingVarsReset (void);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__SAMPLING_H