/*
 * global mutex dispatcher
 */

#ifndef __VAR_MUTEX_H
#define __VAR_MUTEX_H

#include "util/typedefs.h"

/* **************************************************** *
 *                  SYSTEM-WIDE MACROS
 * **************************************************** */
enum mutexEnum {
	MUTEX_VOLTCHECK	 = 1,
	MUTEX_ENDPCHECK,
	MUTEX_STATECHECK,
	MUTEX_COMMCHECK,
	MUTEX_VOLTUPDATE,
	_MUTEX_MAXPOS    = 31,
	_MUTEX_MAXNUM    = 0x7FFFFFFF, // removed 31st bit
};

/* **************************************************** *
 *                 FUNCTION PROTOTYPES
 * **************************************************** */
int32 MutexInit (void);
int32 MutexCheckPending (void);
int32 MutexGet (enum mutexEnum var_pos);
int32 MutexClear (enum mutexEnum var_pos);
char MutexGetAndClear (enum mutexEnum var_pos);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__VAR_MUTEX_H
