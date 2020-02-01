/*
 * Pseudo modicon bus abstraction layer
 *
 * Can header double bit indicates mbus-like transmission:
 * 00b - nothing is in the buffer
 * 01b - request from a master device
 * 10b - response from a slave device
 * 11b - impossible state (grand error)
 *
 * Can message packet is disassembled as following:
 * [1] byte - function code (can be from 1 to 127)
 * [2-3] bytes - starting address for a function
 * [4] byte - describes significant bytes in packet
 * [5-6,7-8] bytes - incoming/outgoing data
 */

#ifndef __COMM_MODICON_H
#define __COMM_MODICON_H

#include "util/typedefs.h"

/* **************************************************** *
 *                  HEADER SPECIFIC MACROS
 * **************************************************** */
enum mbusGeneralEnum {
	MBUS_DATA_SIZE_CELL = 2,
	MBUS_DATA_SIZE_MAX  = 4,
	MBUS_MEMORY_START   = 1,
	MBUS_MEMORY_RO_COIL = 9999,  // read only bools right border
	MBUS_MEMORY_RW_COIL = 19999, // read/write bools
	MBUS_MEMORY_RO_REGS = 39999, // read only 16-bits regs
	MBUS_MEMORY_RW_REGS = 49999, // read/write 16-bits regs
};

enum mbusExceptionEnum {
	MBUS_ERROR_FUNCTION = 0x1,
	MBUS_ERROR_ADDRESS  = 0x2,
	MBUS_ERROR_VALUE    = 0x3,
	MBUS_ERROR_DEV_FAIL = 0x4,
//	MBUS_ERROR_DEV_BUSY = 0x6,
};

enum mbusFunctionEnum {
	MBUS_FUNC_ERROR_ADD   = 0x80, // this added value indicates error
	MBUS_FUNC_READ_COIL   = 1,
	MBUS_FUNC_READ_COIL2  = 2,
	MBUS_FUNC_READ_REG    = 3,
	MBUS_FUNC_READ_REG2   = 4,
	MBUS_FUNC_WRITE_COIL  = 5,
	MBUS_FUNC_WRITE_REG   = 6,
	MBUS_FUNC_WRITE_MCOIL = 15,
	MBUS_FUNC_WRITE_MREG  = 16,
};

/* **************************************************** *
 *                 FUNCTION PROTOTYPES
 * **************************************************** */
int8 ModbusMessageProcess(const uint8 input[8], uint8 *output);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__COMM_MODICON_H
