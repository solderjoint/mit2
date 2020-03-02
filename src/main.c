/*
 * Calculation module firmware project.
 * rev 1C since august 2019
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
#include "logic/state.h"
#include "logic/config.h"

#include "vars/mutex.h"

/* **************************************************** *
 *               MAIN PROGRAM ENTRY POINT
 * **************************************************** */
#include "periph/gpio.h"
int main(void) {
	ConfigurePeripherals();
	ConfigureVariables();

	while (1) {
		GpioLedsSet(3, -1); // show free cycles in main
		// 333.3K switches == 1.5usec for whole cycle
		if (MutexCheckPending() > 0) {
			if (MutexGetAndClear(MUTEX_COMMCHECK)) StateSmolinCheck();
			if (MutexGetAndClear(MUTEX_VOLTCHECK)) StateVoltageCheck();
			if (MutexGetAndClear(MUTEX_ENDPCHECK)) StateEndpointCheck();
			if (MutexGetAndClear(MUTEX_STATECHECK)) StateLineCheck();
			if (MutexGetAndClear(MUTEX_VOLTUPDATE)) StateVoltageNormalSet();
		}
	}
	return 0;
}
