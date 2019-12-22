#ifndef __CRASHROUTINE_H
#define __CRASHROUTINE_H

#include "util/typedefs.h"

/* **************************************************** *
 *                HEADER SPECIFIC MACROS
 * **************************************************** */

/* **************************************************** *
 *                 FUNCTION PROTOTYPES
 * **************************************************** */
int32 CrashCheck (void);

void CrashUpdateNormalVoltage (void);
void CrashVarsReset (void);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__CRASHROUTINE_H
