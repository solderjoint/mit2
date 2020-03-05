
#include "vars/mutex.h"

#include <string.h>

#include "periph/timer.h"

#include "vars/period.h"

#include "util/util.h"

/* **************************************************** *
 *            MUTEX LOCAL VARIABLE UTILITIES
 * **************************************************** */
static volatile uint32 mutex_buf;

static void mutex_check (const uint32 time, \
		const uint32 compare, enum mutexEnum var_pos) {
	if ((compare != 0) && ((time % compare) == 0)) {
		mutex_buf |= (1 << var_pos);
	}
}

/* **************************************************** *
 *               MUTEX INTERRUPT WRAPPER
 * **************************************************** */
//#include "periph/gpio.h"
static void _mutex_handler (void) {
	const uint32 time = PeriodCounterGet();

//	const uint32 comm = PeriodCommCheckGet();
	const uint32 endp = PeriodEndpointCheckGet();
	const uint32 volt = PeriodLineVoltCheckGet();
	const uint32 vupd = PeriodLineVoltUpdateGet();
	const uint32 line = PeriodLineStateUpdateGet();

//	mutex_check(time, comm, MUTEX_COMMCHECK);
	mutex_check(time, endp, MUTEX_ENDPCHECK);
	mutex_check(time, volt, MUTEX_VOLTCHECK);
	mutex_check(time, vupd, MUTEX_VOLTUPDATE);
	mutex_check(time, line, MUTEX_STATECHECK);

	PeriodCounterIncrement();
}

/* **************************************************** *
 *                GLOBAL MUTEX WRAPPER
 * **************************************************** */
int32 MutexInit (void) {
	mutex_buf = ((int32) 0);  // clear mutex buf variable
	TimerSemaphoreAttachInterrupt(_mutex_handler);
}

inline char MutexCheckPending (void) {
	// returns 0 if no mutex is pending
	return (mutex_buf != 0);
}

inline char MutexGet (enum mutexEnum var_pos) {
//	_check (var_pos < _MUTEX_MAXPOS);
	const int32 result = mutex_buf & ((1 << var_pos) & 0x7FFFFFFF);
	return (result != 0);
}

inline int32 MutexClear (enum mutexEnum var_pos) {
//	_check (var_pos < _MUTEX_MAXPOS);
	mutex_buf &= ~((1 << var_pos) & 0x7FFFFFFF);
	return mutex_buf;
}

inline char MutexGetAndClear (enum mutexEnum var_pos) {
	const int32 state = MutexGet(var_pos);
	if (state) MutexClear(var_pos);
	return state;
}
