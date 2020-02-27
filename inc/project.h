﻿/* **************************************************** *
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
	PERIPH_CAN_RATE       = 100000, // can baudrate
	PERIPH_SPI_RATE       = 800000, // external spi baudrate
	PERIPH_UART_RATE      = 115200, // uart console baudrate

	PERIOD_TIMER_FREQ     = 100,    // default semaphore value
	PERIOD_CHECK_LINE     = 2,      // check every Nx10ms
	PERIOD_CHECK_COMM     = 20,     // check can messages
	PERIOD_RENEW_VOLT     = 1000,   // update normal voltage

	FOURIER_FREQ_STEP     = 50,     // default step between freqs
	FOURIER_FREQ_RANGEMIN = 4150,   // default minimum freq range
	FOURIER_FREQ_RANGEMAX = 5850,   // default maximum freq range
	FOURIER_FREQ_REFPOWER = 5000,   // power reference freq

	FOURIER_SAMPLE_BUFLEN = 1024,   // size of sampling buffer
	FOURIER_SAMPLE_RATE   = 12800,  // default sampling rate
	FOURIER_SAMPLE_PASSES = 4,      // fourier oversamples

	FOURIER_POWER_MIN     = 350,    // minimum power of a freq to consider
	FOURIER_POWER_MULTMIN = 35,     // minimum [%] of a reference power

	VOLTAGE_AC_ADCCOUNTS  = 2048,   // ac line adc counts
	VOLTAGE_AC_MAXVOLT    = 5000,   // ac line maximum voltage [mV]
	VOLTAGE_DC_ADCCOUNTS  = 2048,   // dc line adc counts
	VOLTAGE_DC_MAXVOLT    = 24000,  // dc line maximum voltage [mV]

	VOLTAGE_SPIKE         = 4000,   // minimum voltage diff to crash [mV]
	VOLTAGE_NORMAL        = 4000,   // normal conditions voltage [mV]
	VOLTAGE_OVERFLOW      = 22500,  // overflow voltage value [mV]
	VOLTAGE_UNDERFLOW     = 500,    // underflow voltage value [mV]
};

//#define  VOLTAGE_DCMULT     (0.011718750f)  // convert 0..2048ADC to 0..24VDC
//#define  VOLTAGE_ACMULT     (0.002441406f)  // convert 0..2048ADC to 0..5VAC

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__PROJECT_DEF
