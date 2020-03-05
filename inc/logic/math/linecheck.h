
#ifndef __LINECHECK_H
#define __LINECHECK_H

#include "util/typedefs.h"
#include "util/util.h"
//#include "project.h"

/* **************************************************** *
 *                  SYSTEM-WIDE MACROS
 * **************************************************** */
enum linecheckEnum {
	LINE_VOLTBUF_SIZE   = 9,     // voltage buffer size
	LINE_SIGNALBUF_SIZE = 5,     // endpoint signal buffer size
};

/* **************************************************** *
 *                 FUNCTION PROTOTYPES
 * **************************************************** */
int32 CheckEndpointSignalLoss (void);
int32 CheckLineVoltageSpike (void);
int32 CheckLineVoltageOverflow (void);

void CheckLineEndpointSignalRenew (void);
void CheckLineVoltageBufferRenew (void);
float CheckLineVoltageMeanGet (void);
void CheckLineVoltageNormalUpdate (void);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__LINECHECK_H
