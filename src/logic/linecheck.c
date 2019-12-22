
#include "logic/linecheck.h"

#include "periph/gpio.h"
#include "periph/spi.h"
#include "vars/voltageconsts.h"

/* **************************************************** *
 *           LINE ENDPOINT SIGNAL CRASH CHECK
 * **************************************************** */
static uint8 endpointSignalBuffer[LINE_SIGNALBUF_SIZE];

uint8 EndpointSignalPrevGet (void) { return endpointSignalBuffer[0]; }
int32 EndpointSignalGet (void) {
	int32 sum = 0;
	for (int32 i = 0; i < LINE_SIGNALBUF_SIZE; i++)
		sum += endpointSignalBuffer[i];
	const int32 mean = (2 * sum) / LINE_SIGNALBUF_SIZE;
	return mean;
}

void EndpointSignalSet (uint8 x) {
	// append a value in the front of the buffer
	for (int32 i = LINE_SIGNALBUF_SIZE - 1; i > 0; i--) {
		endpointSignalBuffer[i] = endpointSignalBuffer[i-1];
	}
	if (x > 0) endpointSignalBuffer[0] = x;
}

void EndpointSignalRenew (void) {
	const uint8 pin = 1;
//	const uint8 pin = GpioTriggerGet();
	const uint8 res = (pin > 0)? 0 : 1;
	EndpointSignalSet(res);
}

/* **************************************************** *
 *              LINE VOLTAGE INPUT BUFFER
 * **************************************************** */
static float lineVoltageBuffer[LINE_VOLTBUF_SIZE];

float LineVoltageBufferPrevGet (void) { return lineVoltageBuffer[0]; }
float LineVoltageBufferMeanGet (void) {
	// return a mean value of voltage buffer
	float sum = 0.0f;
	for (int32 i = 0; i < LINE_VOLTBUF_SIZE; i++)
		sum += lineVoltageBuffer[i];
	const float mean = sum / LINE_VOLTBUF_SIZE;
	return mean;
}

void LineVoltageBufferSet (float val) {
	// append a value in the front of the buffer
	for (int32 i = LINE_VOLTBUF_SIZE - 1; i > 0; i--) {
		lineVoltageBuffer[i] = lineVoltageBuffer[i-1];
	}
	/*if (val > 0.0f)*/ lineVoltageBuffer[0] = val;
}

void LineVoltageBufferRenew (void) {
	const uint16 adcraw = SpiExternalAdcGetNonBlocking();
	const float mult = /*LineVoltDCMultGet()*/ VOLT_DCMULT;
	const float res = cast(float, adcraw) * mult;
	LineVoltageBufferSet(res);
}

/* **************************************************** *
 *                  LINE STATUS CHECKS
 * **************************************************** */
int32 CheckEndpointSignalLoss (void) {
	EndpointSignalRenew();

	int32 count = 0;
	for (int32 i = 0; i < LINE_SIGNALBUF_SIZE; i++) {
		if (endpointSignalBuffer[i] > 0) count++;
	}
	// if count exceeds half of the buffer return 1
	return count > ((LINE_SIGNALBUF_SIZE + 1) / 2);
}

int32 CheckLineVoltageSpike (void) {
	LineVoltageBufferRenew();

	const float diff = VoltageSpikeGet();
	const float norm = VoltageNormalGet();
	const float mean = LineVoltageBufferMeanGet();
	if (mean > (diff + norm)) return 1;
	else return 0;
}

int32 CheckLineVoltageOverflow (void) {
	const float smax = VoltageOverflowGet();
	const float smin = VoltageUnderflowGet();
	const float mean = LineVoltageBufferMeanGet();

	if (mean > smax) return 1;
	else if (mean < smin) return -1;
	else return 0;
}


void CheckLineVoltageNormalUpdate (void) {
	const float mean = LineVoltageBufferMeanGet();
	VoltageNormalSet(mean);
}

/* **************************************************** *
 *            LOCAL VARIABLES INITIALIZATION
 * **************************************************** */
void CheckLineVarsReset (void) {
	VoltageACMultSet(VOLT_ACMULT);
	VoltageDCMultSet(VOLT_DCMULT);

	VoltageSpikeSet(VOLT_SPIKE);
	VoltageUnderflowSet(VOLT_UNDERFLOW);
	VoltageOverflowSet(VOLT_OVERFLOW);
	VoltageNormalSet(VOLT_NORMAL);
}
