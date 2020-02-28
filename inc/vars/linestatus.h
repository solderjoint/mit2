/*
 * Header for line status variable manipulation
 */

#ifndef __VAR_LINESTATUS_H
#define __VAR_LINESTATUS_H

#include "util/typedefs.h"

/* **************************************************** *
 *                  SYSTEM-WIDE MACROS
 * **************************************************** */
enum lineStatusEnum {
	STATUS_OK = 0,
	STATUS_VOLTSPIKE = 1,
	STATUS_SAMPLING = 2,
	STATUS_PROCESSING = 4,
	STATUS_SIGNALLOST = 8,
	STATUS_UNDERFLOW = 16,
	STATUS_OVERFLOW = 32,
	STATUS_BUZZERFOUND = 64,
};

/* **************************************************** *
 *             SECURE LINE STATUS VARIABLE
 * **************************************************** */
int32 LineStatusSet (enum lineStatusEnum val);
int32 LineStatusGet (void) ;
// TODO: add line voltage?
/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__VAR_LINESTATUS_H
