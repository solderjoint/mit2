#ifndef __CRASHROUTINE_H
#define __CRASHROUTINE_H

//#include "util/typedefs.h"

/* **************************************************** *
 *                HEADER SPECIFIC MACROS
 * **************************************************** */

/* **************************************************** *
 *                 FUNCTION PROTOTYPES
 * **************************************************** */
int StateVoltageNormalSet (void);
int StateEndpointCheck (void);
int StateVoltageCheck (void);

int StateLineCheck (void);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__CRASHROUTINE_H
