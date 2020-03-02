
#include "logic/state.h"


#include "logic/comm/smolin.h"
#include "logic/math/domain.h"
#include "logic/math/fft1024.h"
#include "logic/math/linecheck.h"
#include "logic/math/sampling.h"

#include "periph/gpio.h"

#include "util/print.h"
#include "util/util.h"

#include "vars/canmessage.h"
#include "vars/founddomain.h"
#include "vars/linestatus.h"

/* **************************************************** *
 *               LOCAL VARIABLES SECTION
 * **************************************************** */
static float complex fftInputBuf[FOURIER_POINTS];

static uint32 timeStamper;

static void TimeStampSet (void) {
	const uint32 curr = kil(_time()) + _time_ms();
	timeStamper = curr;
}

static uint32 TimeStampGet (void) {
	const uint32 curr = kil(_time()) + _time_ms();
	const uint32 diff = curr - timeStamper;
	return diff;
}

/* **************************************************** *
 *              FOURIER WRAPPER FUNCTIONS
 * **************************************************** */
int32 CrashFourierCapture (void) {
	xprint("info>capturing");

	TimeStampSet();
	SamplingInit(fftInputBuf);
	return 0;
}

int32 CrashFourierProcess (void) {
	xprintln(" in %i ms", TimeStampGet());
	xprint("info>processing");

	TimeStampSet();
	SamplingFinish();
	FourierProcess(fftInputBuf, FOURIER_POINTS);
	DomainProcess(fftInputBuf);
	xprintln(" in %i ms", TimeStampGet());
	return 0;
}

int32 CrashFourierPerform (int32 state) {
	// wait until an entire buffer is filled
	if (state == STATUS_SAMPLING) { // the go-to branch
		if (SamplingBufferIsFull() > 0)
			return LineStatusSet(STATUS_PROCESSING);
	}
	// buffer capture starting point
	else if (state == STATUS_VOLTSPIKE) {
		if (CrashFourierCapture() == 0)
			return LineStatusSet(STATUS_SAMPLING);
	}
	// wait until fourier is processed
	else if (state == STATUS_PROCESSING) {
		if (CrashFourierProcess() == 0) {
			if (DomainResultIsNotEmpty()) {
				xputs("err>buzzer found");
				return LineStatusSet(STATUS_BUZZERFOUND);
			} else {
				xputs("err>line breach");
				return LineStatusSet(STATUS_OVERFLOW);
			}
		}
	}
}

/* **************************************************** *
 *                 LINE TESTING ROUTINE
 * **************************************************** */
int32 CrashCheckLineState (void) {
	if (CheckEndpointSignalLoss()) {
//		xputs("err>signal lost");
		xputchar ('1');
		return LineStatusSet(STATUS_SIGNALLOST);
	}
	if (CheckLineVoltageSpike()) {
//		xputs("err>voltage spike");
		xputchar ('2');
		return LineStatusSet(STATUS_VOLTSPIKE);
	}
	// disabled since voltage can go as low as 50 mV
//	else if (CheckLineVoltageOverflow() < 0) {
//		xputs("err>line short\n");
//		return LineStatusSet(STATUS_UNDERFLOW);
//	}
	return 0;
}

/* **************************************************** *
 *           VARIOUS LOGIC VARIABLE WRAPPERS
 * **************************************************** */
void CrashTriggerSet (void) {
	GpioTriggerSet();
}

void CrashUpdateNormalVoltage (void) {
	CheckLineVoltageNormalUpdate();
}

/* **************************************************** *
 *               MAIN PROGRAM ENTRY POINT
 * **************************************************** */
void StateSmolinCheck (void) {
	if (CanMessageReceiverFlagGet()) {
		SmolinProtocolProcessIncoming();
		CanMessageReceiverFlagClear();
	}
//	if (CanMessageSenderFlagGet()) {
//		SmolinProtocolProcessOutgoing();
//		CanMessageSenderFlagClear();
//	}
}

void StateVoltageCheck (void) {
	ChecklineVoltageBufferRenew();  // call voltage buffer updating
}

void StateEndpointCheck (void) {
	CheckLineEndpointSignalRenew();  // call linecheck endpoint updater
}

void StateVoltageNormalSet (void) {
	CheckLineVoltageNormalUpdate();  // updates normal voltage level
}

/* **************************************************** */
int StateLineCheck (void) {
	const int32 status = LineStatusGet();
	switch (status) {
		case STATUS_OK:
			return CrashCheckLineState();
			break;

		case STATUS_VOLTSPIKE:
		case STATUS_SAMPLING:
		case STATUS_PROCESSING:
			xputchar('#');
			CrashFourierPerform(status);
			break;

		default:
			CrashTriggerSet();
			break;
	}
	return status;
}
