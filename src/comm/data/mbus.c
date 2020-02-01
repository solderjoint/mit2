
#include "comm/data/mbus.h"

#include <math.h>

#include "vars/fourierconsts.h"

/* **************************************************** *
 *             MODICON BUS VAIABLES HANDLER
 * **************************************************** */
static int32 mbus_read_coil (const int32 address) {
	int32 res = 0;
	switch (address) {
	case 1:
		res = 1;
		break;
	case 10001:
		res = 10001;
		break;
	default:
		res = MBUS_ERROR_ADDRESS;
		break;
	}
	return res;
}

static int32 mbus_read_reg (const int32 address) {
	int32 res = 0;
	switch (address) {
	case 30001:
		res = 30001;
		break;
	default:
		res = MBUS_ERROR_ADDRESS;
		break;
	}
	return res;
}

/* **************************************************** *
 *       UNIFIED MODICON BUS LOCAL BUFFER WRAPPER
 * **************************************************** */
static uint8 mbus_in[8]; // copied input buffer
// >> FUNC[1] ADDRESS[2] BYTES[1] DATA1[2] DATA2[2]
static uint8 mbus_get_func (void) { return mbus_in[0]; }
static uint16 mbus_get_address (void) {
	return ((mbus_in[1] << 8) | mbus_in[2]);
}
static uint8 mbus_get_bytes (void) {
	return ((mbus_in[3] + (mbus_in[3] % 2)) % 5);
}
static uint16 mbus_get_data1 (void) {
	return ((mbus_in[4] << 8) | mbus_in[5]);
}
static uint16 mbus_get_data2 (void) {
	return ((mbus_in[6] << 8) | mbus_in[7]);
}

/* **************************************************** */
static uint8 *mbus_out;  // pointer to the output buffer
// << FUNC[1] ADDRESS[2] BYTES[1] DATA1[2] DATA2[2]
static void mbus_set_func (const uint8 x) { mbus_out[0] = x; }
static void mbus_set_address (const uint16 add) {
	mbus_out[1] = add >> 8;  mbus_out[2] = add & 0xFF;
}
static void mbus_set_bytes (const uint8 x) { mbus_out[3] = x % 5; }
static void mbus_set_data1 (const uint16 data1) {
	mbus_out[4] = data1 >> 8;  mbus_out[5] = data1 & 0xFF;
}
static void mbus_set_data2 (const uint16 data2) {
	mbus_out[6] = data2 >> 8;  mbus_out[7] = data2 & 0xFF;
}
static void mbus_set_error (const int32 err) {
	for (int i = 0; i < 8; i++) mbus_out[i] = 0;
	mbus_out[0] = mbus_in[0] + MBUS_FUNC_ERROR_ADD;
	mbus_out[1] = abs(err) & 0xFF;
}

/* **************************************************** *
 *          UNIFIED MODICON BUS READ PROCESSOR
 * **************************************************** */
static uint16 data[2];

static inline int32 ModbusFunctionRead (void) {
	// >> FUNC[1] ADDRESS[2] BYTES[1]
	int32 error = 0;
	const uint16 address = mbus_get_address();
	const uint8 bytes = mbus_get_bytes();

	if ((address < MBUS_MEMORY_START) || (address > MBUS_MEMORY_RW_REGS)) {
		error = MBUS_ERROR_ADDRESS;
	} else {
		for (int i = 0; i < 2; i++) data[i] = 0;
		for (int i = 0; i < bytes; i += 2) {
			if (address <= MBUS_MEMORY_RW_COIL) {
				const int32 res = mbus_read_coil(address);
				if ((res < 0) || (res > 0xFFFF)) error = abs(res);
				else data[i] = ((uint16) res);
			} else if (address <= MBUS_MEMORY_RW_REGS) {
				const int32 res = mbus_read_reg(address);
				if ((res < 0) || (res > 0xFFFF)) error = abs(res);
				else data[i] = ((uint16) res);
			}
		}
	}
	return error;
}

/* **************************************************** *
 *         UNIFIED MODICON BUS WRITE PROCESSOR
 * **************************************************** */
static inline int32 ModbusFunctionWrite (void) {
	// >> FUNC[1] ADDRESS[2] BYTES[1] DATA1[2] DATA2[2]
	int32 error = 0;
	const uint16 address = mbus_get_address();
	const uint8 bytes = mbus_get_bytes();

	if ((address < MBUS_MEMORY_START) || (address > MBUS_MEMORY_RW_REGS)) {
		error = MBUS_ERROR_ADDRESS;
	} else {
		for (int i = 0; i < 2; i++) data[i] = 0;
		for (int i = 0; i < bytes; i += 2) {
			if (address <= MBUS_MEMORY_RW_COIL) {
				const int32 res = mbus_write_coil(address);
				if ((res < 0) || (res > 0xFFFF)) error = abs(res);
				else data[i] = ((uint16) res);
			} else if (address <= MBUS_MEMORY_RW_REGS) {
				const int32 res = mbus_write_reg(address);
				if ((res < 0) || (res > 0xFFFF)) error = abs(res);
				else data[i] = ((uint16) res);
			}
		}
	}
	return error;
}

/* **************************************************** *
 *         UNIFIED MODICON BUS RESPONSE HANDLER
 * **************************************************** */
static inline int8 ModbusResponse (const int32 error) {
	// << FUNC[1] ADDRESS[2] BYTES[1] DATA1[2] DATA2[2]
	if (error != 0) {
		mbus_set_error(error);
	} else {
		mbus_set_func(mbus_get_func());
		mbus_set_address(mbus_get_address());
		mbus_set_bytes(mbus_get_bytes());
		mbus_set_data1(data[0]);
		mbus_set_data2(data[1]);  // will be zero nevertheless
	}
	return ((int8) error);
}

/* **************************************************** *
 *        UNIFIED MODICON BUS MESSAGE PROCESSER
 * **************************************************** */
int8 ModbusMessageProcess (const uint8 input[8], uint8 *output) {
	for (int i = 0; i < 8; i++) mbus_in[i] = input[i];
	mbus_out = output;  // copy output buffer location

	switch (mbus_in[0]) {
	case MBUS_FUNC_READ_COIL:
	case MBUS_FUNC_READ_COIL2:
	case MBUS_FUNC_READ_REG:
	case MBUS_FUNC_READ_REG2:
		return ModbusResponse(ModbusFunctionRead());
		break;

	case MBUS_FUNC_WRITE_COIL:
	case MBUS_FUNC_WRITE_REG:
	case MBUS_FUNC_WRITE_MCOIL:
	case MBUS_FUNC_WRITE_MREG:
		return ModbusResponse(ModbusFunctionWrite());
		break;

	default:
		return ModbusResponse(MBUS_ERROR_FUNCTION);
		break;
	}
}
