
#include "logic/data/db.h"

#include <stddef.h> // for NULL reference
#include <stdlib.h>
#include <string.h>

#include "periph/rom.h"
//#include "util/print.h"
#include "util/crc.h"

/* **************************************************** *
 *                  DATABASE UTILITIES
 * **************************************************** */
static int32 find_free_pos (const int32 prev_pos) {
	// tries to find next free position in hashtable
	const int32 step = 2 * sizeof(uint32);
	const int32 prev = (prev_pos < 0)? 0 : prev_pos;
	// check free space sequentially from head to tail
	for (int i = (prev + step); i < (DATABASE_SIZE * step); i += step) {
		uint32 buf;
		RomDataRead(i, &buf, sizeof(uint32));
		if (buf == 0) return i;
	}
	// going around from tail to the head
	for (int i = 0; i <= prev; i += step) {
		uint32 buf;
		RomDataRead(i, &buf, sizeof(uint32));
		if (buf == 0) return i;
	}
	return DATABASE_NOEMPTYSPACE;
}

/* **************************************************** *
 *      CRC32 HASH CALCULATION AND FINDER UTILITY
 * **************************************************** */
inline uint32 DatabaseHashGet (const char stringified[64]) {
	if (stringified == NULL) return 0;
	const int32 len = strnlen(stringified, 63);
	return CRC32Calculate(stringified, len);
}

inline int32 DatabaseHashFind (const uint32 hash) {
	// return the position of a hash if something is found
	const int32 step = 2 * sizeof(uint32);
	for (int i = 0; i < (DATABASE_SIZE * step); i += step) {
		uint32 buf;
		RomDataRead(i, &buf, sizeof(uint32));
		if (buf == hash) return i;
	}
	return DATABASE_HASH_NOTFOUND;
}

/* **************************************************** *
 *                DATABASE WRITE WRAPPER
 * **************************************************** */
int32 DatabaseValueGet (const uint32 hash) {
	const int32 pos = DatabaseHashFind(hash);
	if (pos >= 0) {
		uint32 buf;
		const int32 val_size = sizeof(uint32);
		RomDataRead(pos + val_size, &buf, val_size);
		return (int32)buf;
	}
	return pos;
}

float DatabaseValueGetFloat (const uint32 hash) {
	const int32 resi = DatabaseValueGet(hash);
	const float resf = * (float *) &resi;
	return resf;
}

/* **************************************************** *
 *                 DATABASE READ WRAPPER
 * **************************************************** */
int32 DatabaseValueSet (const uint32 hash, const uint32 val) {
	// find previous hash position
	const int32 prev = DatabaseHashFind(hash);
	const uint32 wlen = sizeof(uint32);
	const uint32 buf[2] = {hash, val};
	if (prev >= 0) {
		const uint32 clear = (uint32)0;
		const int32 s = RomDataWrite(prev, &clear, wlen);
		if (s != 0) return DATABASE_WRITE_ERROR;
	}
	// find next free position
	const uint32 pos = find_free_pos((prev < 0)? 0 : prev);
	if (pos < 0) {
		return DATABASE_NOEMPTYSPACE;
	} else {
		const int32 s = RomDataWrite(pos, buf, 2 * wlen);
		if (s != 0) return DATABASE_WRITE_ERROR;
	}
	return 0; // indicate no errors
}

int32 DatabaseValueSetFloat (const uint32 hash, const float val) {
	const float valf = val;
	const uint32 vali = * (uint32 *) &valf;
	return DatabaseValueSet(hash, vali);
}
