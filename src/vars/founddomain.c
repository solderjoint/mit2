
#include "vars/founddomain.h"

#include <math.h>

#include "vars/buzzerdomain.h"  // get limit specifiers

enum internalEnum {
	RAWBUF_LEN  =  128,     // input and output buffer length

};

/* **************************************************** *
 *             RAW FOUND FREQUENCIES BUFFER
 * **************************************************** */
struct domain_t {
	int32 next;
	int16 freq[RAWBUF_LEN];
} ;
static struct domain_t rawbuf;

/* **************************************************** *
 *               FOUND FREQUENCIES BUFFER
 * **************************************************** */
void FoundDomainReset (void) {
	for (uint32 i = 0; i < RAWBUF_LEN; i++) {
		rawbuf.freq[i] = 0;
	}
	rawbuf.next = 0;
}

void FoundDomainSet (const uint32 freq) {
	const uint32 next = rawbuf.next;
	const uint32 ptr = (next < RAWBUF_LEN)? next : 0;
	rawbuf.freq[ptr] = freq;
	rawbuf.next++;
}

void FoundDomainClear (const uint32 ptr) {
	if (ptr >= RAWBUF_LEN) return -1;
	rawbuf.freq[ptr] = 0;
}

/* **************************************************** *
 *               FOUND FREQUENCIES BUFFER
 * **************************************************** */
int32 FoundDomainGetByCounter (const uint32 counter) {
	if (counter >= RAWBUF_LEN) return -1;
	return rawbuf.freq[counter];
}

char DomainResultIsNotEmpty (void) {
	for (int i = 0; i < RAWBUF_LEN; i++) {
		if (rawbuf.freq[i] > 0) return 1;
	}
	return 0;
}

int32 FoundDomainGetLength (void) {
	return rawbuf.next;
}

int32 FoundDomainGetLengthMax (void) {
	return RAWBUF_LEN;
}
