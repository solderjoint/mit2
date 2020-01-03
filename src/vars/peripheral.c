
#include "vars/peripheral.h"

#include <string.h>

#include "logic/data/db.h"
#include "util/crc.h"
#include "util/util.h"

/* **************************************************** *
 *           PERIPHERAL COMMINCATION SET/GETS
 * **************************************************** */
int32 PeripheralCanRateGet (void) {
	const uint8 buf[] = stringify(PeripheralCanRateGet);
	const uint32 size = strnlen(buf, 255);
	const uint32 hash = CRC32Calculate(buf, size);
	return DatabaseFind(hash);
}
int32 PeripheralCanRateSet (void) {}
