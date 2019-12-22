
#include "util/debug.h"

//#include <stdarg.h>
//#include <stddef.h>

//#include "periph/usart.h"

///* **************************************************** *
// *            RING BUFFER FUNCTION WRAPPERS
// * **************************************************** */
//inline static int8 _buf_check (_buf_struct *buf) {
//	if (buf == NULL) return -1;
//	else if (buf->elems == NULL) return -2;

//	if (buf->end < 0) return -11;
//	if (buf->size < 0) return -12;
//	if (buf->start < 0) return -13;

//	if (buf->start >= buf->end) return -20;
//	return 0;
//}

///* **************************************************** */
//inline static int8 _buf_push (_buf_struct *buf, char symbol) {
//	// push symbol to a rotating ring buffer
////	if (buf == NULL) return -1;
////	else if (buf->elems == NULL) return -2;

//	buf->buffer[buf->end] = symbol;
//	buf->end = (buf->end + 1) % (_RING_LENGTH_MAX + 1);
//	if (buf->start == buf->end) {
//		buf->start = (buf->start + 1) % (buf->size + 1);
//	}
//	return 0;
//}

//static int32 _buf_pushstr (_buf_struct *buf, char str[], int32 len) {
//	// push a string to a ring buffer
//	if (buf == NULL) return -1;
//	else if (buf->elems == NULL) return -2;

//	if (len < 1) return -11; // empty string check
//	if (len > _RING_LENGTH_MAX) return -11;
////	if (len >= buf->size) return -12;
//	const int32 int_len = (len <= buf->size)? len : buf->size;

//	int32 count;
//	for (count = 0; count < int_len; count++) {
//		buf->buffer[buf->end] = str[count];
//		buf->end = (buf->end + 1) % (_RING_LENGTH_MAX + 1);
//		if (buf->start == buf->end) {
//			buf->start = (buf->start + 1) % (buf->size + 1);
//		}
//	}
//	return count;
//}
///* **************************************************** */
//inline static char _buf_pop (_buf_struct *buf) {
//	// pop a symbol from a ring buffer
////	if (buf == NULL) return -1;
////	else if (buf->elems == NULL) return -2;

//	const char symbol = buf->elems[buf->start];
//	buf->start = (buf->start + 1) % (buf->size + 1);
//	return symbol;
//}

//static int32 _buf_popstr (_buf_struct *buf, char str[], int32 len) {
//	// pop whole string from a ring buffer
//	if (buf == NULL) return -1;
//	else if (buf->elems == NULL) return -2;

//	if (len < 1) return -11; // empty string check
//	if (len > _RING_LENGTH_MAX) return -11;
////	if (len >= buf->size) return -12;
//	const int32 int_len = (len <= buf->size)? len : buf->size;

//	int32 count;
//	for (count = 0; count < int_len; count++) {
//		str[count] = buf->elems[buf->start];
//		buf->start = (buf->start + 1) % (buf->size + 1);
//	}
//	return count;
//}

///* **************************************************** *
// *                 FUNCTION DECLARATIONS
// * **************************************************** */
////int32 debug_println (char *fmt, ...) {
//int32 _println (char *fmt, ...) {
//	char *buf;
//	buf = (char *) malloc (96);
//	if (buf == NULL) return -1;

//	snprintf(buf, 96, fmt, __VA_ARGS__);
//	const int32 count = UsartWriteStrLen(_DEBUG_UART_PORT, buf, 96);

//	free (buf);
//	return count;
//}

////int32 debug_vprintln (char *fmt, ...) {
////	va_list vlist;
////	va_start (vlist, fmt);

////	int i;
////	for(i=0; i<num; ++i) {
////		printf ("%d\n", va_arg(vlist, int));
////	}

////	va_end (vlist);//clean up memory
////}


////#define _println(STR,...) \
////		snprintf(gPrintBuf, 112, (STR), __VA_ARGS__); \
////			UsartWriteStrLen(CONSOLE, gPrintBuf, 112); \
////			UsartWriteChar (CONSOLE, '\n')


