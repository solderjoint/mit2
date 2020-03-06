﻿
#include "logic/config.h"

#include "logic/state.h"
#include "logic/comm/smolin.h"
#include "logic/data/db.h"
#include "logic/data/vartable.h"

#include "periph/can.h"
#include "periph/gpio.h"
#include "periph/rom.h"
#include "periph/spi.h"
#include "periph/timer.h"
#include "periph/usart.h"
#include "periph/watchdog.h"

#include "vars/buzzerdomain.h"
#include "vars/canmessage.h"
#include "vars/founddomain.h"
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
	UartConsoleInit(PeriphUartRateGet());
	xprintln("console> ready");

	SpiExternalAdcInit(PeriphSpiRateGet());
	SpiExternalAdcGetNonBlocking();

	CanTransmissionInit(PeriphCanRateGet());
	CanTransmissionAttachInterruptOnReceive(SmolinProtocolProcessIncoming);
//	CanTransmissionAttachInterruptOnReceive(CanMessageReceiverFlagSet);
//	CanTransmissionAttachInterruptOnSend(CanMessageSenderFlagSet);
}

static void periph_gpio (void) {
	GpioModuleCodenameInit();
	GpioModuleAdressInit();
	GpioTriggerInit();
	GpioButtonInit();
	GpioLedsInit();
}

void TimerClockIntHandler (void) {
	GpioLedsSet(GPIO_LED1, GPIO_LED_XOR);  // xor status led
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
static void vars_init (void) {
	CanMessageInit();
	FoundDomainReset();
	FourierConstantsInit();
	PeriodConstantsInit();
	PeriphConstantsInit();
	VoltageConstantsInit();
}

static void vars_logic (void) {
	BuzzerDomainInit();
//	StateUpdateNormalVoltage();
	LineStatusSet(STATUS_OK);
	RelayStatusSet(RELAY_ON);
}

static void vars_restore (void) {
	// restore values from eeprom
	const int32 offset = VAR_READWRITE_OFFSET;
	for (int32 i = offset; i < (offset + kil(10)); i++) {

	}
}

static void vars_mutex (void) {
	const int32 old = PeriodLineStateUpdateGet();
	PeriodLineStateUpdateSet(mil(1));  // infinite delay
	MutexInit();

	const int32 ms = PeriodBootupDelayGet();
	_delay_ms(ms);
	PeriodLineStateUpdateSet(old);
}

/* **************************************************** *
 *                INITIALIZATION SECTION
 * **************************************************** */
int32 Configure (void) {
	periph_system();
	periph_comm();
	periph_gpio();
	periph_other();

	vars_init();
	vars_logic();
	vars_restore();
	vars_mutex();
	// TODO: reinit changed variables

	return 0;
}
