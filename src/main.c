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

/* **************************************************** *
 *                COMMON HEADERS SECTION
 * **************************************************** */
#include <stdbool.h>
#include <stddef.h>

#define  TM4C1231H6PZ
#define  PART_TM4C1231H6PZ
#define  ARM_MATH_CM4
#define  __FPU_PRESENT  (1)

/* **************************************************** *
 *               PROJECT INCLUDE SECTION
 * **************************************************** */
#include "project.h"

#include "logic/crash.h"
#include "logic/config.h"
#include "periph/gpio.h"
#include "periph/timer.h"
#include "util/print.h"
#include "util/typedefs.h"
//#include "util/util.h"

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
 *             FUNCTION PROTOTYPES SECTION
 * **************************************************** */
void mutex_handler (void);

/* **************************************************** *
 *               MAIN PROGRAM ENTRY POINT
 * **************************************************** */
int main(void) {
	ConfigStartup();
	TimerSemaphoreAttachInterrupt(mutex_handler);
	CrashVarsInit();

	while (1) {
		GpioLedsSet(2, -1); // cpu free time output
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
	}
	return 0;
}

/* **************************************************** *
 *        ADDITIONAL INTERRUPT ROUTINE HANDLERS
 * **************************************************** */
// TODO: make periods configurable through variables
void mutex_handler (void) {
	const uint32 counter = TimerSemaphoreCounterGet();
	if (counter % PERIOD_CHECK_LINE == 0) gMutex.line = 1;
	if (counter % PERIOD_CHECK_COMM == 0) gMutex.comm = 1;
	if (counter % PERIOD_RENEW_VOLT == 0) gMutex.volt = 1;
	TimerSemaphoreCounterIncrement();
}
