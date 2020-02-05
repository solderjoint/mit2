
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

int32 RelayStatusSet (const int32 state) {
	if (state > RELAY_OFF) GpioTriggerSet();
	else GpioTriggerReset();
	return RelayStatusGet();
}
