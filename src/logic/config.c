
#include "logic/config.h"

#include "comm/smolin.h"

#include "logic/crash.h"
#include "logic/data/db.h"

#include "periph/can.h"
#include "periph/gpio.h"
#include "periph/rom.h"
#include "periph/spi.h"
#include "periph/timer.h"
#include "periph/usart.h"
#include "periph/watchdog.h"

#include "vars/canmessage.h"
#include "vars/fourierconsts.h"
#include "vars/mutex.h"
#include "vars/period.h"
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
	UsartConsoleInit(PeripheralUsartRateGet());
	_println("console> ready");
	SpiExternalAdcInit(PeripheralSpiRateGet());

	CanTransmissionInit(PeripheralCanRateGet());
	CanTransmissionAttachInterruptOnReceive(SmolinProtocolProcessIncoming);
	CanTransmissionAttachInterruptOnSend(SmolinProtocolProcessOutgoing);
}

static void periph_gpio (void) {
	GpioModuleCodenameInit();
	GpioModuleAdressInit();
	GpioTriggerInit();
	GpioButtonInit();
	GpioLedsInit();
}

static void periph_other (void) {
	TimerClockAttachInterrupt(TimerClockIntHandler);
	const int32 rate = PeriodSemaphoreFreqGet();
	TimerSemaphoreInit(rate);
}
// TODO: make variables initialization somewhere

/* **************************************************** *
 *        CONFIGURABLE VARIABLES INITIALIZATION
 * **************************************************** */
int32 ConfigInitVariables (void) {
	PeriodLineVoltCheckInit();
	PeriodLineVoltUpdateInit();
	PeriodCommCheckInit();
	MutexInit();

	CanMessageInit();
	// it's not important to set id first
//	CanMessageReceiverIdSet (0x203F40);

	CrashVarsInit();
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
