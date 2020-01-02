#ifndef __EEPROM_H
#define __EEPROM_H

#include "util/typedefs.h"

/* **************************************************** *
 *                  HEADER SPECIFIC MACROS
 * **************************************************** */
enum RomEnum {
	EEPROM_BLOCK_SIZE = 16*4,
	EEPROM_SIZE = 2048,
	EEPROM_PASSWORD = 0xDEADBEEF,
};

/* **************************************************** *
 *                 FUNCTION PROTOTYPES
 * **************************************************** */
int32 RomInit(void);

int32 RomDataWrite (const uint32 pos, const uint32 buf[], const int32 size);
int32 RomDataRead(const uint32 pos, uint32 buf[], const int32 size);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__EEPROM_H
