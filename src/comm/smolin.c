
#include "comm/smolin.h"

#include <stdint.h>
#include <string.h>

#include "comm/data/quick.h"

#include "util/print.h"
#include "util/util.h"

/* **************************************************** *
 *         SMOLIN PROTOCOL HEADER MANIPULATION
 * **************************************************** */
//static union obj_u {
//	uint32 i;
//	struct smolin_header_t s;
//} header_union;

//static void HeaderSet (const uint32 x) {
//	header_union.i = x;
//	header_struct = header_union.s;
//}
//static uint32 HeaderGet (void) {
//	header_union.s = header_struct;
//	return header_union.i;
//}

/* **************************************************** *
 *          CAN TOTAL MESSAGES RECEIVED TABLE
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
//
//struct can_buf_t CanMessageBufferGet (const uint8 x, const uint8 y) {
//	const int32 offx = x & ((CAN_MSGBUF_SIZEX - 1) & 0xFF);
//	const int32 offy = y & ((CAN_MSGBUF_SIZEY - 1) & 0xFF);
//	return table[offx][offy];  // return correct address
//}

/* **************************************************** *
 *                SMOLIN DATA PROCESSING
 * **************************************************** */
static void SmolinDataProcess (uint8 msg[8]) {
	_println("%u\t", HeaderReceiverGet());
}

/* **************************************************** *
 *           SMOLIN PROTOCOL MAIN ENTRY POINT
 * **************************************************** */
int32 SmolinProtocolProcess
(const uint32 header, const uint32 length, uint8 msg[8]) {
	_check(length <= 8);

	HeaderSet(header);
	// check that we got our id
//	const uint32 add_low = 0x20, add_high = 0x27;
//	if (HeaderReceiverGet() == address)
	if (HeaderGet() >= 0) {
		SmolinDataProcess(msg);
	} else {
		// append to received table
	}
}

/* **************************************************** *
 *            SMOLIN PROTOCOL ENTRY WRAPPERS
 * **************************************************** */
//#include "periph/can.h"
#include "periph/gpio.h"

#include "vars/canmessage.h"
#include "util/print.h"

/* **************************************************** *
 *                SMOLIN HEADER WRAPPERS
 * **************************************************** */
static uint8 input_buffer[CAN_MSGBUF_LEN];
static int32 input_header;

inline static void InputHeaderSet (const int32 x) {
	input_header = x & HEADER_FULL_MSG;
}
inline static int32 InputHeaderIdFromGet (void) {
	return ((input_header >> HEADER_ID_FROM) & 0xFF);
}
inline static int32 InputHeaderIdDestGet (void) {
	return ((input_header >> HEADER_ID_DEST) & 0xFF);
}
inline static int32 InputHeaderTextEndGet (void) {
	return ((input_header >> HEADER_TEXTEND) & 0x01);
}
inline static int32 InputHeaderFastDataGet (void) {
	return ((input_header >> HEADER_FASTDATA) & 0x01);
}
inline static int32 InputHeaderRequestGet (void) {
	return ((input_header >> HEADER_REQUEST) & 0x01);
}
inline static int32 InputHeaderTextPosGet (void) {
	return ((input_header >> HEADER_TEXTPOS) & 0x1F);
}

/* **************************************************** */
static uint8 output_buffer[CAN_MSGBUF_LEN];
static int32 output_header;

inline static void OutputHeaderClear (void) {
	output_header = 0;
}
inline static void OutputHeaderIdFromSet (const int32 x) {
	output_header |= ((x & 0xFF) << HEADER_ID_FROM);
}
inline static void OutputHeaderIdDestSet (const int32 x) {
	output_header |= ((x & 0xFF) << HEADER_ID_DEST);
}
inline static void OutputHeaderTextEndSet (const int32 x) {
	output_header |= ((x & 0x01) << HEADER_TEXTEND);
}
inline static void OutputHeaderFastDataSet (const int32 x) {
	output_header |= ((x & 0x01) << HEADER_FASTDATA);
}
inline static void OutputHeaderRequestSet (const int32 x) {
	output_header |= ((x & 0x01) << HEADER_REQUEST);
}
inline static void OutputHeaderTextPosSet (const int32 x) {
	output_header |= ((x & 0x1F) << HEADER_TEXTPOS);
}

/* **************************************************** */
static int32 counter = 0;

inline void SmolinProtocolProcessIncoming (void) {
	// wrapper for incoming message processing
	CanMessageReceive();  // update incoming message
	InputHeaderSet (CanMessageReceiverIdGet());

	// protect buffer from overwriting by copying it
	const uint8 *ptr = CanMessageReceiverBufferGet();
	const int32 size = CanMessageReceiverSizeGet ();
	for (int i = 0; i < size; i++) input_buffer[i] = ptr[i];

	// using hard '4' instea of real result from hard-coded pins
	const int32 device_id = GpioModuleAdressGet () \
			| (4 << 3) /*(GpioModuleCodenameGet () << 3)*/;

	if (InputHeaderIdDestGet() == device_id) {
		if (InputHeaderFastDataGet()) {
			QuickInputProcess(input_buffer);
			QuickOutputProcess(output_buffer);

			OutputHeaderClear();
			OutputHeaderIdFromSet (device_id);
			OutputHeaderIdDestSet (InputHeaderIdFromGet ());
			OutputHeaderRequestSet (1);

			CanMessageSenderIdSet(output_header);
			CanMessageSenderBufferSet(output_buffer);
			CanMessageSend();
		}
		GpioLedsSet (2, -1);
	}

//	for (int i = 0; i < CAN_MSGBUF_LEN; i++) {
//		output_buffer[i] = ((uint8) counter);
//	}  counter++;
}

/* **************************************************** *
 *            SMOLIN PROTOCOL ENTRY WRAPPERS
 * **************************************************** */

inline void SmolinProtocolProcessOutgoing (void) {
	// empty stub
}
