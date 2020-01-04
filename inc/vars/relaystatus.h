/*
 * Header for line status variable manipulation
 */

#ifndef __VAR_RELAYSTATUS_H
#define __VAR_RELAYSTATUS_H

#include "util/typedefs.h"

/* **************************************************** *
 *                  SYSTEM-WIDE MACROS
 * **************************************************** */

/* **************************************************** *
 *             SECURE LINE STATUS VARIABLE
 * **************************************************** */
int32 RelayStatusSet (void);
int32 RelayStatusReset (void);
int32 RelayStatusGet (void) ;

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__VAR_RELAYSTATUS_H
