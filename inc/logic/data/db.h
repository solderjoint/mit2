/*
 * Extra small single-linked hash-value database.
 *
 * Hash must be in crc32 format (f.e.: the name of a variable).
 * Value can be up to 4 bytes long (usually int32 or float32).
 */

#ifndef __DATA_DB_H
#define __DATA_DB_H

#include "util/typedefs.h"

/* **************************************************** *
 *                HEADER SPECIFIC MACROS
 * **************************************************** */
enum databaseEnum {
	DATABASE_HASHTYPE_CRC32 = 0,
	DATABASE_SIZE = 128,
};

/* **************************************************** *
 *                 FUNCTION PROTOTYPES
 * **************************************************** */
int32 DatabaseFind (const uint32 hash);

uint32 DatabaseValueGet (const uint32 hash);
float DatabaseValueGetFloat (const uint32 hash);

int32 DatabaseValueUpdate (const uint32 hash, const uint32 val);
int32 DatabaseValueUpdateFloat (const uint32 hash, const uint32 val);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__DATA_DB_H