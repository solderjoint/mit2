
#include "logic/config.h"

#include "periph/can.h"
#include "periph/gpio.h"
#include "periph/spi.h"
#include "periph/timer.h"
#include "periph/usart.h"
#include "periph/watchdog.h"

//#include "vars/fourierconsts.h"

#include "util/print.h"
#include "util/crc.h"

/* **************************************************** */
void print_hello (void) {
	_puts("sys>boot\n");
}

// TODO: startup routine handler
// TODO: peripherial rate vars

/* **************************************************** *
 *        SAMPLING DEFAULT LOCAL VARIABLES INIT
 * **************************************************** */
static int32 ConfigPeripheralInit (void) {
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
	CrashVarsReset();
	DomainVarsReset();
	SamplingVarsReset();
	CheckLineVarsReset();
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
	ConfigPeripheralInit(void);
	return 0;
}
