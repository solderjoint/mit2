/*
 * Header for line status variable manipulation
 */

#ifndef __VAR_RELAYSTATUS_H
#define __VAR_RELAYSTATUS_H

#include "util/typedefs.h"

/* **************************************************** *
 *                  SYSTEM-WIDE MACROS
 * **************************************************** */
enum relayStatusEnum {
	RELAY_BROKEN = -1,
	RELAY_OFF    = 0,
	RELAY_ON     = 1,
};

/* **************************************************** *
 *             SECURE LINE STATUS VARIABLE
 * **************************************************** */
int32 RelayStatusSet (const int32 state);
int32 RelayStatusGet (void) ;

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__VAR_RELAYSTATUS_H
