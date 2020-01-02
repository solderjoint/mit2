#ifndef __UTIL_H
#define __UTIL_H

//#include <stdint.h>
#include "util/typedefs.h"

/* **************************************************** *
 *              MATHEMATICAL NUMBER MACROS
 * **************************************************** */
#define CONST_E     (2.7182818285f)
#define CONST_PI    (3.1415926535f)

#define kil(n)         (1000 * (n))
#define mil(n)         (1000000 * (n))
#define bil(n)         (1000000000L * (n))

/* **************************************************** *
 *               ADDITIONAL USEFUL MACROS
 * **************************************************** */
#define cast(T, X)     ((T) (X))
//#define min(a, b)        ((a) < (b) ? (a) : (b))
//#define max(a, b)        ((a) > (b) ? (a) : (b))

/* **************************************************** *
 *              BITWISE OPERATION MACROS
 * **************************************************** */
#define bitget(NUM, BIT)   ((NUM) & (1 << (BIT)) )
#define bitset(NUM, BIT)   ((NUM) | (1 << (BIT)) )
#define bitclr(NUM, BIT)   ((NUM) &~(1 << (BIT)) )

/* **************************************************** *
 *                 FUNCTION PROTOTYPES
 * **************************************************** */
uint32 _time (void);
uint32 _time_ms (void);
uint32 _time_us (void);
uint32 _time_raw (void);

void _delay (uint32 length);
void _delay_ms (uint32 length);
void _delay_us (uint32 length);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif  //__UTIL_H