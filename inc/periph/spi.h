/*
 * Peripherials header for mcu on-board usart
 */

#ifndef __SPI_H
#define __SPI_H

#include "util/typedefs.h"
#include "util/util.h"

/* **************************************************** *
 *                  SYSTEM-WIDE MACROS
 * **************************************************** */
#define SPI_CLOCK_MAX_DIV  (16) // max spi clock = cpuclk/DIV
//#define SPI_BITLEN_MIN     (5)  // max spi bit length
#define SPI_BITLEN_DEF     (16) // default spi bit length
//#define SPI_BITLEN_MAX     (16) // min spi bit length

/* **************************************************** *
 *                 FUNCTION PROTOTYPES
 * **************************************************** */
void SpiExternalAdcInit(uint32 rate);
int32 SpiExternalAdcDiagnose (void);
void SpiExternalAdcBitlenSet (uint32 x);

uint16 SpiExternalAdcGetNonBlocking (void);
uint16 SpiExternalAdcGet (void);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__SPI_H
