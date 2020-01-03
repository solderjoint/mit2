
#include "logic/config.h"

#include "logic/crash.h"
#include "logic/data/db.h"

#include "periph/can.h"
#include "periph/gpio.h"
#include "periph/rom.h"
#include "periph/spi.h"
#include "periph/timer.h"
#include "periph/usart.h"
#include "periph/watchdog.h"

#include "vars/fourierconsts.h"
#include "vars/peripheral.h"

#include "util/print.h"
//#include "util/crc.h"


/* **************************************************** *
 *               WATCHDOG RESET PROVIDER
 * **************************************************** */
void TimerClockIntHandler (void) {
	GpioLedsSet(1, -1); // xor the led
	WatchdogReset();
}

/* **************************************************** *
 *        SAMPLING DEFAULT LOCAL VARIABLES INIT
 * **************************************************** */
static void periph_system (void) {
	WatchdogInit();
	RomInit();
	TimerClockInit();
}

static void periph_comm (void) {
	int32 rate = PeripheralSpiRateGet();
	SpiExternalAdcInit(rate);
	rate = PeripheralUsartRateGet();
	UsartConsoleInit(rate);
	rate = PeripheralCanRateGet();
	CanTransmissionInit(rate);
}

static void periph_gpio (void) {
	GpioModuleCodenameInit();
	GpioModuleAdressInit();
	GpioTriggerInit();
	GpioButtonInit();
	GpioLedsInit();
}

static void periph_other (void) {
	// let watchdog be the last to be updated
	TimerClockAttachInterrupt(TimerClockIntHandler);
}

/* **************************************************** *
 *               TIMER SEMAPHORE WRAPPER
 * **************************************************** */
int32 ConfigMutexStart (const uint32 rate, void (*fun) (void)) {
	const uint32 r = ((rate > 10) && (rate < kil(1)))? rate : 100;
	TimerSemaphoreInit(r);
	TimerSemaphoreAttachInterrupt(fun);
}

/* **************************************************** *
 *                INITIALIZATION SECTION
 * **************************************************** */
int32 ConfigStartup (void) {
	periph_system();
	periph_comm();
	periph_gpio();
	periph_other();

	return 0;
}
