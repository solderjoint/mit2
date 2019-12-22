
#include "periph/watchdog.h"

#include <stdbool.h>
#include <stdint.h>

#define  PART_TM4C1231H6PZ
#include <tivaware/inc/hw_gpio.h>
#include <tivaware/inc/hw_memmap.h>
#include <tivaware/inc/hw_watchdog.h>

//#include <tivaware/driverlib/gpio.h>
#include <tivaware/driverlib/interrupt.h>
#include <tivaware/driverlib/pin_map.h>
#include <tivaware/driverlib/sysctl.h>
#include <tivaware/driverlib/watchdog.h>

/* **************************************************** *
 *               WATCHDOG LOCAL SETTINGS
 * **************************************************** */
// it is costly to call SysCtlClockGet() each time
static uint32 watchdogPeriod;
void WatchdogPeriodSet (uint32 x) { watchdogPeriod = x; }
uint32 WatchdogPeriodGet (void) { return watchdogPeriod; }

/* **************************************************** *
 *               WATCHDOG BASIC FUNCTIONS
 * **************************************************** */
void WatchdogReset (void) {
	if (WatchdogLockState(WATCHDOG0_BASE) == true) {
		WatchdogUnlock(WATCHDOG0_BASE);
	}
	const uint32 period = WatchdogPeriodGet();
	WatchdogReloadSet(WATCHDOG0_BASE, period);
	WatchdogResetEnable(WATCHDOG0_BASE);
	WatchdogStallEnable(WATCHDOG0_BASE);
	WatchdogEnable(WATCHDOG0_BASE);
}

void WatchdogInit (void) {
	// set watchdog to fire every N seconds
	IntMasterDisable();
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG0);
	while (!SysCtlPeripheralReady(SYSCTL_PERIPH_WDOG0));

	const uint32 clock = (SysCtlClockGet() >> 1);
	const uint32 period = WATCHDOG_RELOAD_PERIOD * clock;
	WatchdogPeriodSet(period);
	WatchdogReset();
	IntMasterEnable();
}

/* **************************************************** */
