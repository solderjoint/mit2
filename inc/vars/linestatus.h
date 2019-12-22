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
	STATUS_SIGNALLOST, STATUS_VOLTSPIKE,
	STATUS_SAMPLING, STATUS_PROCESSING,
	STATUS_UNDERFLOW, STATUS_OVERFLOW,
	STATUS_BUZZERFOUND, STATUS_DEVICEERROR
};

/* **************************************************** *
 *             SECURE LINE STATUS VARIABLE
 * **************************************************** */
int32 LineStatusSet (enum lineStatusEnum val);
int32 LineStatusGet (void) ;

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__VAR_LINESTATUS_H
