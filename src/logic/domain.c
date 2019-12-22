
#include "logic/domain.h"

#include <math.h>
#include <stddef.h>

#include "vars/buzzerdomain.h"
#include "vars/fourierconsts.h"
#include "util/print.h"

/* **************************************************** *
 *               FOUND FREQUENCIES BUFFER
 * **************************************************** */
struct domain_t {
	uint32 next;
	int32 freq[FREQ_DOM_BUFLEN];
} ;
static struct domain_t foundDomain;

void FoundDomainReset (void) {
	for (uint32 i = 0; i < FREQ_DOM_BUFLEN; i++)
		foundDomain.freq[i] = 0;
	foundDomain.next = 0;
}

void FoundDomainSet (const uint32 freq) {
	const uint32 next = foundDomain.next;
	const uint32 ptr = (next < FREQ_DOM_BUFLEN)? next : 0;
	foundDomain.freq[ptr] = freq;
	foundDomain.next++;
}

int32 FoundDomainGetByCounter (const uint32 counter) {
	if (counter >= FREQ_DOM_BUFLEN) return -1;
	return foundDomain.freq[counter];
}

void FoundDomainClear (const uint32 ptr) {
	if (ptr >= FREQ_DOM_BUFLEN) return -1;
	foundDomain.freq[ptr] = 0;
}

int32 DomainResultIsNotEmpty (void) {
	for (int i = 0; i < FREQ_DOM_BUFLEN; i++) {
		if (foundDomain.freq[i] > 0) return 1;
	}
	return 0;
}

/* **************************************************** *
 *           GET SQUARE AREA UNDER THE GRAPH
 * **************************************************** */
float DomainGetGraphArea
(cf32_t buf[], const uint32 start, const uint32 end) {
	if (end < start) return -10.f;
	float sum = 0.0f;
	for (uint32 i = start + 1; i <= end; i++) {
		const float a = cabsf(buf[i-1]);
		const float b = cabsf(buf[i]);
		const float h1 = (a < b)? a : b;
		const float h2 = ((a > b)? a : b) - h1;
		sum += h1 + (0.5f*h2);
	}
	return sum;
}

/* **************************************************** *
 *         FIND MAXIMUM AREA VALUES IN SEQUENCE
 * **************************************************** */
int32 DomainFindMaxAreaValues
(cf32_t buf[], const uint32 start, const uint32 end) {
	// finds all the max values in set sequence
	if (start > 4096) return -1;
	if (end > 4096) return -2;
	if (end < start) return -10;

	const float pwr_min = FREQ_PWR_MIN;
	const float pwr_mult = FREQ_PWR_MINCOEF;
	const int32 step = BuzzerDomainGetFreqStep() / FourierFreqResolutionGet();

	float max = 0.0f, sum = 0.0f;
	for (uint32 i = start; i <= end; i += step) {
		const float val = DomainGetGraphArea(buf, i-step, i);
		sum += val;
		if (val > max) max = val;
	}
	const float mean = sum / ((end - start) / step);
	const float mmax = pwr_mult * (max - mean);
	const float check_max = (mmax > pwr_min)? mmax : pwr_min;

	int32 count = 0;
	for (uint32 i = start; i <= end; i += step) {
		const float area = DomainGetGraphArea(buf, i - step/2, i + step/2);
		if (area > check_max) {
			const int32 freq = i * FourierFreqResolutionGet();
			const int32 rpwr = ((int32) cabsf(buf[i]));
			_printf(" [%d:%d:%d] ", freq, cast(int32, area), rpwr);
			FoundDomainSet(freq);
			count++;
		}
		if (count >= FREQ_DOM_BUFLEN) continue;
	}
	return count;
}

/* **************************************************** *
 *     UPDATE FOUND MAXIMUM AREA VALUES IN SEQUENCE
 * **************************************************** */
int32 DomainUpdateFoundMaxValues (void) {
	// update domain buffer with newfound values
	const uint32 size = FREQ_DOM_BUFLEN;
	for (int32 i = 0; i < size; i++) {
		const int32 found = FoundDomainGetByCounter(i);
		const int32 num = BuzzerDomainGetNumByFreq(found);
		// check if number does not appear
		if (num == 0) FoundDomainClear(i);
	}
	return 0;
}

/* **************************************************** *
 *        RESET BUZZER DOMAIN VALUES TO DEFAULT
 * **************************************************** */
void DomainVarsReset (void) {
#define BUZZER_FREQ_X1   (4150)
#define BUZZER_FREQ_X2   (4900)
#define BUZZER_FREQ_X3   (5100)
#define BUZZER_FREQ_X4   (5850)
#define BUZZER_FREQ_STEP (50)

	BuzzerDomainReset(); // start counting from zero

	const int32 x2 = 1 + (BUZZER_FREQ_X2 - BUZZER_FREQ_X1) / BUZZER_FREQ_STEP;
	for (int32 i = 0; i < x2; i++) {
		const int32 val = BUZZER_FREQ_X1 + i * BUZZER_FREQ_STEP;
		BuzzerDomainSet(i + 1, val);
	}
	const int32 x4 = 1 + (BUZZER_FREQ_X4 - BUZZER_FREQ_X3) / BUZZER_FREQ_STEP;
	for (int32 i = 0; i < x4; i++) {
		const int32 val = BUZZER_FREQ_X3 + i * BUZZER_FREQ_STEP;
		BuzzerDomainSet(i + 1 + x2, val);
	}
}

/* **************************************************** *
 *     UPDATE FOUND MAXIMUM AREA VALUES IN SEQUENCE
 * **************************************************** */
int32 DomainProcess (cf32_t input[]) {
	const float f1 = FourierFreqRangeMinGet();
	const float f2 = FourierFreqRangeMaxGet();

	const int32 rate = FourierSamplingRateGet();
	const int32 size = FourierBufferLengthGet();
	const float div = ((float) (rate / ((float) size)));
	DomainFindMaxAreaValues(input, cast(int32, f1/div), cast(int32, f2/div));

	return 0;
}
