/*
 * Modicon Bus like variables table.
 * Every used ID is represented in included enumerator.
 */

#ifndef __DATA_VARTABLE_H
#define __DATA_VARTABLE_H

#include "util/typedefs.h"

/* **************************************************** *
 *                HEADER SPECIFIC MACROS
 * **************************************************** */

/* **************************************************** *
 *                 FUNCTION PROTOTYPES
 * **************************************************** */
int32 ConfigVarTableGet (const int32 address);
int32 ConfigVarTableSet (const int32 address, const int16 value);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__DATA_VARTABLE_H
