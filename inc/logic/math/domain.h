#ifndef __FREQSEARCH_H
#define __FREQSEARCH_H

#include <complex.h>

#include "util/util.h"
#include "util/typedefs.h"

/* **************************************************** *
 *                HEADER SPECIFIC MACROS
 * **************************************************** */
typedef float complex cf32_t;

#define DOMAIN_PWRBUF_LEN   (256)    //
#define FREQ_DOM_BUFLEN     (64)     // input and output buffer length
#define FREQ_PWR_MINCOEF    (0.35f)  // minimum value to consider = max*coef
#define FREQ_PWR_MIN        (300.f)  // minimum power area value

/* **************************************************** *
 *                 FUNCTION PROTOTYPES
 * **************************************************** */
int32 DomainProcess (cf32_t input[]);

int32 DomainResultIsNotEmpty (void);
void DomainVarsReset (void);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__FREQSEARCH_H
