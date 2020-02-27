
#include "vars/founddomain.h"

#include <math.h>

#include "vars/buzzerdomain.h"  // get limit specifiers

#define FREQ_DOM_BUFLEN     (64)     // input and output buffer length

/* **************************************************** *
 *               FOUND FREQUENCIES BUFFER
 * **************************************************** */
struct domain_t {
	uint32 next;
	int16 freq[FREQ_DOM_BUFLEN];
} ;
static struct domain_t foundDomain;

/* **************************************************** *
 *               FOUND FREQUENCIES BUFFER
 * **************************************************** */
void FoundDomainReset (void) {
	for (uint32 i = 0; i < FREQ_DOM_BUFLEN; i++) {
		foundDomain.freq[i] = 0;
	}
	foundDomain.next = 0;
}

void FoundDomainSet (const uint32 freq) {
	const uint32 next = foundDomain.next;
	const uint32 ptr = (next < FREQ_DOM_BUFLEN)? next : 0;
	foundDomain.freq[ptr] = freq;
	foundDomain.next++;
}

void FoundDomainClear (const uint32 ptr) {
	if (ptr >= FREQ_DOM_BUFLEN) return -1;
	foundDomain.freq[ptr] = 0;
}

/* **************************************************** *
 *               FOUND FREQUENCIES BUFFER
 * **************************************************** */
int32 FoundDomainGetByCounter (const uint32 counter) {
	if (counter >= FREQ_DOM_BUFLEN) return -1;
	return foundDomain.freq[counter];
}

char DomainResultIsNotEmpty (void) {
	for (int i = 0; i < FREQ_DOM_BUFLEN; i++) {
		if (foundDomain.freq[i] > 0) return 1;
	}
	return 0;
}

int32 FoundDomainGetLengthMax (void) {
	return FREQ_DOM_BUFLEN;
}


