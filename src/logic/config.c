
#include "logic/config.h"

#include "logic/crash.h"
#include "logic/comm/smolin.h"
#include "logic/data/db.h"

#include "periph/can.h"
#include "periph/gpio.h"
#include "periph/rom.h"
#include "periph/spi.h"
#include "periph/timer.h"
#include "periph/usart.h"
#include "periph/watchdog.h"

#include "vars/buzzerdomain.h"
#include "vars/canmessage.h"
#include "vars/fourierconsts.h"
#include "vars/linestatus.h"
#include "vars/mutex.h"
#include "vars/period.h"
#include "vars/peripheral.h"
#include "vars/relaystatus.h"
#include "vars/voltageconsts.h"

#include "util/print.h"

/* **************************************************** *
 *        SAMPLING DEFAULT LOCAL VARIABLES INIT
 * **************************************************** */
static void periph_system (void) {
	WatchdogInit();
	RomInit();
	TimerClockInit();
}

static void periph_comm (void) {
	UartConsoleInit(PeripheralUartRateGet());
	xprintln("console> ready");

	SpiExternalAdcInit(PeripheralSpiRateGet());
	SpiExternalAdcGetNonBlocking();

	CanTransmissionInit(PeripheralCanRateGet());
	CanTransmissionAttachInterruptOnReceive(SmolinProtocolProcessIncoming);
//	CanTransmissionAttachInterruptOnSend(SmolinProtocolProcessOutgoing);
}

static void periph_gpio (void) {
	GpioModuleCodenameInit();
	GpioModuleAdressInit();
	GpioTriggerInit();
	GpioButtonInit();
	GpioLedsInit();
}

void TimerClockIntHandler (void) {
	GpioLedsSet(1, -1); // xor the led
	WatchdogReset();
}

static void periph_other (void) {
	TimerClockAttachInterrupt(TimerClockIntHandler);
	const int32 rate = PeriodSemaphoreFreqGet();
	TimerSemaphoreInit(rate);
}

/* **************************************************** *
 *        CONFIGURABLE VARIABLES INITIALIZATION
 * **************************************************** */
/*
#include "util/util.h"

#include <stddef.h>
static int32 data[128];

void test (void) {
	// writing int32 takes only 66..166 microseconds!
	const size_t size = sizeof(data) / sizeof(data[0]);
	const int32 key = 0xAA55BEEF;

	for (size_t i = 0; i < size; i++) data[i] = key;
	RomDataWrite (0, data, sizeof(data));
	for (size_t i = 0; i < size; i++) data[i] = 0x00;
	RomDataRead (0, data, sizeof(data));
	for (size_t i = 0; i < size; i++) {
		if (data[i] != key) {
			xprintln ("asserted @%i", i);
			break;
		}
	}
}
*/

int32 ConfigVariablesInit (void) {
	PeriodLineVoltCheckInit();
	PeriodLineVoltUpdateInit();
	PeriodCommCheckInit();
	MutexInit();

	CanMessageInit();

	CrashVarsInit();
}

/* **************************************************** *
 *                INITIALIZATION SECTION
 * **************************************************** */
int32 ConfigPeripheralsInit (void) {
	periph_system();
	periph_comm();
	periph_gpio();
	periph_other();

	return 0;
}
