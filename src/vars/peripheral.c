
#include "vars/peripheral.h"

#include <string.h>

#include "logic/data/db.h"
#include "util/util.h"
#include "project.h"

/* **************************************************** *
 *                CAN PERIPHERAL WRAPPER
 * **************************************************** */
int32 PeripheralCanRateGet (void) {
	const char buf[] = stringify(PeripheralCanRate);
	const uint32 hash = DatabaseHashGet(buf);
	const int32 val = DatabaseValueGet(hash);
	return ((val < kil(2)) || (val > mil(2)))? CAN_RATE : val;
}

int32 PeripheralCanRateSet (const int32 val) {
	const uint32 rate = ((val < kil(2)) || (val > mil(2)))? CAN_RATE : val;
	const char buf[] = stringify(PeripheralCanRate);
	const uint32 hash = DatabaseHashGet(buf);
	return DatabaseValueSet(hash, rate);
}

/* **************************************************** *
 *               USART PERIPHERAL WRAPPER
 * **************************************************** */
//int32 PeripheralUsartRateGet (void) {
