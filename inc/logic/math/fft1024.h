/*
 * Fast Fourier Transform for variable complex float points
 * Requires at least 16xN bytes of memory per point
 */

#ifndef __FFT_1024_H
#define __FFT_1024_H

#include <complex.h>
#include <math.h>

#include "util/typedefs.h"

/* **************************************************** *
 *                HEADER SPECIFIC MACROS
 * **************************************************** */
typedef float complex cf32_t;

enum fft1024Enum {
	FOURIER_POINTS  = 1024,   // set fft points len
};

/* **************************************************** *
 *             FOURIER FUNCTION PROTOTYPES
 * **************************************************** */
int FourierProcess (cf32_t input[FOURIER_POINTS], const int size);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__FFT_1024_H
