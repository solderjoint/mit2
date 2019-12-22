
#include "periph/spi.h"

#include <stdbool.h>
#include <stdint.h>

#define  PART_TM4C1231H6PZ
#include <tivaware/inc/hw_gpio.h>
#include <tivaware/inc/hw_memmap.h>
#include <tivaware/inc/hw_ssi.h>
#include <tivaware/inc/hw_types.h>

#include <tivaware/driverlib/gpio.h>
#include <tivaware/driverlib/interrupt.h>
#include <tivaware/driverlib/pin_map.h>
#include <tivaware/driverlib/sysctl.h>
#include <tivaware/driverlib/ssi.h>

/* **************************************************** *
 *              SPI LOCAL SETTINGS SECTION
 * **************************************************** */

/* **************************************************** *
 *        EXTERNAL ADC THROUGH SPI READ WRAPPER
 * **************************************************** */
inline uint16 SpiExternalAdcGet (void) {
	SSIDataPut(SSI2_BASE, '\0'); // pull ss line
	while (SSIBusy(SSI2_BASE));  // wait until receive is done
	const uint16 read = cast(uint16, HWREG(SSI2_BASE + SSI_O_DR));
	return read;
}

inline uint16 SpiExternalAdcGetNonBlocking (void) {
	// get input buffer data directly
	const uint16 read = cast(uint16, HWREG(SSI2_BASE + SSI_O_DR));
	// wait until output buffer is empty
	while (!(HWREG(SSI2_BASE + SSI_O_SR) & SSI_SR_TNF)) ;
	// send empty char, pulling ss line
	HWREG(SSI2_BASE + SSI_O_DR) = '\0';
	return read;
}

/* **************************************************** *
 *               EXTERNAL SPI DIAGNOSTIC
 * **************************************************** */
int32 SpiExternalAdcDiagnose (void) {
	SSIDataPut(SSI2_BASE, '\0');
	SysCtlDelay(2000); // delay to be sure receive is complete

	if (SSIBusy(SSI2_BASE)) return 10;

	const uint16 read = cast(uint16, HWREG(SSI2_BASE + SSI_O_DR));
	if (read == 0x0000) return 20;
	else if (read == 0xFFFF) return 21;
	else if (read >= 4095) return 22;
	else return 0; // normal diagnose result
}

/* **************************************************** *
 *                INITIALIZATION SECTION
 * **************************************************** */
void SpiExternalAdcInit (uint32 rate) {
	// init spi in master, motorola mode
	const uint32 cpuclk = SysCtlClockGet();
	const uint32 maxrate = cpuclk / 16;
	const uint32 clock = (rate <= maxrate)? rate : maxrate;
	const uint32 bitlen = SPI_BITLEN_DEF;

	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI2);
	while (!SysCtlPeripheralReady(SYSCTL_PERIPH_SSI2));
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
	while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOH));

	// define spi function for selected pins
	GPIOPinConfigure(GPIO_PH4_SSI2CLK);
	GPIOPinConfigure(GPIO_PH5_SSI2FSS);
	GPIOPinConfigure(GPIO_PH6_SSI2RX);
	//GPIOPinConfigure(GPIO_PH7_SSI2TX);
	GPIOPinTypeSSI(GPIO_PORTH_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6);

	// set motorola mode master with selected clock and bitlen
	SSIConfigSetExpClk(SSI2_BASE, cpuclk, \
		SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, clock, bitlen);
	SSIEnable(SSI2_BASE);
	SysCtlDelay(1000);

	// wait until incoming buffer is cleared
	while (SSIDataGetNonBlocking(SSI2_BASE, &bitlen));
	while (SSIBusy(SSI2_BASE));
}
