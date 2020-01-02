
#include "logic/config.h"

#include <logic/data/db.h>

#include "periph/can.h"
#include "periph/gpio.h"
#include "periph/rom.h"
#include "periph/spi.h"
#include "periph/timer.h"
#include "periph/usart.h"
#include "periph/watchdog.h"

//#include "vars/fourierconsts.h"
#include "project.h"

#include "util/print.h"
#include "util/crc.h"

/* **************************************************** */
void print_hello (void) {
	_puts("sys>boot\n");
}

// TODO: peripherial rate vars in rom

/* **************************************************** */
void TimerClockIntHandler (void) {
	GpioLedsSet(1, -1); // xor the led
	WatchdogReset();
}

/* **************************************************** *
 *        SAMPLING DEFAULT LOCAL VARIABLES INIT
 * **************************************************** */
int32 ConfigPeripheralInit (void) {
	WatchdogInit();
	RomInit();
	TimerClockInit();
	TimerClockAttachInterrupt(TimerClockIntHandler);


	GpioModuleCodenameInit();
	GpioModuleAdressInit();
	GpioTriggerInit();
	GpioButtonInit();
	GpioLedsInit();

	SpiExternalAdcInit(SPI_RATE);
	UsartConsoleInit(CONSOLE_RATE);
	CanTransmissionInit(CAN_RATE);
}

static int32 ConfigVariablesInit (void) {
//	CrashVarsReset();

}

/* **************************************************** *
 *              CONFIGURATION VALUE SETTER
 * **************************************************** */
int32 ConfigValueInt32Set (const uint32 hash, const int32 val) {
}

int32 ConfigValueFloatSet (const uint32 hash, const float val) {
}

/* **************************************************** *
 *                INITIALIZATION SECTION
 * **************************************************** */
int32 ConfigStartup (void) {
	ConfigPeripheralInit();
	ConfigVariablesInit();
//	DatabaseFind(0xdeadbeef);
	DatabaseValueUpdateFloat(0xabcdef, 7);
	return 0;
}