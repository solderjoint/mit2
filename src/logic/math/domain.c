
#include "logic/math/domain.h"

#include <math.h>
#include <stddef.h>

#include "vars/buzzerdomain.h"
#include "vars/founddomain.h"
#include "vars/fourierconsts.h"
#include "util/print.h"

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

	const float pwr_min = FourierPowerMinGet();
	const float pwr_mult = FourierPowerMultMinGet();
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
			const int32 num = BuzzerDomainGetNumByFreq(freq);
//			const int32 rawpwr = cabsf(buf[i]);
			xprint("[F%d:A%d:N%d] ", freq, (int32) area, num);
			FoundDomainSet(freq);
			count++;
		}
		if (count >= FoundDomainGetLengthMax()) continue;
	}
	return count;
}

/* **************************************************** *
 *     UPDATE FOUND MAXIMUM AREA VALUES IN SEQUENCE
 * **************************************************** */
int32 DomainUpdateFoundMaxValues (void) {
	// update domain buffer with newfound values
	const uint32 size = FoundDomainGetLengthMax();
	for (int32 i = 0; i < size; i++) {
		const int32 found = FoundDomainGetByCounter(i);
		const int32 num = BuzzerDomainGetNumByFreq(found);
		// check if number does not appear
		if (num == 0) FoundDomainClear(i);
	}
	return 0;
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
