/*
 * Header for freq searchable domain manipulations
 */

#ifndef __VAR_BUZZERDOMAIN_H
#define __VAR_BUZZERDOMAIN_H

#include "util/typedefs.h"

/* **************************************************** *
 *                  SYSTEM-WIDE MACROS
 * **************************************************** */
enum buzzerDomainEnum {
	BUZZER_BUFLEN   = 64,
	BUZZER_FREQ_MAX = 10000,
	BUZZER_FREQ_MIN = 1000,
	BUZZER_NUM_MAX  = 32,
};

/* **************************************************** *
 *                 FUNCTION PROTOTYPES
 * **************************************************** */
void BuzzerDomainInit (void);
void BuzzerDomainReset (void);
int32 BuzzerDomainSet (const uint16 num, const uint32 freq);

int32 BuzzerDomainGetFreqStep (void);
int32 BuzzerDomainGetNumByFreq(const int32 freq);
int32 BuzzerDomainGetFreqByNum (const int32 num);
//int32 BuzzerDomainGetFreqByCounter (const uint32 ptr);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__VAR_BUZZERDOMAIN_H
