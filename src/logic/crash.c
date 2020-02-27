
#include "logic/crash.h"

#include "vars/linestatus.h"

#include "logic/math/domain.h"
#include "logic/math/fft1024.h"
#include "logic/math/linecheck.h"
#include "logic/math/sampling.h"

#include "periph/gpio.h"

#include "util/print.h"
#include "util/util.h"

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
	xputs("info>capturing");

	TimeStampSet();
	SamplingInit(fftInputBuf);
	return 0;
}

int32 CrashFourierProcess (void) {
	xprintln(" in %i ms", TimeStampGet());
	xputs("info>processing");

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
				xputs("err>buzzer found\n");
				return LineStatusSet(STATUS_BUZZERFOUND);
			} else {
				xputs("err>line breach\n");
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
		xputs("err>signal lost\n");
		return LineStatusSet(STATUS_SIGNALLOST);
	}
	if (CheckLineVoltageSpike()) {
		xputs("err>voltage spike\n");
		return LineStatusSet(STATUS_VOLTSPIKE);
	}
	else if (CheckLineVoltageOverflow() < 0) {
		xputs("err>line short\n");
		return LineStatusSet(STATUS_UNDERFLOW);
	}
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

void CrashVarsInit (void) {
	LineStatusSet(STATUS_OK);
	DomainVarsReset();
	SamplingVarsReset();
}

/* **************************************************** *
 *               MAIN PROGRAM ENTRY POINT
 * **************************************************** */
int32 CrashCheck (void) {
	const int32 status = LineStatusGet();
	if (status == STATUS_OK) {
		CrashCheckLineState();
	} else if ((status >= STATUS_VOLTSPIKE) && (status <= STATUS_PROCESSING)) {
		CrashFourierPerform(status);
	} else if (status == STATUS_SIGNALLOST) {
		CrashTriggerSet();
	}
	return 0;
}
