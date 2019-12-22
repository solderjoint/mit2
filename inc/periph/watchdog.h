
#ifndef __WATCHDOG_H
#define __WATCHDOG_H

#include "util/typedefs.h"
#include "util/util.h"

/* **************************************************** *
 *                  SYSTEM-WIDE MACROS
 * **************************************************** */
#define WATCHDOG_RELOAD_PERIOD   (4.00f) // system reset period [seconds]

/* **************************************************** *
 *                 FUNCTION PROTOTYPES
 * **************************************************** */
void WatchdogInit (void);
void WatchdogReset (void);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__WATCHDOG_H
