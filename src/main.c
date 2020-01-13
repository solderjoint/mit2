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
 *               PROJECT INCLUDE SECTION
 * **************************************************** */
#include "logic/crash.h"
#include "logic/config.h"

#include "periph/gpio.h"

#include "vars/mutex.h"
#include "vars/period.h"

#include "util/typedefs.h"

/* **************************************************** *
 *               MAIN PROGRAM ENTRY POINT
 * **************************************************** */
int main(void) {
	ConfigStartup();
	ConfigInitVariables();

	while (1) {
		if (MutexCheckPending() > 0) {
			if (MutexGet(MUTEX_LINECHECK)) {
				CrashCheck();
				MutexClear(MUTEX_LINECHECK);
			}
			if (MutexGet(MUTEX_COMMCHECK)) {
				// can message routine
				MutexClear(MUTEX_COMMCHECK);
			}
			if (MutexGet(MUTEX_VOLTUPDATE)) {
				CrashUpdateNormalVoltage();
				MutexClear(MUTEX_VOLTUPDATE);
			}
		}
		GpioLedsSet(2, -1); // cpu free time output
		// 333.3K switches == 1.5usec for whole cycle
	}
	return 0;
}
