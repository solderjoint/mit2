
#include "vars/buzzerdomain.h"

#include <math.h>
#include <stddef.h>

#include "project.h"

#include "vars/fourierconsts.h"

/* **************************************************** *
 *         BUZZER DOMAIN FREQUENCY SEARCH VARS
 * **************************************************** */
struct domain_t {
	uint32 size;
	int16 freq[BUZZER_BUFLEN];
	int16 number [BUZZER_BUFLEN];
} ;
static struct domain_t domain;

/* **************************************************** *
 *       GLOBAL ID NUMBER TO FREQUENCY TRANSFORMS
 * **************************************************** */
int32 BuzzerDomainGetNumByFreq (const int32 freq) {
	if (freq < BUZZER_FREQ_MIN) return 0/*-1*/;
	if (freq > BUZZER_FREQ_MAX) return 0/*-2*/;

	const int32 border = 1.65f * FourierFreqResolutionGet();
	const int16 left = freq + border;
	const int16 right = freq - border;
	// search within left and right borders range
	for (int32 i = 0; (i < BUZZER_BUFLEN); i++) {
		const int16 comp = domain.freq[i];
		if ((comp > right) && (comp < left)) {
			return ((int32) domain.number[i]);
		}
	}
	return 0;
}

int32 BuzzerDomainGetFreqByNum (const int32 num) {
	if (num > BUZZER_NUM_MAX) return -1;

	for (int32 i = 0; (i < BUZZER_BUFLEN); i++) {
		const int32 comp = domain.number[i];
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
		if (domain.number[i] == num) return i;
	return 0;
}

int32 BuzzerDomainGetCounterByFreq (const uint32 freq) {
	for (int32 i = 0; (i < BUZZER_BUFLEN); i++)
		if (domain.freq[i] == freq) return i;
	return 0;
}

int32 BuzzerDomainGetLength() {
	// returns size of currently stored buzzer positions
	int32 counter = 0;
	for (int32 i = 0; i < BUZZER_BUFLEN; i++) {
		if (domain.freq[i] != 0) counter++;
	}
	return counter;
}

/* **************************************************** *
 *                MAIN GET/SET WRAPPERS
 * **************************************************** */
void BuzzerDomainReset (void) {
	for (size_t i = 0; i < BUZZER_BUFLEN; i++) {
		domain.freq[i] = 0;
		domain.number[i] = 0;
	}
	domain.size = 0;
}

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
	domain.number[ptr] = num;
	domain.size++;
}

/* **************************************************** *
 *           EXTERNAL INITIALIZATION WRAPPER
 * **************************************************** */
static const int16 staticfreq[] = {
	FOURIER_FREQ_REFPOWER,  // reference endpoint
	4150, 4200, 4250, 4300, 4350, 4400, 4450, 4500,
	4550, 4600, 4650, 4700, 4750, 4800, 4850, 4900,
	5100, 5150, 5200, 5250, 5300, 5350, 5400, 5450,
	5500, 5550, 5600, 5650, 5700, 5750, 5800, 5850,
};

void BuzzerDomainInit (void) {
	// if no previously set values are found -> init them manually
	domain.size = 0;
	const size = sizeof(staticfreq) / sizeof(staticfreq[0]);
	for (size_t i = 0; i < size; i++) {
		domain.freq[i] = staticfreq[i];
		domain.number[i] = 1 + i;
	}
}
