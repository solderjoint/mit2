
#include "periph/can.h"

#include <stdbool.h>
#include <stddef.h>

#include <tivaware/inc/hw_can.h>
#include <tivaware/inc/hw_ints.h>
#include <tivaware/inc/hw_memmap.h>

#include <tivaware/driverlib/can.h>
#include <tivaware/driverlib/gpio.h>
#include <tivaware/driverlib/interrupt.h>
#include <tivaware/driverlib/pin_map.h>
#include <tivaware/driverlib/sysctl.h>

#include "util/util.h"

/* **************************************************** *
 *                CAN INTERRUPT HANDLER
 * **************************************************** */
static void (*attached_int_on_recv) (void) = NULL;

void CanTransmissionAttachInterruptOnReceive (void (*foo) (void)) {
	attached_int_on_recv = foo;
}

static inline void CanTransmissionInterruptOnReceiveCall (void) {
	if ((*attached_int_on_recv) != NULL) attached_int_on_recv();
}

/* **************************************************** */
static void (*attached_int_on_send) (void) = NULL;

void CanTransmissionAttachInterruptOnSend (void (*foo) (void)) {
	attached_int_on_send = foo;
}

static inline void CanTransmissionInterruptOnSendCall (void) {
	if ((*attached_int_on_send) != NULL) attached_int_on_send();
}

/* **************************************************** */
void CanInterruptHandler (void) {
	const uint32 cause = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE);

	if (cause == CAN_INT_INTID_STATUS) {
		const int32 status = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);
		if (status & CAN_STATUS_RXOK) {
			CanTransmissionInterruptOnReceiveCall();
		} else if (status & CAN_STATUS_TXOK) {
			CanTransmissionInterruptOnSendCall();
		}
		CANIntClear(CAN0_BASE, cause);
		CANIntClear(CAN0_BASE, status);
	} else {
//		xpanic();  // should never have happened
		CANIntClear(CAN0_BASE, cause);
		CANIntClear(CAN0_BASE, 0);
	}
}

/* **************************************************** *
 *                  CAN INITIALIZATION
 * **************************************************** */
void CanTransmissionInit (const uint32_t rate) {
	const uint32 sys_clock = SysCtlClockGet();
	_check (rate <= sys_clock / 32);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);
	while (!SysCtlPeripheralReady(SYSCTL_PERIPH_CAN0));
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));

	GPIOPinConfigure(GPIO_PB4_CAN0RX);
	GPIOPinConfigure(GPIO_PB5_CAN0TX);
	GPIOPinTypeCAN(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5);

	CANInit(CAN0_BASE);
	CANBitRateSet(CAN0_BASE, sys_clock, rate);

	CANIntRegister(CAN0_BASE, CanInterruptHandler);
	CANIntEnable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);
	IntEnable(INT_CAN0);
	IntPrioritySet(INT_CAN0, (CAN_INT_PRIORITY << 5));
	CANEnable(CAN0_BASE);

	SysCtlDelay(100);
}
