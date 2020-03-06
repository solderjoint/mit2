
#include "logic/comm/smolin.h"

#include "logic/data/quick.h"
//#include "logic/data/longword.h"
#include "logic/comm/mbus.h"

#include "vars/canmessage.h"

#include "util/print.h"
#include "util/util.h"

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
 *           SMOLIN INCOMING MESSAGE WRAPPERS
 * **************************************************** */
static volatile uint8 input_buffer[CAN_MSGBUF_LEN];
static volatile int32 input_header;

inline static void InputHeaderSet (const int32 x) {
	input_header = x & HEADER_FULL_MSG;
}
inline static int32 InputHeaderModbusDataGet (void) {
	return ((input_header >> HEADER_MBUSDATA) & 0x01);
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

/* **************************************************** *
 *           SMOLIN OUTGOING MESSAGE WRAPPERS
 * **************************************************** */
static volatile uint8 output_buffer[CAN_MSGBUF_LEN];
static volatile int32 output_header;

inline static void OutputHeaderClear (void) {
	output_header = 0;
}
inline static int32 OutputHeaderModbusDataSet (const int32 x) {
	output_header |= ((x & 0x02) << HEADER_MBUSDATA);
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

/* **************************************************** *
 *              SMOLIN PROTOCOL UTILITIES
 * **************************************************** */
inline static void smolin_send (void) {
	CanMessageSenderIdSet(output_header);
	CanMessageSenderBufferSet(output_buffer);
	CanMessageSend();
}

inline static void SmolinProtocolQuickSend (void) {
	OutputHeaderClear();
	OutputHeaderIdFromSet (InputHeaderIdDestGet());
	OutputHeaderIdDestSet (InputHeaderIdFromGet());
	OutputHeaderRequestSet (1);
	smolin_send();  // call sending wrapper
}

inline static void SmolinProtocolMbusSend (void) {
	OutputHeaderClear();
	OutputHeaderIdFromSet (InputHeaderIdDestGet());
	OutputHeaderIdDestSet (InputHeaderIdFromGet());
	OutputHeaderModbusDataSet(0xF);
	smolin_send();  // call sending wrapper
}

/* **************************************************** *
 *           SMOLIN PROTOCOL INCOMING HANDLER
 * **************************************************** */
inline void SmolinProtocolProcessIncoming (void) {
	CanMessageReceive();  // update incoming message
	InputHeaderSet (CanMessageReceiverIdGet());
	// protect buffer from overwriting by copying it
	const uint8 *ptr = CanMessageReceiverBufferGet();
	const int32 size = CanMessageReceiverSizeGet() % 9;  // <= 8
	for (int i = 0; i < size; i++) input_buffer[i] = ptr[i];

	const int32 dev_id = CanMessageDeviceIdGet();
	if (InputHeaderIdDestGet() == dev_id) {
		if (InputHeaderFastDataGet()) {
//			if (size > 0) QuickInputProcess(input_buffer);
			QuickOutputProcess(output_buffer);
			SmolinProtocolQuickSend();
		} else if (InputHeaderModbusDataGet()) {
			ModbusMessageProcess(input_buffer, output_buffer);
			SmolinProtocolMbusSend();
		}
	}
}

/* **************************************************** *
 *           SMOLIN PROTOCOL OUTGOING HANDLER
 * **************************************************** */
inline void SmolinProtocolProcessOutgoing (void) {
	// empty stub
}
