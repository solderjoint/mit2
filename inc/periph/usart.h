/*
 * Peripherials header for mcu on-board usart
 */

#ifndef __USART_H
#define __USART_H

#include "util/typedefs.h"
#include "util/util.h"

/* **************************************************** *
 *                  HEADER SPECIFIC MACROS
 * **************************************************** */
#define USART_STRLEN_MAX    (128)    // maximum input string len
#define USART_BUFLEN_MAX    (512)    // maximum buffer length

#define USART_BAUDRATE_DEF  (115200) // default usart rate
#define USART_BAUDRATE_MIN  (300)    // minimum usart baudrate
#define USART_BAUDRATE_MAX  (512000) // maximum usart baudrate

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
