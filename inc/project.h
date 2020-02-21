/* **************************************************** *
 *          IMPULSE MODULATED SENSOR CATCHING
 * **************************************************** *
 * Main purpose of this project is to provide a firmware
 * for proper sensor line error state handling, using
 * only 1 adc and 1 analog/digital trigger per line
 *
 * Electrical diagrams are located in "hardware" folder
 * Software part (if any) is located in "software" folder
 * MCU firmware is located in "firmware" folder
 * Documentation is located in "docs" folder
 * **************************************************** */

#ifndef __PROJECT_DEF
#define __PROJECT_DEF

/* **************************************************** *
 *          MAIN PERIPH AND LOGIC DEFINITIONS
 * **************************************************** */
enum projectDefaultEnum {
	CAN_RATE      = 100000,  // can baudrate
	SPI_RATE      = 800000,  // external spi baudrate
	USART_RATE    = 115200,  // uart console baudrate

	PERIOD_TIMER_FREQ = 100,  // default semaphore value
	PERIOD_CHECK_LINE = 2,    // check every Nx10ms
	PERIOD_CHECK_COMM = 20,   // check can messages
	PERIOD_RENEW_VOLT = 1000, // update normal voltage

	FOURIER_SAMPLE_BUFLEN = 1024,   // size of sampling buffer
	FOURIER_SAMPLE_RATE   = 12800,  // default sampling rate
	FOURIER_OVERSAMPLES   = 4,      // fourier oversamples
	FOURIER_FREQRANGE_MIN = 4150,   // default minimum freq
	FOURIER_FREQRANGE_MAX = 5850,   // default minimum freq

};

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__PROJECT_DEF
