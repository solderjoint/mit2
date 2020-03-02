#ifndef __CRASHROUTINE_H
#define __CRASHROUTINE_H

//#include "util/typedefs.h"

/* **************************************************** *
 *                HEADER SPECIFIC MACROS
 * **************************************************** */

/* **************************************************** *
 *                 FUNCTION PROTOTYPES
 * **************************************************** */
void StateVoltageNormalSet(void);
void StateEndpointCheck(void);
void StateVoltageCheck(void);
void StateSmolinCheck (void);

int StateLineCheck (void);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__CRASHROUTINE_H
