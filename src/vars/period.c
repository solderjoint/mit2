
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
static int32 period_volt_check;

void PeriodLineVoltCheckInit (void) {
	*var_name = stringify(PeriodLineVoltCheck);
	const uint32 hash = DatabaseHashGet(var_name);
	const int32 val = DatabaseValueGet(hash);
	period_volt_check = (val > 100)? PERIOD_CHECK_LINE : val;
}

int32 PeriodLineVoltCheckGet (void) {
	const int32 res = period_volt_check;
	if ((res >= 1) && (res < 100)) {
		return res;
	} else {
		PeriodLineVoltCheckInit();
		return period_volt_check;
	}
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
static int32 period_volt_update;

void PeriodLineVoltUpdateInit (void) {
	*var_name = stringify(PeriodLineVoltUpdate);
	const uint32 hash = DatabaseHashGet(var_name);
	const int32 val = DatabaseValueGet(hash);
	period_volt_update = ((val < 1000) || (val > kil(100)))
						 ? PERIOD_RENEW_VOLT : val;
}

int32 PeriodLineVoltUpdateGet (void) {
	const int32 res = period_volt_update;
	if ((res >= kil(1)) && (res <= kil(100))) {
		return res;
	} else {
		PeriodLineVoltUpdateInit();
		return period_volt_update;
	}
}

int32 PeriodLineVoltUpdateSet (const uint32 val) {
	const uint32 rate = ((val >= kil(1)) && (val <= kil(100)))
						? PERIOD_RENEW_VOLT : val;
	const char buf[] = stringify(PeriodLineVoltUpdate);
	const uint32 hash = DatabaseHashGet(buf);
	return DatabaseValueSet(hash, rate);
}

/* **************************************************** *
 *               USART PERIPHERAL WRAPPER
 * **************************************************** */
