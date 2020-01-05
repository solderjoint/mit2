
#include "periph/can.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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
static tCANMsgObject msg_struct;

static uint32 can_obj_id;  // can msg obj id [1..31]
void object_id_set (const uint32 x) { can_obj_id = x & 0x1F; }
uint32 object_id_get (void) { return can_obj_id; }


/* **************************************************** *
 *           CAN MESSAGE BUFFER MANIPULATION
 * **************************************************** */
static volatile uint8 msg_buffer[CAN_MSGBUF_LEN];

static void msg_buf_update (void) {
	msg_struct.pui8MsgData = msg_buffer;
	CANMessageGet(CAN0_BASE, object_id_get(), &msg_struct, 0);
	if (msg_struct.ui32Flags & MSG_OBJ_DATA_LOST) {
		_println("info>\tcan msg loss");
	}
}

static void msg_buf_init (void) {
	msg_struct.ui32MsgID = 0xFFFF; // workaround for 29-bits wide id
	msg_struct.ui32MsgIDMask = 0;  // empty mask receives everything
	msg_struct.ui32Flags = MSG_OBJ_RX_INT_ENABLE;  // only on receive
	msg_struct.ui32MsgLen = 8;     // fixed message length 8 bytes

	object_id_set(CAN_OBJECT_ID);
	const uint32 obj = object_id_get();
	CANMessageSet(CAN0_BASE, obj, &msg_struct, MSG_OBJ_TYPE_RX);
}

/* **************************************************** *
 *                   CAN TEST ROUTINE
 * **************************************************** */
void _test (void);

void _test (void) {
	const uint32 status = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);
	msg_buf_update();

	const uint32 *ptr = (CAN0_BASE + CAN_O_IF1ARB1);
	const uint32 id1 = *ptr & CAN_IF1ARB1_ID_M; // bigger part
	const uint32 id2 = *ptr & CAN_IF1ARB2_ID_M; // smaller part

	_println("id>\t[0x%4X]::[0x%8X]", status, msg_struct.ui32MsgID);
	_println("dir>\t%4X+%4X\t[0x%8X]", id1, id2, (id2 << 13) | id1);

//	for (int i = 0; i < 8; i++) _printf("%2X", msg_buffer[i]);
//	_puts("<");

	CANIntDisable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_STATUS);
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
	const uint32 object = object_id_get();
	const uint32 status = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE);
	if (status == CAN_INT_INTID_STATUS) {
		_test();
//		CanTransmissionInterruptAttachmentCall();
	} else if (status == object) {
		_putchar('*');  // debug check
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
	SysCtlDelay(100);

	msg_buf_init();
}
