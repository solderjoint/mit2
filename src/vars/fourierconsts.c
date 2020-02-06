
#include "vars/fourierconsts.h"
#include "project.h"

/* **************************************************** *
 *             DEFAULT FREQUENCY VARIABLES
 * **************************************************** */
static int32 fourierOversamples;
int32 FourierOversamplesGet (void) { return fourierOversamples; }
void FourierOversamplesSet (const int32 x) {
	if ((x > 0) && (x <= 128)) fourierOversamples = x;
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
 *               LINE DEPENDANT VARIABLES
 * **************************************************** */
//static float fourierSampleMult;
//float FourierSampleMultGet (void) { return fourierSampleMult; }
//void FourierSampleMultSet (const float x) {
//	if ((x > 0.f) && (x < 1.0f)) fourierSampleMult = x;
//}

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

