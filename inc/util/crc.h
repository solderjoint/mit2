/*
 * modbus crc16 and crc32 implementations
 */

#ifndef __UTIL_CRC_H
#define __UTIL_CRC_H

#include "util/typedefs.h"

/* **************************************************** *
 *                 FUNCTION PROTOTYPES
 * **************************************************** */
int32 CRC16Calculate (const uint8 *data, const int32 size);
uint32 CRC32Calculate (const uint8 *data, const int32 size);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif // __UTIL_CRC_H
