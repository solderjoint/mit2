
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
	_puts("info>capturing.");

	TimeStampSet();
	SamplingInit(fftInputBuf);
	return 0;
}

int32 CrashFourierProcess (void) {
	_println(" in %i ms", TimeStampGet());
//	_putchar('\n'); // newline
	_puts("info>processing");

	TimeStampSet();
	SamplingFinish();             _putchar('.');
	FourierProcess(fftInputBuf);  _putchar('.');
	DomainProcess(fftInputBuf);   _putchar('.');
	_println(" in %i ms", TimeStampGet());
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
				_puts("err>buzzer found\n");
				return LineStatusSet(STATUS_BUZZERFOUND);
			} else {
				_puts("err>line breach\n");
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
		_puts("err>signal lost\n");
		return LineStatusSet(STATUS_SIGNALLOST);
	}
	if (CheckLineVoltageSpike()) {
		_puts("err>voltage spike\n");
		return LineStatusSet(STATUS_VOLTSPIKE);
	}
	else if (CheckLineVoltageOverflow() < 0) {
		_puts("err>line short\n");
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
	CheckLineVarsReset();
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
