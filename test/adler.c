#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* **************************************************** *
 *                  HEADER SPECIFIC MACROS
 * **************************************************** */
#define HASHTABLE_SIZE  (128)

/* **************************************************** *
 *           ADLER32 SLOW CRC IMPLEMENTATION
 * **************************************************** */
static uint32_t adler32 (const char data[], const uint32_t len) {
	const uint32_t MOD_ADLER = 65521;

	uint32_t a = 1, b = 0;
	for (int32_t i= 0; i < len; i++) {
		a = (a + data[i]) % MOD_ADLER;
		b = (b + a) % MOD_ADLER;
	}
	return (b << 16) | a;
}

/* **************************************************** *
 *              HASH TABLE LOCAL VARIABLES
 * **************************************************** */
union hashval_u {
	uint32_t i;
	float f;
	char c[4];
};

struct hashtable_t {
	uint32_t hash;
	uint32_t val;
};

static struct hashtable_t hash_arr[HASHTABLE_SIZE];

/* **************************************************** *
 *         HASH TABLE CHECK FOR HASH DUPLICATES
 * **************************************************** */
int32_t HashTableCheck (const uint32_t hash, const uint32_t ptr) {
	int32_t flag = 0;
	union hashval_u newhash;  newhash.i = hash;

	for (int32_t i = 0; (hash_arr[i].hash != 0) && (i < HASHTABLE_SIZE); i++) {
		if ((hash_arr[i].hash == newhash.i) && (i != ptr)) {
			flag = -1;
			continue;
		}
	}
	return flag;
}

/* **************************************************** *
 *             HASH TABLE GET/SET WRAPPERS
 * **************************************************** */
uint32_t HashTableCalculate (const char data[], const uint32_t len) {
	const uint32_t int_len = (len < 128)? len : 128;
	return adler32 (data, int_len);
}

uint32_t HashTableValueGet (const uint32_t hash) {
	for (int32_t pos = 0; (pos < HASHTABLE_SIZE); pos++) {
		if (hash_arr[pos].hash == hash)
			return hash_arr[pos].val;
	}
	return 0;
}

uint32_t HashTableFloatGet (const uint32_t hash) {
	union hashval_u val; val.i = HashTableValueGet(hash);
	return val.f;
}

uint32_t HashTableIntegerGet (const uint32_t hash) {
	return HashTableValueGet(hash);
}

/* **************************************************** *
 *           HASH TABLE MANIPULATION WRAPPERS
 * **************************************************** */
int32_t _HashTableAppend (const uint32_t hash, const uint32_t value) {
	uint32_t pos;
	for (pos = 0; hash_arr[pos].hash != 0; pos++) ;
	if (pos > HASHTABLE_SIZE) return -10;

	if (HashTableCheck (hash, pos) != 0) {
		return -1;  // indicate hash duplicate error
	} else {
		hash_arr[pos].hash = hash;
		hash_arr[pos].val = value;
		return 0;   // normal exit value
	}
}

void _HashTableRemove (const uint32_t hash) {
	uint32_t pos;
	for (pos = 0; hash_arr[pos].hash != hash; pos++) ;
	if (pos < HASHTABLE_SIZE) hash_arr[pos].hash = ((uint32_t) 0);
}



#define HashTableAppend(HASH, VAR) { \
	const char _name_##VAR[] = (#VAR); \
	const uint32_t _len_##VAR = strnlen(_name_##VAR, 128); \
	const uint32_t _hash_##VAR = HashTableCalculate(_name_##VAR, _len_##VAR); \
	const int32_t _res_##VAR = _HashTableAppend(_hash_##VAR, (VAR)); \
	(HASH) = (_res_##VAR == 0)? _hash_##VAR : 0; \
	}

int main (void) {
	const uint32_t vars[10] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 0xFFFF};
	for (int32_t i = 0; i < 10; i++) {
		uint32_t hash = 0;
//		HashTableAppend(hash, vars[0]);
		HashTableAppend(hash, i);

		if (hash == 0) {
			printf ("%i>\tcollision>\n", i);
		} else {
			const uint32_t vali = HashTableValueGet(hash);
			printf ("%i>\t[%08X]:[%X>%X]\n", i, hash, vars[i], vali);
		}
	}

	return 0;
}
