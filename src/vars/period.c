﻿
#include "vars/period.h"

#include "util/util.h"
#include "project.h"

/* **************************************************** *
 *              SEMAPHORE COUNTER WRAPPER
 * **************************************************** */
static uint32 period_counter;

void PeriodCounterIncrement (void) { period_counter++; }
uint32 PeriodCounterGet (void) { return period_counter; }

/* **************************************************** *
 *          TIMER SEMAPHORE FREQUENCY VARIABLE
 * **************************************************** */
static int32 period_freq;

int32 PeriodSemaphoreFreqGet (void) {
	const int32 val = (period_freq > 10)? period_freq : PERIOD_TIMER_FREQ;
	return (val > 1000)? PERIOD_TIMER_FREQ : val;
}

int32 PeriodSemaphoreFreqSet (const uint32 val) {
	const int32 rate = ((val < 10) || (val > 1000))
			? PERIOD_TIMER_FREQ : val;
	period_freq = rate;
	period_counter = 0; // clear counter
	return period_freq;
}

/* **************************************************** *
 *              LINE ENDPOINT CHECK PERIOD
 * **************************************************** */
static int32 endp_check;

inline int32 PeriodLineEndpointCheckGet (void) {
	const int32 x = endp_check;
	return ((x < 1) || (x > 100))? PERIOD_CHECK_ENDPOINT : x;
}

int32 PeriodLineEndpointCheckSet (const uint32 val) {
	endp_check = ((val < 1) || (val > 100))? PERIOD_CHECK_ENDPOINT : val;
	return endp_check;
}

/* **************************************************** *
 *              LINE VOLTAGE CHECK PERIOD
 * **************************************************** */
static int32 line_check;

inline int32 PeriodLineVoltCheckGet (void) {
	const int32 x = line_check;
	return ((x < 1) || (x > 100))? PERIOD_CHECK_VOLT : x;
}

int32 PeriodLineVoltCheckSet (const uint32 val) {
	line_check = ((val < 1) || (val > 100))? PERIOD_CHECK_VOLT : val;
	return line_check;
}

/* **************************************************** *
 *          LINE VOLTAGE NORMAL UPDATE PERIOD
 * **************************************************** */
static int32 volt_update;

inline int32 PeriodLineVoltUpdateGet (void) {
	const int32 x = volt_update;
	return ((x < 100) || (x > mil(1)))? PERIOD_UPDATE_VOLT : x;
}

int32 PeriodLineVoltUpdateSet (const uint32 val) {
	volt_update = ((val >= 100) && (val <= mil(1))) \
				? PERIOD_UPDATE_VOLT : val;
	return volt_update;
}

/* **************************************************** *
 *           LINE GLOBAL STATE UPDATE PERIOD
 * **************************************************** */
static int32 state_update;

inline int32 PeriodLineStateUpdateGet (void) {
	const int32 x = state_update;
	return ((x < 100) || (x > mil(1)))? PERIOD_CHECK_STATE : x;
}

int32 PeriodLineStateUpdateSet (const uint32 val) {
	state_update = ((val < 10) || (val > mil(1))) \
				? PERIOD_CHECK_STATE : val;
	return state_update;
}

/* **************************************************** *
 *         COMMUNICATION UPDATE PERIOD WRAPPER
 * **************************************************** */
static int32 comm_check;

inline int32 PeriodCommCheckGet (void) {
	const int32 val = comm_check;
	return ((val < 100) || (val > mil(1)))? PERIOD_CHECK_COMM : val;
}

int32 PeriodCommCheckSet (const uint32 val) {
	const uint32 rate = ((val < 1) || (val > 1000))
				? PERIOD_CHECK_COMM : val;
	comm_check = rate;
	return comm_check;
}

/* **************************************************** *
 *            EXTERNAL DEFAULT VALUE SETTER
 * **************************************************** */
char PeriodConstantsInit (void) {
	period_counter = 0; // clear the counter
	period_freq = PERIOD_TIMER_FREQ;
	comm_check = PERIOD_CHECK_COMM;
	line_check = PERIOD_CHECK_STATE;
	volt_update = PERIOD_UPDATE_VOLT;
}
