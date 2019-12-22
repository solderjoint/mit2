
#ifndef __TIMER_H
#define __TIMER_H

#include "util/typedefs.h"
#include "util/util.h"

/* **************************************************** *
 *                HEADER SPECIFIC MACROS
 * **************************************************** */

/* **************************************************** *
 *                 FUNCTION PROTOTYPES
 * **************************************************** */
void TimerClockInit (void);
void TimerClockAttachInterrupt (void (*foo) (void));
uint32 TimerClockSecondsGet (void);
uint32 TimerClockMicrosGet (void);
uint32 TimerClockMillisGet (void);
uint32 TimerClockRawGet (void);

void TimerSemaphoreInit (const uint32 period);
void TimerSemaphoreAttachInterrupt (void (*foo) (void));
void TimerSemaphoreCounterIncrement (void);
uint32 TimerSemaphoreCounterGet (void);

void TimerSamplingInit(const uint32 period);
void TimerSamplingDestroy (void);
void TimerSamplingAttachInterrupt (void (*foo) (void));


/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__TIMER_H
