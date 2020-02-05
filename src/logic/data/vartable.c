
#include "logic/data/vartable.h"

#include <stddef.h> // for NULL reference
//#include <string.h>

#include "logic/comm/mbus.h"


// TODO: finish variables table and move it to header
enum mbusVarTableEnum {
	VAR_LINE_STATUS = 10001,
};

/* **************************************************** *
 *         MODICON BUS VARIABLES TABLE READING
 * **************************************************** */
int32 ConfigVarTableGet (const int32 address) {
	int32 res;
	switch (address & 0xFFFF) {
	case VAR_LINE_STATUS:
		res = LineStatusGet();
		break;
	default:
		res = MBUS_ERROR_ADDRESS;
		break;
	}
	return res;
}

/* **************************************************** *
 *         MODICON BUS VARIABLES TABLE HANDLING
 * **************************************************** */
int32 ConfigVarTableSet (const int32 address, const uint16 value) {
	int32 res;
	switch (address & 0xFFFF) {
	case VAR_LINE_STATUS:
		LineStatusSet(value & 0xFF);
		res = LineStatusGet();
		break;
	default:
		res = MBUS_ERROR_ADDRESS;
		break;
	}
	return res;
}
