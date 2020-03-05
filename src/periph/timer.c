
#include "periph/timer.h"

#include <stdbool.h>
#include <stdlib.h> // for null reference
#include <stdint.h>

#define  PART_TM4C1231H6PZ
#include <tivaware/inc/hw_gpio.h>
#include <tivaware/inc/hw_memmap.h>
#include <tivaware/inc/hw_ints.h>
#include <tivaware/inc/hw_timer.h>

#include <tivaware/driverlib/gpio.h>
#include <tivaware/driverlib/interrupt.h>
#include <tivaware/driverlib/pin_map.h>
#include <tivaware/driverlib/timer.h>
#include <tivaware/driverlib/sysctl.h>

/* **************************************************** *
 *            SYSTEM TIMER VARIABLE WRAPPERS
 * **************************************************** */
volatile static uint32 timerClockSeconds;

void TimerClockSecondsReset (void) { timerClockSeconds = 0; }
void TimerClockSecondsIncrement (void) { timerClockSeconds++; }

uint32 TimerClockSecondsGet (void) { return timerClockSeconds; }

inline uint32 TimerClockRawGet (void) {
	return TimerValueGet(WTIMER0_BASE, TIMER_A);
}

uint32 TimerClockMicrosGet (void) {
//	const uint32 seconds = mil(timerClockSeconds);
	const uint32 micros = TimerClockRawGet() % mil(1);
	return /*seconds + */ micros;
}

uint32 TimerClockMillisGet (void) {
//	const uint32 seconds = timerClockSeconds * 1000;
	const uint32 millis = TimerClockRawGet() / 1000;
	return /*seconds + */ millis;
}

/* **************************************************** *
 *            SYSTEM TIMER INTERRUPT HANDLER
 * **************************************************** */
static void (*timerClockInterruptAttachment) (void) = NULL;

void TimerClockAttachInterrupt (void (*foo) (void)) {
	timerClockInterruptAttachment = foo;
}
void timerClockInterruptAttachmentCall (void) {
	if ((*timerClockInterruptAttachment) != NULL)
		timerClockInterruptAttachment();
}

void TimerClockInterrupt (void) {
	TimerClockSecondsIncrement();
	timerClockInterruptAttachmentCall();
	TimerIntClear(WTIMER0_BASE, TIMER_TIMA_TIMEOUT);
}

/* **************************************************** *
 *             SYSTEM TIMER INITIALIZATION
 * **************************************************** */
void TimerClockInit (void) {
	// main system 1Hz tick provider (each tick == 1us)
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
	while (!SysCtlPeripheralReady(SYSCTL_PERIPH_WTIMER0));

	const uint32 prescaler = SysCtlClockGet() / mil(1);
	TimerConfigure(WTIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC);
	TimerPrescaleSet(WTIMER0_BASE, TIMER_A, prescaler);
	TimerLoadSet(WTIMER0_BASE, TIMER_A, mil(1));
	SysCtlDelay(100);

	TimerIntRegister(WTIMER0_BASE, TIMER_A, TimerClockInterrupt);
	TimerIntEnable(WTIMER0_BASE, TIMER_TIMA_TIMEOUT);
	IntEnable(INT_WTIMER0A);
	SysCtlDelay(100);

	TimerClockSecondsReset(); // start the clock counting
	TimerEnable(WTIMER0_BASE, TIMER_A);
	SysCtlDelay(100);
}

/* **************************************************** *
 *         EXTERNAL ADC TIMER INTERRUPT HANDLER
 * **************************************************** */
static void (*timerSamplingInterruptAttachment) (void) = NULL;

void TimerSamplingAttachInterrupt (void (*foo) (void)) {
	timerSamplingInterruptAttachment = foo;
}
void TimerSamplingInterruptAttachmentCall (void) {
	if ((*timerSamplingInterruptAttachment) != NULL)
		timerSamplingInterruptAttachment();
}

void TimerSamplingInterrupt (void) {
	TimerSamplingInterruptAttachmentCall();
	TimerIntClear(WTIMER1_BASE, TIMER_TIMA_TIMEOUT);
}

/* **************************************************** *
 *        EXTERNAL ADC THROUGH SPI TIMER SECTION
 * **************************************************** */
void TimerSamplingInit (const uint32 period) {
	// freq domain sampling precision timer
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER1);
	while (!SysCtlPeripheralReady(SYSCTL_PERIPH_WTIMER1));

	const uint32 val = SysCtlClockGet() / period;
	TimerConfigure(WTIMER1_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC);
//	TimerPrescaleSet(WTIMER1_BASE, TIMER_A, 1);
	TimerLoadSet(WTIMER1_BASE, TIMER_A, val);
	SysCtlDelay(100);

	TimerIntRegister(WTIMER1_BASE, TIMER_A, TimerSamplingInterrupt);
	TimerIntEnable(WTIMER1_BASE, TIMER_TIMA_TIMEOUT);
	IntEnable(INT_WTIMER1A);
	SysCtlDelay(100);

	TimerEnable(WTIMER1_BASE, TIMER_A);
	SysCtlDelay(100);
}

void TimerSamplingDestroy (void) {
	TimerDisable(WTIMER1_BASE, TIMER_A);
	SysCtlDelay(100);
}

/* **************************************************** *
 *          SEMAPHORE TIMER INTERRUPT HANDLER
 * **************************************************** */
static void (*timerSemaphoreInterruptAttachment) (void) = NULL;

//#include "periph/gpio.h"
void TimerSemaphoreAttachInterrupt (void (*foo) (void)) {
	timerSemaphoreInterruptAttachment = foo;
}
void TimerSemaphoreInterruptAttachmentCall (void) {
	if ((*timerSemaphoreInterruptAttachment) != NULL)
		timerSemaphoreInterruptAttachment();
//	else GpioLedsSet(GPIO_LED3, GPIO_LED_XOR);
}

void TimerSemaphoreInterrupt (void) {
	TimerSemaphoreInterruptAttachmentCall();
	TimerIntClear(WTIMER2_BASE, TIMER_TIMA_TIMEOUT);
}

/* **************************************************** *
 *            SEMAPHORE TIMER INITIALIZATION
 * **************************************************** */
void TimerSemaphoreInit (const uint32 period) {
	// freq domain sampling precision timer
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER2);
	while (!SysCtlPeripheralReady(SYSCTL_PERIPH_WTIMER2));

	const uint32 val = SysCtlClockGet() / period;
	TimerConfigure(WTIMER2_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC);
//	TimerPrescaleSet(WTIMER2_BASE, TIMER_A, 1);
	TimerLoadSet(WTIMER2_BASE, TIMER_A, val);
	SysCtlDelay(100);

	TimerIntRegister(WTIMER2_BASE, TIMER_A, TimerSemaphoreInterrupt);
	TimerIntEnable(WTIMER2_BASE, TIMER_TIMA_TIMEOUT);
	IntEnable(INT_WTIMER2A);
	SysCtlDelay(100);

	TimerEnable(WTIMER2_BASE, TIMER_A);
	SysCtlDelay(100);
}
