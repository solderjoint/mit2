
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
#include "vars/period.h"

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
				return LineStatusSet(STATUS_LINEBREACH);
			}
		}
	}
}

/* **************************************************** *
 *                 LINE TESTING ROUTINE
 * **************************************************** */
int32 CrashCheckLineState (void) {
	if (CheckEndpointSignalLoss()) {
		xputs("err>signal lost");
		return LineStatusSet(STATUS_SIGNALLOST);
	}
	if (CheckLineVoltageSpike()) {
		xputs("err>voltage spike");
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
 *               MAIN PROGRAM ENTRY POINT
 * **************************************************** */
static int32 comm_noflag = 0;

void StateSmolinCheck (void) {
	// read message from buffer if any is received
	if (CanMessageReceiverFlagGet()) {
		GpioLedsSet(GPIO_LED2, GPIO_LED_ON);
		SmolinProtocolProcessIncoming();
		CanMessageReceiverFlagClear();
		comm_noflag = 0;
	} else {
		if (comm_noflag > mil(1)) {
			// error: communication channel lost
			GpioLedsSet(GPIO_LED2, GPIO_LED_OFF);
		} else comm_noflag++;
	}
}
/* **************************************************** */

inline void StateVoltageCheck (void) {
	// call voltage buffer updating
	CheckLineVoltageBufferRenew();
}

inline void StateEndpointCheck (void) {
	// call linecheck endpoint updater
	CheckLineEndpointSignalRenew();
}

inline void StateVoltageNormalSet (void) {
	// updates normal voltage level
	if (LineStatusGet() == STATUS_OK) CheckLineVoltageNormalUpdate();
}

static void StateRelaySet (void) {
	GpioTriggerSet();
}

/* **************************************************** */
int StateLineCheck (void) {
	int32 status = LineStatusGet();
	switch (status) {
		case STATUS_OK:
			GpioLedsSet(GPIO_LED3, GPIO_LED_OFF);
			status = CrashCheckLineState();
			break;

		case STATUS_VOLTSPIKE:
		case STATUS_SAMPLING:
		case STATUS_PROCESSING:
			GpioLedsSet(GPIO_LED3, GPIO_LED_XOR);
			CrashFourierPerform(status);
			break;

		default:
			GpioLedsSet(GPIO_LED3, GPIO_LED_ON);
			StateRelaySet();
			break;
	}
	return status;
}
