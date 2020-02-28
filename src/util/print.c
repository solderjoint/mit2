
#include "util/print.h"

#include <stddef.h> // null ref
#include <stdarg.h>

#include "periph/usart.h"

/* **************************************************** *
 *                    LOCAL DEFINES
 * **************************************************** */
#define PRINT_GETS_ECHO   // echo back get characters
#define PRINT_NEWLINE_WIN // output CRLF on newline
//#define PRINT_NONBLOCKING // non-blocking function call

/* **************************************************** *
 *             PRINTOUT FUNCTION PROTOTYPES
 * **************************************************** */
void xputchar (uint8 c);
int32 xputs (const uint8 buf[]);

void xfputchar (void (*stream) (uint8), uint8 c);
int32 xfputs (void (*stream) (uint8), const uint8 buf[]);

static void _vprintf (void (*stream) (uint8), const char* fmt, va_list arp);

/* **************************************************** *
 *                SIMPLE OUTPUT WRAPPERS
 * **************************************************** */
inline void xputchar (uint8 c) {
#ifdef PRINT_NEWLINE_WIN
	if (c == '\n') xputchar('\r');
#endif

#ifdef PRINT_NONBLOCKING
	UartConsoleSendCharNonBlocking (c);
#else
	UartConsolePutchar(c);
#endif
}

int32 xputs (const uint8 buf[]) {
	int32 i;
	for (i = 0; (i < PRINT_BUFLEN_MAX) && (buf[i]); i++) xputchar(buf[i]);
	xputchar('\n');

	return i + 1;
}

/* **************************************************** *
 *          SELECTABLE STREAM OUTPUT WRAPPERS
 * **************************************************** */
inline void xfputchar (void (*stream) (uint8), uint8 c) {
	if (*stream == NULL) return;
#ifdef PRINT_NEWLINE_WIN
	if (c == '\n') xfputchar(*stream, '\r');
#endif
	stream(c);
}

int32 xfputs (void (*stream) (uint8), const uint8 buf[]) {
	if (*stream == NULL) return -1;
	int32 i;
	for (i = 0; (i < PRINT_BUFLEN_MAX) && (buf[i]); i++)
		xfputchar(*stream, buf[i]);
	return i;
}

/* **************************************************** *
 *           INTERNAL FORMATTED STRING OUTPUT
 * **************************************************** */
void xprint (const char* fmt, ...) {
	va_list arp;
	va_start(arp, fmt);
	_vprintf(UartConsolePutchar, fmt, arp); // main function call
	va_end(arp);
}
/* **************************************************** */
void xprintln (const char* fmt, ...) {
	va_list arp;
	va_start(arp, fmt);
	_vprintf(UartConsolePutchar, fmt, arp); // main function call
	va_end(arp);
	xputchar('\n');
}
/* **************************************************** */
void xfprintf (void (*func) (uint8), const char* fmt, ...) {
	if (*func == NULL) return;

	va_list arp;
	va_start(arp, fmt);
	_vprintf(*func, fmt, arp);
	va_end(arp);
}
/* **************************************************** */
//void _snprintf (uint8 buf[], const uint32 size, const char* fmt, ...) {
//	// switch memory destination
//	outptr = storage;

//	va_list arp;
//	va_start(arp, fmt);
//	_vprintf(UartConsolePutchar(uint8), fmt, arp);
//	va_end(arp);

//	*outptr = '0';      // terminate with null character
//	outptr = 0;         // switch ptr destination
//}

/* **************************************************** *
 *           INTERNAL FORMATTED STRING OUTPUT
 * **************************************************** */
static int8 _vprint_str[PRINT_INT_BUFLEN];

