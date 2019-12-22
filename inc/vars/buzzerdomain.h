/*
 * Header for freq searchable domain manipulations
 */

#ifndef __VAR_BUZZERDOMAIN_H
#define __VAR_BUZZERDOMAIN_H

#include "util/typedefs.h"

/* **************************************************** *
 *                  SYSTEM-WIDE MACROS
 * **************************************************** */
#define BUZZER_BUFLEN    (32)
#define BUZZER_FREQ_MAX  (1 << 15)
#define BUZZER_FREQ_MIN  (1 << 10)
#define BUZZER_NUM_MAX   (1 << 10)

/* **************************************************** *
 *             SECURE LINE STATUS VARIABLE
 * **************************************************** */
void BuzzerDomainReset (void);
int32 BuzzerDomainSet (const uint16 num, const uint32 freq);

int32 BuzzerDomainGetFreqStep (void);
int32 BuzzerDomainGetNumByFreq (const uint32 freq);
int32 BuzzerDomainGetFreqByNum (const uint16 num);
//int32 BuzzerDomainGetFreqByCounter (const uint32 ptr);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__VAR_BUZZERDOMAIN_H
