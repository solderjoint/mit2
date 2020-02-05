
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
	UsartConsoleInit(PeripheralUsartRateGet());
	_println("console> ready");

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
