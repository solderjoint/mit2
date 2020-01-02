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
#define PRINT_BUFLEN_MAX  (512) // maximum output buffer size
#define PRINT_INT_BUFLEN  (64)  // output integer bufer size

#define PRINT_NEWLINE_WIN // output CRLF on newline
//#define PRINT_NEWLINE_MAC // output LF on newline
#define PRINT_GETS_ECHO   // echo back get characters

/* **************************************************** *
 *             PRINTOUT FUNCTION PROTOTYPES
 * **************************************************** */
//#define PRINT_OUTPUT_STREAM(FUN) _out_stream = (void(*)(uint8))(FUN)
//extern void (*_out_stream) (uint8);

void _putchar (uint8 c);
int32 _print (const uint8 buf[]);
int32 _puts (const uint8 buf[]);

void _fputchar (void (*stream) (uint8), uint8 c);
int32 _fputs (void (*stream) (uint8), const uint8 buf[]);


void _printf (const char* fmt, ...);
void _println (const char* fmt, ...);

void _fprintf (void (*func) (uint8), const char* fmt, ...);
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