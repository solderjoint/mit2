#ifndef __FREQSEARCH_H
#define __FREQSEARCH_H

#include <complex.h>

#include "util/util.h"
#include "util/typedefs.h"

/* **************************************************** *
 *                HEADER SPECIFIC MACROS
 * **************************************************** */
typedef float complex cf32_t;

/* **************************************************** *
 *                 FUNCTION PROTOTYPES
 * **************************************************** */
int32 DomainProcess (cf32_t input[]);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__FREQSEARCH_H