static void _vprintf (void (*stream) (uint8), const char* fmt, va_list arp) {
	if (*stream == NULL) return; // check that input function is not null

	unsigned int bits, i, j;
	uint32 val;
	uint32 flag = 0;
	uint32 width = 0;
	uint8 type;
	uint8 *p;
	int8 c;

	while(1) {
		// get next char and check formatting
		c = *fmt++;
		if (!c) break;
		if (c != '%') {
			xfputchar(*stream, c);
			continue;
		}

		// get justification and 0 fill
		c = *fmt++;
		if (c == '0') {
			flag = 1;
			c = *fmt++;
		} else {
			if (c == '-') {			// Flag: left justified
				flag = 2;
				c = *fmt++;
			}
		}

		// get minimum width
		for (; c >= '0' && c <= '9'; c = *fmt++)
			width = width * 10 + c - '0';
		// get prefix
		if (c == 'l' || c == 'L') {
			flag |= 4;
			c = *fmt++;
		}

		// convert 'a' to 'A'
		if (!c) break;
		type = c;
		if (type >= 'a') type -= 0x20;

		// check type of argument
		switch (type) {
		case 'S' :  // string
			p = va_arg(arp, uint8*);
			for (j = 0; p[j]; j++) ;
			while (!(flag & 2) && j++ < width)
				xfputchar(*stream, ' ');
			xfputs(*stream, p);
			while (j++ < width)
				xfputchar(*stream, ' ');
			continue;

		case 'C' :  // symbol
			xfputchar(*stream, (uint8)va_arg(arp, int));
			continue;

		case 'I' :  // simple integer variable
		case 'D' :  // signed decimal
		case 'U' :  // unsigned decimal
			bits = 10;
			break;

		case 'X' :  // hexadecimal
			bits = 16;
			break;

		case 'B' :  // binary digit
			bits = 2;
			break;

		case 'O' :  // octal digit
			bits = 8;
			break;

		default :   // unknown
			xfputchar(*stream, c);
			continue;
		}

		// pseudo long conversion
		val = (flag & 4) ? \
				va_arg(arp, int64) : ((type == 'D') ? \
				(int64)va_arg(arp, int32) : (int64)va_arg(arp, uint32));

		if (type == 'D' && (val & (1 << 27))) {
			val = 0 - val;
			flag |= 8;
		}

		i = 0;   //
		do {
			type = (char)(val % bits);
			val /= bits;
			if (type > 9)
				type += (c == 'x') ? 0x27 : 0x07;
			_vprint_str[i++] = type + '0';
		} while (val && i < sizeof(_vprint_str));

		if (flag & 8) _vprint_str[i++] = '-';
		j = i;
		type = (flag & 1) ? '0' : ' ';
		while (!(flag & 2) && j++ < width)
			xfputchar(*stream, type);

		do {
			xfputchar(*stream, _vprint_str[--i]);
		} while(i);

		while (j++ < width)
			xfputchar(*stream, ' ');
	}
}

/* **************************************************** *
 *                INPUT LOCAL VARIABLES
 * **************************************************** */
//uint8 (*_in_stream) (void); // input stream pointer

//static void _input_echo (uint8 c) {
//#ifdef PRINT_GETS_ECHO
//	_putc(c);
//#endif
//	return;
//}

/* **************************************************** *
 *             SIMPLE BUFFER INPUT ROUTINE
 * **************************************************** */
//int32 _gets (char* buff, int32 len) {
//	// check that len is within bounds and stream function is not null
//	if ((len > 0) && (len < PRINT_BUFLEN_MAX)) return -1;
//	if (!_in_stream) return -2;

//	int32 i;
//	for (i = 0; ; ) {
//		const int32 c = _in_stream();
//		if (!c) return 0;
//		if ((c == '\r') || (c == '\n')) break;
//		if (c == '\b' && i) {
//			i--;
//			_input_echo(c);
//			continue;
//		}
//		if (c >= ' ' && i < len - 1) {
//			buff[i++] = c;
//			_input_echo(c);
//		}
//	}
//	buff[i] = 0;    // terminate resulting buffer with null
//	_input_echo('\n');
//	return 1;
//}

///* **************************************************** *
// *           SEPARATE FUNCTIONS INPUT WRAPPER
// * **************************************************** */
//int32 _fgets (uint8 (*func) (void), char* buff, int32 len) {
//	unsigned char (*pf)(void);
//	pf = _in_stream;    // swap input streams
//	_in_stream = func;
//	const int32 n = _gets(buff, len);
//	_in_stream = pf;    // restore input device pointer
//	return n;
//}

///* **************************************************** *
// *          INTEGER VALUE INPUT SCAN FUNCTION
// * **************************************************** */
//int32 _atoi (char **buf, int32 *res) {
//	// set value of a storage to zero
//	*res = 0;

//	uint8 c, bits, s = 0;
//	while ((c = **buf) == ' ') (*buf)++; // skip spaces

//	if (c == '-') {
//		s = 1;
//		c = *(++(*buf));
//	}

//	uint32 val;
//	if (c == '0') {
//		c = *(++(*buf));
//		switch (c) {
//		case 'x':   // hexadecimal
//			bits = 16; c = *(++(*buf));
//			break;
//		case 'b':   // binary
//			bits = 2; c = *(++(*buf));
//			break;
//		default:    // octal?
//			if (c <= ' ') return 1;  // simply a zero
//			if (c < '0' || c > '9') return 0; // not a digit
//			else bits = 8;
//		}
//	} else {
//		if (c < '0' || c > '9') return 0;
//		bits = 10;  // decimal
//	}

//	val = 0;
//	while (c > ' ') {
//		if (c >= 'a') c -= 0x20;

//		c -= '0';
//		if (c >= 17) {
//			c -= 7;
//			if (c <= 9) return 0; // invalid utility chars
//		}

//		if (c >= bits) return 0;  // too big number for current bitset
//		val = val * bits + c;
//		c = *(++(*buf));
//	}
//	if (s) val = -val;         // apply sign if needed
//	*res = val;
//	return 1;
//}
