
#include "comm/smolin.h"

#include <stdint.h>
#include <string.h>

#include "comm/data/quick.h"
#include "util/print.h"

/* **************************************************** *
 *         SMOLIN PROTOCOL HEADER MANIPULATION
 * **************************************************** */
static union obj_u {
	uint32 i;
	struct smolin_header_t s;
} _header_union;

// message header variable
static struct smolin_header_t _header;

static void HeaderSet (const uint32 x) {
	_header_union.i = x;
	_header = _header_union.s;
}
static uint32 HeaderGet (void) {
	_header_union.s = _header;
	return _header_union.i;
}
static uint8 HeaderReceiverGet (void) { return _header.receiver; }
static uint8 HeaderSenderGet (void) { return _header.sender; }
static uint8 HeaderTextEndBitGet (void) { return _header.text_end; }
static uint8 HeaderQuickDataBitGet (void) { return _header.quickdata; }
static uint8 HeaderRequestBitGet (void) { return _header.request; }
static uint8 HeaderTextPositionGet (void) { return _header.text_pos; }

/* **************************************************** *
 *                  CAN INITIALIZATION
 * **************************************************** */
static void SmolinQuickDataProcess (void) {

}

/* **************************************************** *
 *           SMOLIN PROTOCOL MAIN ENTRY POINT
 * **************************************************** */
inline int32 SmolinProtocolProcess
(const uint32 header, const uint32 length, uint8 msg[8]) {
	if (length > 8) return -1;

	HeaderSet(header);
//	_putchar('.');
//	if ((HeaderReceiverGet() >= 0x20) && (HeaderReceiverGet() >= 0x27)) {
	if (HeaderReceiverGet() >= 0) {
		_printf("info>\t0x");
		for (int i = 0; i < 8; i++) _printf("%02X", msg[i]);
	}

//	if (HeaderQuickDataBitGet() > 0) {
//		SmolinQuickDataProcess();
//	} else {
//		//
//	}
}
