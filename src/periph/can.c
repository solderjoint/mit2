
#include "periph/can.h"

#include <stdbool.h>
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
#include "util/debug.h"

/* **************************************************** *
 *                 CAN LOCAL VARIABLES
 * **************************************************** */
// main can object variable
static tCANMsgObject msg_struct;

static uint32 can_obj_id;  // can msg obj id [1..31]
void object_id_set (const uint32 x) { can_obj_id = x & 0x1F; }
uint32 object_id_get (void) { return can_obj_id; }

//static int8 can_msg_int;   // can interrupt indicator
//static void CanMsgIntSet (void) { can_msg_int = 1; }
//static void CanMsgIntReset (void) { can_msg_int = 0; }
//int32 CanMsgIntGet (void) { return can_msg_int; }

/* **************************************************** *
 *           CAN MESSAGE BUFFER MANIPULATION
 * **************************************************** */
//static struct can_buf_t table[CAN_MSGBUF_SIZEX][CAN_MSGBUF_SIZEY];
//static void CanMessageBufferSet
//(struct can_buf_t input, const uint8 x, const uint8 y) {
//	// make sure offset is within borders
//	const int32 offx = x & ((CAN_MSGBUF_SIZEX - 1) & 0xFF);
//	const int32 offy = y & ((CAN_MSGBUF_SIZEY - 1) & 0xFF);
//	for (int32 i = 0; i < can_msg_struct.ui32MsgLen; i++) {
//		table[offx][offy].msg[i] = input.msg[i];
//	}
//}
//struct can_buf_t CanMessageBufferGet (const uint8 x, const uint8 y) {
//	const int32 offx = x & ((CAN_MSGBUF_SIZEX - 1) & 0xFF);
//	const int32 offy = y & ((CAN_MSGBUF_SIZEY - 1) & 0xFF);
//	return table[offx][offy];  // return correct address
//}

static volatile uint8 msg_buffer[8];

static void CanBufferUpdate (void) {
	msg_struct.pui8MsgData = msg_buffer;
	CANMessageGet(CAN0_BASE, object_id_get(), &msg_struct, 0);

	// check if a message was lost
	if (msg_struct.ui32Flags & MSG_OBJ_DATA_LOST) {
//		_println("info>\tcan msg loss");
	}
}

static void CanBufferInit (void) {
	// set default values for incoming messages
	msg_struct.ui32MsgID = 0;      // empty input id
	msg_struct.ui32MsgIDMask = 0;  // empty mask receives everything
	msg_struct.ui32Flags = MSG_OBJ_RX_INT_ENABLE;
	msg_struct.ui32MsgLen = 8;     // message length is 8 bytes

	object_id_set(1);
//	CanMsgIntSet();
	const uint32 obj = object_id_get();
	CANMessageSet(CAN0_BASE, obj, &msg_struct, MSG_OBJ_TYPE_RX);
}

/* **************************************************** *
 *                CAN INTERRUPT HANDLER
 * **************************************************** */
void _test (void) {
	const uint32 status = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);
	CanBufferUpdate();

	const uint32 *ptr = (CAN0_BASE + CAN_O_IF1ARB1);
	const uint32 id1 = *ptr & CAN_IF1ARB1_ID_M; // bigger part
	const uint32 id2 = *ptr & CAN_IF1ARB2_ID_M; // smaller part

	_println("info>\t[0x%4X]::[0x%4X]", status, msg_struct.ui32MsgID);
	_println("info>\t%4X+%4X\t[0x%8X]", id1, id2, (id2 << 13) | id1);

	for (int i = 0; i < 8; i++) _printf("%2X", msg_buffer[i]);
	_puts("<");

	CANIntDisable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_STATUS);
}

void CanInterruptHandler (void) {
	const uint32 object = object_id_get();
	const uint32 status = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE);

	// main interrupt entry point
	if (status == CAN_INT_INTID_STATUS) {
		_test();
	} else if (status == object) {
		CANIntClear(CAN0_BASE, 1);  // disable following interrupts
		_putchar('*');  // debug check
	} else {
		_println("info>\tcan error[-%i]", __LINE__);
	}
}

/* **************************************************** *
 *                  CAN INITIALIZATION
 * **************************************************** */
void CanTransmissionInit (const uint32_t rate) {
	const uint32 sys_clock = SysCtlClockGet();
	_check ((rate >= 800) && (rate <= sys_clock / 32));

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
	IntPrioritySet(INT_CAN0, 80); // set low interrupt priority
	CANEnable(CAN0_BASE);

	CanBufferInit();
}
