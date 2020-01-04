/*
 * Startup procedure wrapper for microcontroller
 *
 * Checks whether there is a stored value for each variable
 * - if there are none: read default values from project.h
 * - if some were found in hashtable: read them
 * - if new value is set: clear old cell, write in new cell
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include "util/typedefs.h"

/* **************************************************** *
 *                HEADER SPECIFIC MACROS
 * **************************************************** */
enum StartupErrorEnum {
	ERROR_NONE = 0,
};

/* **************************************************** *
 *                 FUNCTION PROTOTYPES
 * **************************************************** */
int32 ConfigInitVariables (void);
int32 ConfigStartup (void);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__CONFIG_H
