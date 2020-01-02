
#include "periph/rom.h"

#include <stdbool.h>
#include <stdint.h>

#include <tivaware/inc/hw_can.h>
#include <tivaware/inc/hw_ints.h>
#include <tivaware/inc/hw_memmap.h>

#include <tivaware/driverlib/eeprom.h>
#include <tivaware/driverlib/interrupt.h>
#include <tivaware/driverlib/sysctl.h>

/* **************************************************** *
 *                EEPROM INITIALIZATION
 * **************************************************** */
int32 RomInit (void) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
	while (!SysCtlPeripheralReady(SYSCTL_PERIPH_EEPROM0)) ;
	uint32 status = EEPROMInit();
	if (status != EEPROM_INIT_OK) {
		// while (1) ; // ensure we stop here
		return status;
	} else {
		return 0;
	}
}

/* **************************************************** *
 *                 EEPROM DATA HANDLERS
 * **************************************************** */
int32 RomDataWrite (const uint32 pos, const uint32 buf[], const int32 size) {
	const uint32 rom_size = EEPROMSizeGet();
	const uint32 int_size = (size / 4) * 4;
	if ((pos + size) > rom_size) return -1;
	if (int_size <= 0) return -2;

	EEPROMProgram(buf, pos, int_size);
	const int32 status = EEPROMStatusGet();
	return status;
}
// TODO: make sure reading and writing succeeds w/out password
int32 RomDataRead (const uint32 pos, uint32 buf[], const int32 size) {
	const uint32 rom_size = EEPROMSizeGet();
	const int32 int_size = (size / 4) * 4;
	if ((pos + size) > rom_size) return -1;
	if (int_size <= 0) return -2;

	EEPROMRead(buf, pos, int_size);
	return int_size;
}
