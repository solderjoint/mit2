/*
 * Peripherials header for mcu on-board usart
 */

#ifndef __USART_H
#define __USART_H

#include "util/typedefs.h"

/* **************************************************** *
 *                  HEADER SPECIFIC MACROS
 * **************************************************** */
enum _usartEnum {
	USART_INT_PRIORITY = 7,      // lowest priority level
	USART_STRLEN_MAX   = 128,    // maximum input string len
	USART_BUFLEN_MAX   = 1024,   // maximum buffer length
	USART_BAUDRATE_MAX = 960000, // maximum usart baudrate
};

/* **************************************************** *
 *                 FUNCTION PROTOTYPES
 * **************************************************** */
void UsartConsoleInit (const uint32 rate);

int32 UsartConsoleSendNonBlocking (uint8 buf[], const uint32 len);
int32 UsartConsoleSendCharNonBlocking (uint8 tx);

void UsartConsolePutchar (uint8 symbol);
uint32 UsartConsoleSendBlocking (uint8 buf[], const uint32 len);

uint32 UsartConsoleScan (uint8 buf[], const uint32 len);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__USART_H
