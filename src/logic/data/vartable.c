
#include "logic/data/vartable.h"

#include <stddef.h> // for NULL reference
#include <string.h>

#include "logic/comm/mbus.h"

#include "vars/linestatus.h"
#include "vars/relaystatus.h"

// TODO: finish variables table and move it to header
enum mbusVarTableEnum {
	_VAR_RO_COIL_OFFSET = 10000,
	VAR_LINE_STATUS,
	VAR_RELAY_STATUS,
};

/* **************************************************** *
 *         MODICON BUS VARIABLES TABLE READING
 * **************************************************** */
int32 ConfigVarTableGet (const int32 address) {
	int32 res;
	switch (address & 0xFFFF) {
	case VAR_LINE_STATUS: res = LineStatusGet(); break;
	case VAR_RELAY_STATUS: res = RelayStatusGet(); break;

	default: res = MBUS_ERROR_ADDRESS; break;
	}
	return res;
}

/* **************************************************** *
 *         MODICON BUS VARIABLES TABLE HANDLING
 * **************************************************** */
int32 ConfigVarTableSet (const int32 address, const int16 value) {
	int32 res;
	switch (address & 0xFFFF) {
	case VAR_LINE_STATUS:
		LineStatusSet(value & 0xFF);
		res = LineStatusGet();
		break;
	case VAR_RELAY_STATUS:
		RelayStatusSet((value > 0)? RELAY_ON : RELAY_OFF);
		res = RelayStatusGet();
		break;

	default:
		res = MBUS_ERROR_ADDRESS;
		break;
	}
	return res;
}
