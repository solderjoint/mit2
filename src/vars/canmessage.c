﻿
#include "vars/canmessage.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include <tivaware/inc/hw_can.h>
#include <tivaware/inc/hw_ints.h>
#include <tivaware/inc/hw_memmap.h>

#include <tivaware/driverlib/can.h>
#include <tivaware/driverlib/interrupt.h>
#include <tivaware/driverlib/pin_map.h>
#include <tivaware/driverlib/sysctl.h>

#include "periph/gpio.h"
//#include "util/print.h"
#include "util/util.h"

/* **************************************************** *
 *             CAN MESSAGE RECEVING BUFFER
 * **************************************************** */
static volatile char receive_flag;
inline char CanMessageReceiverFlagGet (void) { return receive_flag; }
inline void CanMessageReceiverFlagSet (void) { receive_flag = 1; }
inline void CanMessageReceiverFlagClear (void) { receive_flag = 0; }

static volatile char sender_flag;
inline char CanMessageSenderFlagGet (void) { return sender_flag; }
inline void CanMessageSenderFlagSet (void) { sender_flag = 1; }
inline void CanMessageSenderFlagClear (void) { sender_flag = 0; }

/* **************************************************** *
 *             CAN MESSAGE RECEVING BUFFER
 * **************************************************** */
static volatile tCANMsgObject recv_struct;
volatile uint8 recv_buffer[CAN_MSGBUF_LEN];

inline void CanMessageReceive (void) {
	// updates incoming message buffer
	CANMessageGet(CAN0_BASE, CAN_RECV_OBJ_ID, &recv_struct, false);
}

uint8* CanMessageReceiverBufferGet (void) {
	// returns pointer to the buffer
	return recv_buffer;
}

inline int32 CanMessageReceiverIdGet (void) {
	// returns 29-bit length incoming message id
	const int32 id = recv_struct.ui32MsgID;
	return (id & 0x1FFFFFFF);
}

inline int32 CanMessageReceiverSizeGet (void) {
	// returns size of incoming buffer
	const int32 size = recv_struct.ui32MsgLen;
	return (size > 8)? -1 : size;
}

/* **************************************************** */
static void CanMessageReceiverInit (void) {
	for (int i = 0; i < CAN_MSGBUF_LEN; i++) recv_buffer[i] = 0;

	// setting (empty mask + id filter) correctly accepts
	// incoming messages, otherwise nothing will be received
	recv_struct.pui8MsgData = recv_buffer;
	recv_struct.ui32MsgLen = CAN_MSGBUF_LEN;
	recv_struct.ui32MsgID = 0xABCDEF; // workaround for 29-bits wide id
	recv_struct.ui32MsgIDMask = 0;    // empty mask receives everything
	recv_struct.ui32Flags = MSG_OBJ_RX_INT_ENABLE \
			| MSG_OBJ_EXTENDED_ID | MSG_OBJ_USE_ID_FILTER;

	CANMessageSet(CAN0_BASE, CAN_RECV_OBJ_ID, &recv_struct, MSG_OBJ_TYPE_RX);
}

/* **************************************************** *
 *              CAN MESSAGE SENDING BUFFER
 * **************************************************** */
static volatile tCANMsgObject send_struct;
volatile uint8 send_buffer[CAN_MSGBUF_LEN];

inline void CanMessageSend (void) {
	// sends a message from buffer
	CANMessageSet (CAN0_BASE, CAN_SEND_OBJ_ID, &send_struct, MSG_OBJ_TYPE_TX);
}

void CanMessageSenderBufferSet (const uint8 buffer[CAN_MSGBUF_LEN]) {
	// copying a message, then a pointer to buffer
	for (int i = 0; i < CAN_MSGBUF_LEN; i++) send_buffer[i] = buffer[i];
}

inline void CanMessageSenderIdSet (const int32 id) {
	// sets 29-bit length outgoing message id
	send_struct.ui32MsgID = id & 0x1FFFFFFF;
}

static void CanMessageSenderInit (void) {
	for (int i = 0; i < CAN_MSGBUF_LEN; i++) send_buffer[i] = 0;

	send_struct.pui8MsgData = send_buffer;
	send_struct.ui32MsgID = 0x123456; // workaround for 29-bits wide id
	send_struct.ui32MsgIDMask = 0;    // empty mask receives everything
	send_struct.ui32Flags = /*MSG_OBJ_TX_INT_ENABLE |*/ MSG_OBJ_EXTENDED_ID;
	send_struct.ui32MsgLen = CAN_MSGBUF_LEN;

	CANMessageSet(CAN0_BASE, CAN_SEND_OBJ_ID, &send_struct, MSG_OBJ_TYPE_TX);
}

/* **************************************************** *
 *         CAN MESSAGE LOCAL DEVICE ID WRAPPER
 * **************************************************** */
static int32 dev_id = 0;
int32 CanMessageDeviceIdGet (void) { return dev_id & 0xFF; }
void CanMessageDeviceIdSet (const int32 id) { dev_id = id & 0xFF; }

/* **************************************************** */
void CanMessageInit (void) {
	// using hard '32' offset instead of hard-coded pins
	const int32 pins = GpioModuleAdressGet () \
			| (4 << 3) /*(GpioModuleCodenameGet () << 3)*/;

	CanMessageDeviceIdSet(pins);
	CanMessageReceiverInit();
	CanMessageSenderInit();
}
