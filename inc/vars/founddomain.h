/*
 * Lost&found frequencies and buzzers domain
 */

#ifndef __VAR_FOUNDDOMAIN_H
#define __VAR_FOUNDDOMAIN_H

#include "util/typedefs.h"

/* **************************************************** *
 *                  SYSTEM-WIDE MACROS
 * **************************************************** */
enum foundDomainEnum {
	FOUND_BUFLEN   = 32,
	FOUND_FREQ_MAX = (1 << 15),
	FOUND_FREQ_MIN = (1 << 10),
	FOUND_NUM_MAX  = 32,
};

/* **************************************************** *
 *                 FUNCTION PROTOTYPES
 * **************************************************** */
void FoundDomainReset (void);
void FoundDomainSet (const uint32 freq);
void FoundDomainClear (const uint32 ptr);

int32 FoundDomainGetByCounter (const uint32 counter);
char DomainResultIsNotEmpty (void);
int32 FoundDomainGetLengthMax (void);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__VAR_FOUNDDOMAIN_H
