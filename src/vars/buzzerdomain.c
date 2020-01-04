
#include "vars/buzzerdomain.h"

#include <math.h>

/* **************************************************** *
 *         BUZZER DOMAIN FREQUENCY SEARCH VARS
 * **************************************************** */
struct domain_t {
	uint32 size;
	int16 freq[BUZZER_BUFLEN];
	int16 num [BUZZER_BUFLEN];
} ;
static struct domain_t domain;

/* **************************************************** *
 *       GLOBAL ID NUMBER TO FREQUENCY TRANSFORMS
 * **************************************************** */
int32 BuzzerDomainGetNumByFreq (const int32 freq) {
	if (freq < BUZZER_FREQ_MIN) return -1;
	if (freq > BUZZER_FREQ_MAX) return -2;

	const int32 off = 20;
	const int32 max = freq + off;
	const int32 min = freq - off;
	for (int32 i = 0; (i < BUZZER_BUFLEN); i++) {
		const int32 comp = domain.freq[i];
		if ((comp > min) && (comp < max)) return ((int32) domain.num[i]);
	}
	return 0;
}

int32 BuzzerDomainGetFreqByNum (const int32 num) {
	if (num > BUZZER_NUM_MAX) return -1;

	for (int32 i = 0; (i < BUZZER_BUFLEN); i++) {
		const int32 comp = domain.num[i];
		if (comp == num) return domain.freq[i];
	}
	return 0;
}

int32 BuzzerDomainGetFreqStep (void) {
	// returns minimal set buzzer frequency step
	int32 step = 1000;
	for (int i = 1; i < domain.size; i++) {
		const int32 diff = abs(domain.freq[i] - domain.freq[i-1]);
		if ((diff >= 20) && (diff < step)) step = diff;
	}
	return step;
}

/* **************************************************** *
 *          FREQUENCY SEARCH UTILITY FUNCTIONS
 * **************************************************** */
int32 BuzzerDomainGetFreqByCounter (const uint32 ptr) {
	if (ptr >= BUZZER_BUFLEN) return -1;
	const uint32 freq = domain.freq[ptr];
	if ((freq < BUZZER_FREQ_MAX) && (freq > BUZZER_FREQ_MIN)) return freq;
	else return 0;
}

int32 BuzzerDomainGetCounterByNum (const uint16 num) {
	for (int32 i = 0; (i < BUZZER_BUFLEN); i++)
		if (domain.num[i] == num) return i;
	return 0;
}

int32 BuzzerDomainGetCounterByFreq (const uint32 freq) {
	for (int32 i = 0; (i < BUZZER_BUFLEN); i++)
		if (domain.freq[i] == freq) return i;
	return 0;
}

/* **************************************************** *
 *                MAIN GET/SET WRAPPERS
 * **************************************************** */
void BuzzerDomainReset (void) { domain.size = 0; }

int32 BuzzerDomainSet (const uint16 num, const uint32 freq) {
	if (freq < BUZZER_FREQ_MIN) return -1;
	if (freq > BUZZER_FREQ_MAX) return -2;
	if (num > BUZZER_NUM_MAX) return -3;

	uint32 ptr = (domain.size < BUZZER_BUFLEN)? domain.size : 0;
	const int32 sfreq = BuzzerDomainGetFreqByNum(num);
	const int32 snum = BuzzerDomainGetNumByFreq(freq);
	if (sfreq > 0) ptr = BuzzerDomainGetCounterByFreq(sfreq);
	else if (snum > 0) ptr = BuzzerDomainGetCounterByNum(snum);

	domain.freq[ptr] = freq;
	domain.num[ptr] = num;
	domain.size++;
}
