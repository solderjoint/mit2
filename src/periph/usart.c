
#include "periph/usart.h"

#include <stdbool.h>

#include <tivaware/inc/hw_gpio.h>
#include <tivaware/inc/hw_ints.h>
#include <tivaware/inc/hw_memmap.h>

#include <tivaware/driverlib/gpio.h>
#include <tivaware/driverlib/interrupt.h>
#include <tivaware/driverlib/pin_map.h>
#include <tivaware/driverlib/sysctl.h>
#include <tivaware/driverlib/uart.h>

/* **************************************************** *
 *          USART CONSOLE RING BUFFER VARIABLE
 * **************************************************** */
static struct {
	volatile uint16 end;
	volatile uint16 size;
	volatile uint16 start;
	uint8 buf[USART_BUFLEN_MAX];
} usartConsoleBuffer;

uint8 UsartConsoleBufferGet (void) {
	const uint32 ptr = usartConsoleBuffer.start;
	const uint8 symbol = usartConsoleBuffer.buf[ptr];
	// assign next pointer
	const uint32 size = usartConsoleBuffer.size;
	const uint32 next = (ptr + 1) % (size + 1);
	usartConsoleBuffer.start = next;
	return symbol;
}

void UsartConsoleBufferSet (uint8 symbol) {
	const uint32 end = usartConsoleBuffer.end;
	usartConsoleBuffer.buf[end] = symbol;
	// assign new end point
	const uint32 size = usartConsoleBuffer.size;
	const uint32 prev = (end + 1) % (size + 1);
	usartConsoleBuffer.end = prev;
	// check if end == start point
	const uint32 ptr = usartConsoleBuffer.start;
	if (ptr == end) usartConsoleBuffer.start = (ptr + 1) % (size + 1);
}

/* **************************************************** *
 *          USART CONSOLE WRITE/READ WRAPPERS
 * **************************************************** */
void UsartConsoleInterrupt (void) {
	if (UARTIntStatus(UART0_BASE, true) == UART_INT_TX) {
		const uint8 tx = UsartConsoleBufferGet();
		if (tx) UARTCharPut(UART0_BASE, tx);
		UARTIntClear(UART0_BASE, UART_INT_TX);
	}
}

inline int32 UsartConsoleSendCharNonBlocking (uint8 tx) {
	UsartConsoleBufferSet(tx);
}

inline int32 UsartConsoleSendNonBlocking (uint8 buf[], const uint32 len) {
	uint32 i;
	const uint32 int_len = (len < USART_STRLEN_MAX)? len : USART_STRLEN_MAX;
	for (i = 0; (i < int_len) && (buf[i] != '\0'); i++)
		UsartConsoleBufferSet(buf[i]);
	return i;
}

/* **************************************************** *
 *          USART CONSOLE WRITE/READ WRAPPERS
 * **************************************************** */
void UsartConsolePutchar (uint8 symbol) {
	UARTCharPut(UART0_BASE, symbol);
}

uint32 UsartConsoleSendBlocking (uint8 buf[], const uint32 len) {
	uint32 i;
	const uint32 int_len = (len < USART_STRLEN_MAX)? len : USART_STRLEN_MAX;
	for (i = 0; (i < int_len) && (buf[i] != '\0'); i++)
		UARTCharPut(UART0_BASE, buf[i]);
	return i;
}

uint32 UsartConsoleScan (uint8 buf[], const uint32 len) {
	uint32 i;
	const uint32 int_len = (len < USART_STRLEN_MAX)? len : USART_STRLEN_MAX;
	for (i = 0; i < int_len; i++) {
		const uint8 symbol = UARTCharGet(UART0_BASE);
		buf[i] = symbol;
		if ((symbol == '\r') || (symbol == '\n')) return i;
	}
	return i;
}

/* **************************************************** *
 *           USART PC CONSOLE INITIALIZATION
 * **************************************************** */
void UsartConsoleInit (const uint32 rate) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	while (!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0));
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));

	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	SysCtlDelay(100);

	// config 8N1
	const uint32 cpuclk = SysCtlClockGet();
	const uint32 irate = (rate < USART_BAUDRATE_MAX)? rate:USART_BAUDRATE_MAX;
	UARTConfigSetExpClk(UART0_BASE, cpuclk, irate, UART_CONFIG_WLEN_8);
	SysCtlDelay(100);

//	UARTIntRegister(UART0_BASE, UsartConsoleInterrupt);
//	IntPrioritySet(INT_UART0, 0xE0);
//	UARTIntEnable(UART0_BASE, UART_INT_TX);
//	UARTTxIntModeSet(UART0_BASE, UART_TXINT_MODE_EOT);

	UARTEnable(UART0_BASE);
	SysCtlDelay(100);
}
