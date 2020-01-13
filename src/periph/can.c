
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
 *             CAN MESSAGE RECEVING BUFFER
 * **************************************************** */
static volatile tCANMsgObject _msg_recv_s;

static int32 _msg_recv_objid;
void msg_recv_id_set (const int32 x) { _msg_recv_objid = x & 0x1F; }
int32 msg_recv_id_get (void) { return _msg_recv_objid; }

static volatile uint8 msg_recv_buffer[CAN_MSGBUF_LEN];

inline static void msg_recv_update (void) {
	CANMessageGet(CAN0_BASE, msg_recv_id_get(), &_msg_recv_s, true);
}

static void msg_recv_init (const int32 obj_id) {
	for (int i = 0; i < CAN_MSGBUF_LEN; i++) msg_recv_buffer[i] = 0;

	_msg_recv_s.pui8MsgData = msg_recv_buffer;
	_msg_recv_s.ui32MsgID = 0xFFFF; // workaround for 29-bits wide id
	_msg_recv_s.ui32MsgIDMask = 0;  // empty mask receives everything
	_msg_recv_s.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_EXTENDED_ID;
	_msg_recv_s.ui32MsgLen = CAN_MSGBUF_LEN;

	msg_recv_id_set (obj_id);
}

/* **************************************************** *
 *              CAN MESSAGE SENDING BUFFER
 * **************************************************** */
static volatile tCANMsgObject _msg_send_s;

static int32 _msg_send_objid;
void msg_send_id_set (const int32 x) { _msg_send_objid = x & 0x1F; }
int32 msg_send_id_get (void) { return _msg_send_objid; }

static volatile uint8 msg_send_buffer[CAN_MSGBUF_LEN];

inline static void msg_send_update (void) {
	CANMessageSet (CAN0_BASE, msg_send_id_get(), &_msg_send_s, MSG_OBJ_TYPE_TX);
}

static void msg_send_init (const int32 obj_id) {
	for (int i = 0; i < CAN_MSGBUF_LEN; i++) msg_send_buffer[i] = 8;

	_msg_send_s.pui8MsgData = msg_send_buffer;
	_msg_send_s.ui32MsgID = 0xFFFF; // workaround for 29-bits wide id
	_msg_send_s.ui32MsgIDMask = 0;  // empty mask receives everything
	_msg_send_s.ui32Flags = /*MSG_OBJ_TX_INT_ENABLE |*/ MSG_OBJ_EXTENDED_ID;
	_msg_send_s.ui32MsgLen = CAN_MSGBUF_LEN;

	msg_send_id_set (obj_id);
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
		msg_buf_receive(obj_id);
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
	uint32 status = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE);
	uint32 object = 0; // empty obj_id holder

	if (status == CAN_INT_INTID_STATUS) {
		status = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);

		// check which interrupt we got
		if (status & CAN_STATUS_RXOK) {
			object = msg_recv_id_get ();
			msg_recv_update ();
			msg_send_update ();
		} else if (status & CAN_STATUS_TXOK) {
			object = msg_send_id_get ();
			msg_send_update ();
		}
//		CanTransmissionInterruptAttachmentCall();
		CANIntClear(CAN0_BASE, object);
	} else {
		GpioLedsSet(3, -1);
		CANIntClear(CAN0_BASE, CAN_INT_INTID_STATUS);
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
	CANIntEnable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_STATUS);
	IntEnable(INT_CAN0);
	IntPrioritySet(INT_CAN0, (CAN_INT_PRIORITY << 5));
	CANEnable(CAN0_BASE);
//	SysCtlDelay(100);

	msg_recv_init(CAN_RECV_OBJ_ID);
	msg_send_init(CAN_SEND_OBJ_ID);
}
