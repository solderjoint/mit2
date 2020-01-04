
#include "vars/relaystatus.h"

#include "periph/gpio.h"

/* **************************************************** *
 *                 RELAY STATE CHECKER
 * **************************************************** */
inline int32 RelayStatusGet (void) {
	// implement proper -1 handling when second
	// 'checking' optocouple will be available
	return GpioTriggerGet();
}

int32 RelayStatusSet (void) {
	GpioTriggerSet();
	return RelayStatusGet();
}

int32 RelayStatusReset (void) {
	GpioTriggerReset();
	return RelayStatusGet();
}
