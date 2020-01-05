/*
 * Calculation module firmware project.
 * rev 1A since august 2019
 *
 * The goal of this project is to complete following routines:
 * + check pin designated as a line endpoint signal catcher
 * + check line voltage, catch overflow and underflow
 * - perform fourier transform to catch signals
 * - complete specified can protocol transactions
 * - perform startup diagnostics to check hardware errors
 * - implements a bootloader with firmware update ability
 */

#ifdef   __cplusplus
#	undef   __cplusplus
#endif

/* **************************************************** *
 *               PROJECT INCLUDE SECTION
 * **************************************************** */
#include "logic/crash.h"
#include "logic/config.h"

#include "periph/gpio.h"
#include "periph/timer.h"

#include "util/print.h"
#include "util/typedefs.h"

#include "vars/period.h"

/* **************************************************** *
 *               GLOBAL VARIABLES SECTION
 * **************************************************** */
struct _mutex_t {
	volatile int8 line:1;  // only 1 bit wide
	volatile int8 comm:1;
	volatile int8 volt:1;
	volatile int8 temp:4;
};
static struct _mutex_t gMutex;

/* **************************************************** *
 *           MUTEX INTERRUPT ROUTINE HANDLER
 * **************************************************** */
void mutex_handler (void) {
	const uint32 timer = PeriodCounterGet();
	const uint32 line = PeriodLineVoltCheckGet();
	const uint32 volt = PeriodLineVoltUpdateGet();
	const uint32 comm = PeriodCommCheckGet();

	if (timer % line == 0) gMutex.line = 1;
	if (timer % comm == 0) gMutex.comm = 1;
	if (timer % volt == 0) gMutex.volt = 1;
	PeriodCounterIncrement();
}

/* **************************************************** *
 *               MAIN PROGRAM ENTRY POINT
 * **************************************************** */
int main(void) {
	ConfigStartup();
	ConfigInitVariables();
	TimerSemaphoreAttachInterrupt(mutex_handler);

	while (1) {
		if (gMutex.line) {
			CrashCheck();
			gMutex.line = 0;
		}
		if (gMutex.comm) {
			// can message routine
			gMutex.comm = 0;
		}
		if (gMutex.volt) {
			CrashUpdateNormalVoltage();
			gMutex.volt = 0;
		}
		GpioLedsSet(2, -1); // cpu free time output
	}
	return 0;
}
