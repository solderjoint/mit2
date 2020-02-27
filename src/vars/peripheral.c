
#include "vars/peripheral.h"

#include "util/util.h"
#include "project.h"

/* **************************************************** *
 *                CAN PERIPHERAL WRAPPER
 * **************************************************** */
static int32 rate_can;

int32 PeriphCanRateGet (void) {
	const int32 x = rate_can;
	return ((x < kil(1)) || (x > mil(1)))? PERIPH_CAN_RATE : x;
}

int32 PeriphCanRateSet (const uint32 x) {
	rate_can = ((x < kil(1)) || (x > mil(1)))? PERIPH_CAN_RATE : x;
	return rate_can;
}

/* **************************************************** *
 *                SPI PERIPHERAL WRAPPER
 * **************************************************** */
static int32 rate_spi;

int32 PeriphSpiRateGet (void) {
	const int32 x = rate_spi;
	return ((x < kil(1)) || (x > mil(1)))? PERIPH_SPI_RATE : x;
}

int32 PeriphSpiRateSet (const uint32 x) {
	rate_spi = ((x < kil(1)) || (x > mil(1)))? PERIPH_SPI_RATE : x;
	return rate_spi;
}

/* **************************************************** *
 *               USART PERIPHERAL WRAPPER
 * **************************************************** */
static int32 rate_uart;

int32 PeriphUartRateGet (void) {
	const int32 x = rate_uart;
	return ((x < kil(1)) || (x > mil(1)))? PERIPH_UART_RATE : x;
}

int32 PeriphUartRateSet (const uint32 x) {
	rate_uart = ((x < kil(1)) || (x > mil(1)))? PERIPH_UART_RATE : x;
	return rate_uart;
}

/* **************************************************** *
 *            EXTERNAL DEFAULT VALUE SETTER
 * **************************************************** */
char PeriphConstantsInit (void) {
	rate_can = PERIPH_CAN_RATE;
	rate_spi = PERIPH_SPI_RATE;
	rate_uart = PERIPH_UART_RATE;
}
