#ifndef __SYSCALLS_H
#define __SYSCALLS_H

#include <sys/types.h>

/* **************************************************** *
 *                  SYSTEM-WIDE MACROS
 * **************************************************** */
//#define ADC_BITS          12     // adc bits
//#define ADC_RESOLUTION    4096   // adc resolution
//#define ADC_TEMP_SLOPE    4.3f   // int adc temp sensor [mV/C] slope
//#define ADC_TEMP_NORM_RAW 1700   // standard condition temp sensor value

/* **************************************************** *
 *                FUNCTION PROTOTYPES
 * **************************************************** */
void * _sbrk (ptrdiff_t nbytes);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__SYSCALLS_H
