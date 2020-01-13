
#include "periph/can.h"

#include <stdbool.h>
#include <stddef.h>
//#include <stdint.h>

#include <tivaware/inc/hw_can.h>
#include <tivaware/inc/hw_ints.h>
#include <tivaware/inc/hw_memmap.h>

#include <tivaware/driverlib/can.h>
#include <tivaware/driverlib/gpio.h>
#include <tivaware/driverlib/interrupt.h>
#include <tivaware/driverlib/pin_map.h>
#include <tivaware/driverlib/sysctl.h>

#include "util/print.h"
#include "util/util.h"

/* **************************************************** *
 *                 CAN LOCAL VARIABLES
 * **************************************************** */
// main can object variable
static volatile tCANMsgObject msg_struct;

static uint32 can_obj_id;  // can msg obj id [1..32]
void object_id_set (const uint32 x) { can_obj_id = x & 0x1F; }
uint32 object_id_get (void) { return can_obj_id; }

/* **************************************************** *
 *           CAN MESSAGE BUFFER MANIPULATION
 * **************************************************** */
static volatile uint8 msg_buffer[CAN_MSGBUF_LEN];

inline static void msg_buf_update (const int32 obj_id) {
	CANMessageGet(CAN0_BASE, obj_id, &msg_struct, true);
}

static void msg_buf_init (void) {
	msg_struct.pui8MsgData = msg_buffer;
	msg_struct.ui32MsgID = 0xFFFF; // workaround for 29-bits wide id
	msg_struct.ui32MsgIDMask = 0;  // empty mask receives everything
	msg_struct.ui32Flags = MSG_OBJ_RX_INT_ENABLE \
						 | MSG_OBJ_EXTENDED_ID;
	msg_struct.ui32MsgLen = CAN_MSGBUF_LEN;

	object_id_set(CAN_OBJECT_ID);
}

/* **************************************************** *
 *                   CAN TEST ROUTINE
 * **************************************************** */
#include "periph/gpio.h"  // debug led checking
void _test (void) {
	const uint32 status = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);
	const uint32 obj_id = object_id_get();

//	_println("S[0x%X]", status);
	if (status == CAN_STATUS_RXOK) {
		msg_buf_update(obj_id);
//		_println("[%i]:[%i]:[%i]:[%i]", \
//				msg_struct.ui32MsgLen, msg_struct.ui32MsgID, \
//				msg_struct.ui32Flags, obj_id);
//		CANIntDisable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_STATUS);
//		GpioLedsSet(3, -1);
	}
}

/* **************************************************** *
 *                CAN INTERRUPT HANDLER
 * **************************************************** */
static void (*canTransmissionInterruptAttachment) (void) = NULL;

void CanTransmissionAttachInterrupt (void (*foo) (void)) {
	canTransmissionInterruptAttachment = foo;
}
void CanTransmissionInterruptAttachmentCall (void) {
	if ((*canTransmissionInterruptAttachment) != NULL)
		canTransmissionInterruptAttachment();
}

void CanInterruptHandler (void) {
	const uint32 int_cause = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE);
	const uint32 object = object_id_get();

	if (int_cause == CAN_INT_INTID_STATUS) {
		const uint32 status = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);
		if (status == CAN_STATUS_RXOK) {
			msg_buf_update(object);
//			CanTransmissionInterruptAttachmentCall();
//			_test();
		}
	} else {
		GpioLedsSet(3, -1);
	}
	CANIntClear(CAN0_BASE, object);
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
	CANIntEnable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_STATUS);
	IntEnable(INT_CAN0);
	IntPrioritySet(INT_CAN0, (CAN_INT_PRIORITY << 5));
	CANEnable(CAN0_BASE);
//	SysCtlDelay(100);

	msg_buf_init();
}
