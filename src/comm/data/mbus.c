
#include "comm/data/mbus.h"

#include "vars/fourierconsts.h"

/* **************************************************** *
 *          MBUS SHARED VAIABLES ADDRESS TABLE
 * **************************************************** */
static int32 mbus_get_coil (const uint16 address) {
	int32 res;
	switch (address) {
	case 1:
//		res =
		break;
	default:
		return -1;
		break;
	}
}

/* **************************************************** *
 *        SMOLIN QUICK INCOMING DATA PROCESSING
 * **************************************************** */
// TODO: combine function/response in a single function
static int32 ModbusFunctionReadSingle (const uint8 input[8]) {
	// >> FUNC[1] ADDRESS[2] BYTES[1]
	const uint16 address = (input[1] << 8) | input[2];
	const uint8 bytes = input[3] % 7;  // <= 6
	if (bytes != MBUS_DATA_SIZE_CELL) return -MBUS_ERROR_ADDRESS;
	if (address < MBUS_MEMORY_START) return -MBUS_ERROR_ADDRESS;
	else if (address <= MBUS_MEMORY_RW_COIL) return mbus_get_coil(address);
	else if (address <= MBUS_MEMORY_RW_REGS) return mbus_get_reg(address);
	else return -MBUS_ERROR_ADDRESS;
}

// TODO: redo data outut for multiple reads
static inline int8 ModbusResponseReadSingle
(const uint8 input[8], uint8 output[8], const int32 data) {
	// << FUNC[1] BYTES[1] DATA[2]
	if (data < 0) {
		output[0] += MBUS_ERROR_ADDER;
		output[1] = -data;  // return error code
	} else {
		output[1] = MBUS_DATA_SIZE_CELL;

		output[2] = ((uint16) data) >> 8;
		output[3] = ((uint8) data);
	}
	return ((int8)output[0]);
}

/* **************************************************** *
 *        SMOLIN QUICK OUTGOING DATA PREPARATION
 * **************************************************** */
static int32 ModbusFunctionWriteSingle (const uint8 input[8]) {
	// >> FUNC[1] ADDRESS[2] DATA[2]
	const uint16 address = (input[1] << 8) | input[2];
	const uint8 bytes = input[3] % 5;  // <= 4
	if (bytes != MBUS_DATA_SIZE_CELL) return -MBUS_ERROR_ADDRESS;
	if (address < MBUS_MEMORY_START) return -MBUS_ERROR_ADDRESS;
	else if (address <= MBUS_MEMORY_RW_COIL) return mbus_set_coil(address);
	else if (address <= MBUS_MEMORY_RW_REGS) return mbus_set_reg(address);
	else return -MBUS_ERROR_ADDRESS;
}

static inline int8 ModbusResponseWriteSingle
(const uint8 input[8], uint8 output[8], const int32 data) {
	// << FUNC[1] ADDRESS[2] DATA[2]
	if (data < 0) {
		output[0] += MBUS_ERROR_ADDER;
		output[1] = -data;  // return error code
	} else {
		output[1] = input[1];  // copy address
		output[2] = input[2];
		output[3] = ((uint16) data) >> 8;
		output[4] = ((uint8) data);
	}
	return ((int8)output[0]);
}

/* **************************************************** *
 *        SMOLIN QUICK OUTGOING DATA PREPARATION
 * **************************************************** */
int8 ModbusMessageProcess (const uint8 input[8], uint8 output[8]) {
	int32 data;  // buffer to store results
	for (int i = 0; i < 8; i++) output[i] = 0;
	output[0] = input[0];  // copy packet command

	switch (input[0]) {
	case MBUS_FUNC_READ_COIL:
	case MBUS_FUNC_READ_COIL2:
	case MBUS_FUNC_READ_REG:
	case MBUS_FUNC_READ_REG2:
		data = ModbusFunctionReadSingle(input);
		return ModbusResponseReadSingle(input, output, data);
		break;

	case MBUS_FUNC_WRITE_COIL:
	case MBUS_FUNC_WRITE_REG:
		data = ModbusFunctionWriteSingle(input);
		return ModbusResponseWriteSingle(input, output, data);
		break;

	case MBUS_FUNC_WRITE_MCOIL:
	case MBUS_FUNC_WRITE_MREG:
		// >> FUNC[1] ADDRESS[2] BYTES[1] DATA[BYTES]
		// << FUNC[1] ADDRESS[2] BYTES[1] DATA[BYTES]
		break;

	default:
//		comm += MBUS_FUNC_ERROR_ADD; // indicate error
		return -MBUS_ERROR_FUNCTION;
		break;
	}
}
