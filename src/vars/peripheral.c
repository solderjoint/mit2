
#include "vars/peripheral.h"

#include <string.h>

#include "logic/data/db.h"
#include "util/util.h"
#include "project.h"

/* **************************************************** *
 *              LOCAL VARIABLE NAME BUFFER
 * **************************************************** */
static char var_name[64];

/* **************************************************** *
 *                CAN PERIPHERAL WRAPPER
 * **************************************************** */
int32 PeripheralCanRateGet (void) {
	*var_name = stringify(PeripheralCanRate);
	const uint32 hash = DatabaseHashGet(var_name);
	const int32 val = DatabaseValueGet(hash);
	return ((val < kil(2)) || (val > mil(2)))? CAN_RATE : val;
}

int32 PeripheralCanRateSet (const uint32 val) {
	const uint32 rate = ((val < kil(2)) || (val > mil(2)))? CAN_RATE : val;
	const char buf[] = stringify(PeripheralCanRate);
	const uint32 hash = DatabaseHashGet(buf);
	return DatabaseValueSet(hash, rate);
}

/* **************************************************** *
 *                SPI PERIPHERAL WRAPPER
 * **************************************************** */
int32 PeripheralSpiRateGet (void) {
	*var_name = stringify(PeripheralSpiRate);
	const uint32 hash = DatabaseHashGet(var_name);
	const uint32 val = DatabaseValueGet(hash);
	return (val > mil(1))? SPI_RATE : val;
}

int32 PeripheralSpiRateSet (const uint32 val) {
	const uint32 rate = (val > mil(1))? SPI_RATE : val;
	const char buf[] = stringify(PeripheralSpiRate);
	const uint32 hash = DatabaseHashGet(buf);
	return DatabaseValueSet(hash, rate);
}

/* **************************************************** *
 *               USART PERIPHERAL WRAPPER
 * **************************************************** */
int32 PeripheralUsartRateGet (void) {
	*var_name = stringify(PeripheralUsartRate);
	const uint32 hash = DatabaseHashGet(var_name);
	const uint32 val = DatabaseValueGet(hash);
	return (val > mil(2))? USART_RATE : val;
}

int32 PeripheralUsartRateSet (const uint32 val) {
	const uint32 rate = (val > mil(2))? USART_RATE : val;
	const char buf[] = stringify(PeripheralUsartRate);
	const uint32 hash = DatabaseHashGet(buf);
	return DatabaseValueSet(hash, rate);
}
