
#include "logic/math/sampling.h"

#include "periph/spi.h"
#include "periph/timer.h"
#include "vars/fourierconsts.h"
#include "vars/voltageconsts.h"
#include "util/print.h"

#include "project.h"

/* **************************************************** *
 *           SAMPLING BUFFER POINTER WRAPPERS
 * **************************************************** */
static cf32_t *int_cbuf;
static int32 int_cbuf_counter;
static int32 int_samples_counter;

int32 SamplingBufferIsFull (void) {
	const int32 max_oversamples = FourierOversamplesGet();
	return (int_samples_counter >= max_oversamples)? 1 : 0;
}

int32 SamplingBufferOversamplesIncrement (void) {
	if (SamplingBufferIsFull() > 0) return -1;
	int_cbuf_counter = 0;
	int_samples_counter++;
	xputchar('.');
	return int_samples_counter;
}

void SamplingBufferPrepare (void) {
	// initiate pointer to input array
	for (int16 i = 0; i < FourierBufferLengthGet(); i++)
		int_cbuf[i] = 0;
	int_samples_counter = 1;
	int_cbuf_counter = 0;
}

void SamplingBufferSet (float x) {
	if (SamplingBufferIsFull() > 0) return;

	if (int_cbuf_counter < FourierBufferLengthGet()) {
		const float casted = cast(float, x);
		int_cbuf[int_cbuf_counter++] = casted;
	} else {
		SamplingBufferOversamplesIncrement();
	}
}

void SamplingInterruptAttachment (void) {
	const uint16 adcraw = SpiExternalAdcGetNonBlocking();
	const float mult = VoltageACMultGet();
	const float res = mult * adcraw;
	SamplingBufferSet (res);
}

/* **************************************************** *
 *            SAMPLING BUFFER NORMALIZATION
 * **************************************************** */
int32 SamplingNormalize (const uint32 start, const uint32 len) {
	// flatten mins and maxs if they exceed 0.5 and 1.5 of mean
	if ((len < 128) || (len > 8192)) return -1;
	if (len < start) return -10;

	float sum = 0.0f;
	for (uint32 i = start; i < len; i++) sum += int_cbuf[i];
	// flatten sequence
	const float mean = sum / (len - start + 1);
	const float mmin = 0.5f * mean;
	const float mmax = 1.5f * mean;

	for (uint32 i = start; i < len; i++) {
		const float comp = int_cbuf[i];
		if (comp < mmin) int_cbuf[i] = mmin;
		else if (comp > mmax) int_cbuf[i] = mmax;
	}

	// substract mean minimal value to remove dc characteristic
	for (uint32 i = start; i < len; i++) int_cbuf[i] -= mmin;
	return 0;
}

/* **************************************************** *
 *        SAMPLING DEFAULT LOCAL VARIABLES INIT
 * **************************************************** */
void SamplingVarsReset (void) {
	FourierBufferLengthSet(FOURIER_SAMPLE_BUFLEN);
	FourierSamplingRateSet(FOURIER_SAMPLE_RATE);
	FourierOversamplesSet(FOURIER_OVERSAMPLES);
//	FourierSampleMultSet(FOURIER_SAMPLE_MULT);

	FourierFreqRangeMinSet(FOURIER_FREQRANGE_MIN);
	FourierFreqRangeMaxSet(FOURIER_FREQRANGE_MAX);
}

/* **************************************************** *
 *           SAMPLING INITIALIZATION SECTION
 * **************************************************** */
int32 SamplingInit (cf32_t buffer[]) {
	int_cbuf = buffer;     // get pointer to the buffer

	const int32 rate = FourierSamplingRateGet();
	SamplingBufferPrepare();
	TimerSamplingInit(rate);
	TimerSamplingAttachInterrupt(SamplingInterruptAttachment);
	return 0;
}

int32 SamplingFinish (void) {
	TimerSamplingDestroy();
	SamplingNormalize(0, FourierBufferLengthGet());
	return 0;
}
