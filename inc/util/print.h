/* ****************************************************
 * Universal string handler for user console interface
 * ****************************************************
 * Original license: zero responsibility free software
 * Copyright (C) 2011, ChaN, all right reserved.
 * ****************************************************
 * Modified version: unlicensed 2019 ICT SB RAN
 * ****************************************************
 */

#ifndef __IO_PRINT_H
#define __IO_PRINT_H

#include "util/typedefs.h"

/* **************************************************** *
 *                  DEFINITION MACROS
 * **************************************************** */
enum _printEnum {
	PRINT_BUFLEN_MAX  = 512, // maximum output buffer size
	PRINT_INT_BUFLEN  = 64,  // output integer bufer size
};

/* **************************************************** *
 *             PRINTOUT FUNCTION PROTOTYPES
 * **************************************************** */
//#define PRINT_OUTPUT_STREAM(FUN) _out_stream = (void(*)(uint8))(FUN)
//extern void (*_out_stream) (uint8);

void xputchar (uint8 c);
//int32 xputs (const uint8 buf[]);
int32 xputs (const uint8 buf[]);

void xfputchar (void (*stream) (uint8), uint8 c);
int32 xfputs (void (*stream) (uint8), const uint8 buf[]);


void xprint (const char* fmt, ...);
void xprintln (const char* fmt, ...);

void xfprintf (void (*func) (uint8), const char* fmt, ...);
//void _sprintf (uint8 *buff, const char* fmt, ...);

/* **************************************************** *
 *              INPUT FUNCTION PROTOTYPES
 * **************************************************** */
//#define PRINT_INPUT_STREAM(FUN) _in_stream = (uint8(*)(void))(FUN)
//extern uint8 (*_in_stream) (void);

//int32 _gets (char* buff, int32 len);
//int32 _fgets (uint8 (*func) (void), char* buff, int32 len);
//int32 _atoi (char **buf, int32 *res);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif // __IO_PRINT_H
