
#include "vars/period.h"

#include <string.h>

#include "logic/data/db.h"
#include "util/util.h"
#include "project.h"

/* **************************************************** *
 *              LOCAL VARIABLE NAME BUFFER
 * **************************************************** */
static char var_name[64];

/* **************************************************** *
 *              SEMAPHORE COUNTER WRAPPER
 * **************************************************** */
static uint32 period_counter;

void PeriodCounterIncrement (void) { period_counter++; }
uint32 PeriodCounterGet (void) { return period_counter; }

/* **************************************************** *
 *          TIMER SEMAPHORE FREQUENCY VARIABLE
 * **************************************************** */
int32 PeriodSemaphoreFreqGet (void) {
	*var_name = stringify(VarSemaphoreFreq);
	const uint32 hash = DatabaseHashGet(var_name);
	const int32 val = DatabaseValueGet(hash);
	return ((val < 10) || (val > 1000))? PERIOD_TIMER_FREQ : val;
}

int32 PeriodSemaphoreFreqSet (const uint32 val) {
	const uint32 rate = ((val < 10) || (val > 1000))? PERIOD_TIMER_FREQ : val;
	const char buf[] = stringify(VarSemaphoreFreq);
	const uint32 hash = DatabaseHashGet(buf);
	return DatabaseValueSet(hash, rate);
}

/* **************************************************** *
 *              LINE VOLTAGE CHECK PERIOD
 * **************************************************** */
static int32 volt_check;

void PeriodLineVoltCheckInit (void) {
	*var_name = stringify(PeriodLineVoltCheck);
	const uint32 hash = DatabaseHashGet(var_name);
	const int32 val = DatabaseValueGet(hash);
	volt_check = (val > 100)? PERIOD_CHECK_LINE : val;
}

int32 PeriodLineVoltCheckGet (void) {
	const int32 val = volt_check;
	return ((val < 1) || (val > 100))? PERIOD_CHECK_LINE : val;
}

int32 PeriodLineVoltCheckSet (const uint32 val) {
	const uint32 rate = (val > 100)? PERIOD_CHECK_LINE : val;
	const char buf[] = stringify(PeriodLineVoltCheck);
	const uint32 hash = DatabaseHashGet(buf);
	return DatabaseValueSet(hash, rate);
}

/* **************************************************** *
 *          LINE VOLTAGE NORMAL UPDATE PERIOD
 * **************************************************** */
static int32 volt_update;

void PeriodLineVoltUpdateInit (void) {
	*var_name = stringify(PeriodLineVoltUpdate);
	const uint32 hash = DatabaseHashGet(var_name);
	const int32 val = DatabaseValueGet(hash);
	volt_update = ((val < 1000) || (val > kil(100)))
						 ? PERIOD_RENEW_VOLT : val;
}

int32 PeriodLineVoltUpdateGet (void) {
	const int32 val = volt_update;
	return ((val < 1) || (val > 100))? PERIOD_RENEW_VOLT : val;
}

int32 PeriodLineVoltUpdateSet (const uint32 val) {
	const uint32 rate = ((val >= kil(1)) && (val <= kil(100)))
						? PERIOD_RENEW_VOLT : val;
	const char buf[] = stringify(PeriodLineVoltUpdate);
	const uint32 hash = DatabaseHashGet(buf);
	return DatabaseValueSet(hash, rate);
}

/* **************************************************** *
 *         COMMUNICATION UPDATE PERIOD WRAPPER
 * **************************************************** */
static int32 comm_check;

void PeriodCommCheckInit (void) {
	*var_name = stringify(PeriodCommCheck);
	const uint32 hash = DatabaseHashGet(var_name);
	const int32 val = DatabaseValueGet(hash);
	comm_check = ((val < 1000) || (val > kil(100)))
						 ? PERIOD_CHECK_COMM : val;
}

int32 PeriodCommCheckGet (void) {
	const int32 val = comm_check;
	return ((val < 1) || (val > 100))? PERIOD_CHECK_COMM : val;
}

int32 PeriodCommCheckSet (const uint32 val) {
	const uint32 rate = ((val >= kil(1)) && (val <= kil(100)))
						? PERIOD_RENEW_VOLT : val;
	const char buf[] = stringify(PeriodCommCheck);
	const uint32 hash = DatabaseHashGet(buf);
	return DatabaseValueSet(hash, rate);
}

