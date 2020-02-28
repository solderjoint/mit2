
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
	if ((comparison <= 0) || (comparison > 0xFFFFFF)) return;
	else if (time % comparison == 0) mutex_buf |= (1 << var_pos);
}

/* **************************************************** *
 *               MUTEX INTERRUPT WRAPPER
 * **************************************************** */
static void _mutex_handler (void) {
	const int32 time = PeriodCounterGet();

	const int32 comm = PeriodCommCheckGet();
	const int32 endp = PeriodLineEndpointCheckGet();
	const int32 volt = PeriodLineVoltCheckGet();
	const int32 line = PeriodLineStateUpdateGet();
	const int32 vupd = PeriodLineVoltUpdateGet();

	mutex_check(time, comm, MUTEX_COMMCHECK);
	mutex_check(time, endp, MUTEX_ENDPCHECK);
	mutex_check(time, volt, MUTEX_VOLTCHECK);
	mutex_check(time, line, MUTEX_STATECHECK);
	mutex_check(time, vupd, MUTEX_VOLTUPDATE);

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

inline char MutexGetAndClear (enum mutexEnum var_pos) {
	const int32 state = MutexGet(var_pos);

	if (state > 0) {
		MutexClear(var_pos);
		return 1;
	} else {
		return 0;
	}
}
