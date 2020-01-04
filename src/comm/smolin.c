
#include "comm/smolin.h"

#include <stdint.h>
#include <string.h>

#include "comm/data/quick.h"
#include "util/print.h"

/* **************************************************** *
 *         SMOLIN PROTOCOL HEADER MANIPULATION
 * **************************************************** */
struct smolin_header_t {
	uint32 _reserved:8;
	uint32 receiver:8;
	uint32 sender:8;
	uint32 text_end:1;
	uint32 quickdata:1;
	uint32 request:1;
	uint32 text_pos:4;
};
static struct smolin_header_t header_struct;

static union obj_u {
	uint32 i;
	struct smolin_header_t s;
} header_union;

static void HeaderSet (const uint32 x) {
	header_union.i = x;
	header_struct = header_union.s;
}
static uint32 HeaderGet (void) {
	header_union.s = header_struct;
	return header_union.i;
}
static uint8 HeaderReceiverGet (void) { return header_struct.receiver; }
static uint8 HeaderSenderGet (void) { return header_struct.sender; }
static uint8 HeaderTextEndBitGet (void) { return header_struct.text_end; }
static uint8 HeaderQuickDataBitGet (void) { return header_struct.quickdata; }
static uint8 HeaderRequestBitGet (void) { return header_struct.request; }
static uint8 HeaderTextPositionGet (void) { return header_struct.text_pos; }

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
static void SmolinDataProcess (void) {
	_println("%u\t", HeaderReceiverGet());
}

/* **************************************************** *
 *           SMOLIN PROTOCOL MAIN ENTRY POINT
 * **************************************************** */
int32 SmolinProtocolProcess
(const uint32 header, const uint32 length, uint8 msg[8]) {
	if (length > 8) return -1;

	HeaderSet(header);
	// check that we got our id
//	const uint32 add_low = 0x20, add_high = 0x27;
//	if (HeaderReceiverGet() == address)
	if (HeaderGet() >= 0) {
		SmolinDataProcess();
	} else {
		// append to received table
	}
}
