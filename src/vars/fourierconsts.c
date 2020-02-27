
#include "vars/fourierconsts.h"

#include "project.h"

/* **************************************************** *
 *             DEFAULT FREQUENCY VARIABLES
 * **************************************************** */
static int32 fourierSamplingPasses;
int32 FourierSamplingPassesGet (void) { return fourierSamplingPasses; }
void FourierSamplingPassesSet (const int32 x) {
	if ((x > 0) && (x <= 128)) fourierSamplingPasses = x;
}

static int32 fourierSamplingBufLen;
int32 FourierBufferLengthGet (void) { return fourierSamplingBufLen; }
void FourierBufferLengthSet (const int32 x) {
	if ((x > 0) && (x < 2048)) fourierSamplingBufLen = x;
}

static int32 fourierSamplingRate;
int32 FourierSamplingRateGet (void) { return fourierSamplingRate; }
void FourierSamplingRateSet (const int32 x) {
	if ((x > 0) && (x <= 128000)) fourierSamplingRate = x;
}

float FourierFreqResolutionGet (void) {
	const int32 rate = FourierSamplingRateGet();
	const int32 size = FourierBufferLengthGet();
	return ((float) (rate / ((float) size)));
}

/* **************************************************** *
 *            CONVERSION POWER COEFFICIENTS
 * **************************************************** */
static float fourierPowerMultMin;
float FourierPowerMultMinGet (void) { return fourierPowerMultMin; }
void FourierPowerMultMinSet (const float x) {
	const float fx = (x > 0.1f)? ((x < 100.f)? x : 100.f) : 0.1f;
	fourierPowerMultMin = fx;
}

static float fourierPowerMin;
float FourierPowerMinGet (void) { return fourierPowerMin; }
void FourierPowerMinSet (const float x) {
	const float fx = (x > 100.f)? ((x < 10000.f)? x : 10000.f) : 100.f;
	fourierPowerMin = fx;
}

/* **************************************************** *
 *               LINE DEPENDANT VARIABLES
 * **************************************************** */
//static float fourierSampleMult;
//float FourierSampleMultGet (void) { return fourierSampleMult; }
//void FourierSampleMultSet (const float x) {
//	if ((x > 0.f) && (x < 1.0f)) fourierSampleMult = x;
//}

static int32 fourierFreqRef;
int32 FourierFreqReferenceGet (void) { return fourierFreqRef; }
void FourierFreqReferenceSet (const int32 x) {
	const int32 max = FourierSamplingRateGet() / 2;
	if ((x > 1000) && (x < max)) fourierFreqRef = x;
}

static int32 fourierFreqRangeMin;
int32 FourierFreqRangeMinGet (void) { return fourierFreqRangeMin; }
void FourierFreqRangeMinSet (const int32 x) {
	const int32 max = FourierSamplingRateGet() / 2;
	if ((x > 1000) && (x < max)) fourierFreqRangeMin = x;
}

static int32 fourierFreqRangeMax;
int32 FourierFreqRangeMaxGet (void) { return fourierFreqRangeMax; }
void FourierFreqRangeMaxSet (const int32 x) {
	const int32 max = FourierSamplingRateGet() / 2;
	if ((x > 1000) && (x < max)) fourierFreqRangeMax = x;
}

/* **************************************************** *
 *               LINE DEPENDANT VARIABLES
 * **************************************************** */
void FourierConstantsInit (void) {
	fourierSamplingPasses = FOURIER_SAMPLE_PASSES;
	fourierSamplingBufLen = FOURIER_SAMPLE_BUFLEN;
	fourierSamplingRate = FOURIER_SAMPLE_RATE;

	fourierPowerMultMin = ((float) FOURIER_POWER_MULTMIN) / 100.f;
	fourierPowerMin = (float) FOURIER_POWER_MIN;

	fourierFreqRangeMin = FOURIER_FREQ_RANGEMIN;
	fourierFreqRangeMax = FOURIER_FREQ_RANGEMAX;
	fourierFreqRef = FOURIER_FREQ_REFPOWER;
}
