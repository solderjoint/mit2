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
#define CAN_RATE        (100000)  // can baudrate
#define SPI_RATE        (800000)  // external spi baudrate
#define USART_RATE      (115200)  // uart console baudrate

#define FOURIER_RATE    (12800)   // fft sampling rate

#define PERIOD_TIMER_FREQ  (100)  // default semaphore value
#define PERIOD_CHECK_LINE  (2)    // check every Nx10ms
#define PERIOD_CHECK_COMM  (20)   // check can messages
#define PERIOD_RENEW_VOLT  (1000) // update normal voltage

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__PROJECT_DEF
