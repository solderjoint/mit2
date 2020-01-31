
#include "vars/mutex.h"

#include <string.h>

#include "periph/timer.h"

#include "vars/period.h"

#include "util/util.h"

/* **************************************************** *
 *            MUTEX LOCAL VARIABLE UTILITIES
 * **************************************************** */
static volatile int32 mutex_buf;

inline static void mutex_check (const int32 time, \
		const int32 comparison, enum mutexEnum var_pos) {
	if (time % comparison == 0) mutex_buf |= (1 << var_pos);
}

/* **************************************************** *
 *               MUTEX INTERRUPT WRAPPER
 * **************************************************** */
static void _mutex_handler (void) {
	const int32 time = PeriodCounterGet();
	const int32 line = PeriodLineVoltCheckGet();
	const int32 volt = PeriodLineVoltUpdateGet();
//	const int32 comm = PeriodCommCheckGet();

	mutex_check(time, line, MUTEX_LINECHECK);
//	mutex_check(time, comm, MUTEX_COMMCHECK);
	mutex_check(time, volt, MUTEX_VOLTUPDATE);

	PeriodCounterIncrement();
}

/* **************************************************** *
 *                GLOBAL MUTEX WRAPPER
 * **************************************************** */
int32 MutexInit (void) {
	mutex_buf = ((int32) 0);
	TimerSemaphoreAttachInterrupt(_mutex_handler);
}

inline int32 MutexCheckPending (void) {
	// returns 0 if no mutex is pending
	return mutex_buf & _MUTEX_MAXNUM;
}

inline int32 MutexGet (enum mutexEnum var_pos) {
	_check (var_pos < _MUTEX_MAXNUM);
	const int32 shift = (1 << (var_pos));
	return mutex_buf & shift;
}

inline int32 MutexClear (enum mutexEnum var_pos) {
	_check (var_pos < _MUTEX_MAXNUM);
	mutex_buf &= ~(1 << (var_pos));
	return mutex_buf;
}
