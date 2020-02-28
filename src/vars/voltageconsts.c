
#include "vars/voltageconsts.h"
#include "project.h"

/* **************************************************** *
 *          LINE AND FREQ VOLTAGE MULTIPLIERS
 * **************************************************** */
static int16 voltac_counts;
int32 VoltageACAdcCountsGet (void) {
	return voltac_counts;
}
void VoltageACAdcCountsSet (const int32 x) {
	if ((x > 511) && (x < 8193)) voltac_counts = x;
	else voltac_counts = VOLTAGE_AC_ADCCOUNTS;
}

static float voltac_max;
float VoltageACMaxGet (void) {
	return voltac_max;
}
void VoltageACMaxSet (const float x) {
	if ((x > 1.0f) && (x < 10.f)) voltac_max = x;
	else voltac_max = VOLTAGE_AC_MAXVOLT / 1000.f;
}

float VoltageACMultGet (void) { return voltac_max / ((float) voltac_counts); }

/* **************************************************** */
static int16 voltdc_counts;
int32 VoltageDCAdcCountsGet (void) {
	return voltdc_counts;
}
void VoltageDCAdcCountsSet (const int32 x) {
	if ((x > 511) && (x < 8193)) voltdc_counts = x;
	else voltdc_counts = VOLTAGE_DC_ADCCOUNTS;
}

static float voltdc_max;
float VoltageDCMaxGet (void) {
	return voltdc_max;
}
void VoltageDCMaxSet (const float x) {
	if ((x > 1.0f) && (x < 50.f)) voltdc_max = x;
	else voltdc_max = VOLTAGE_DC_MAXVOLT / 1000.f;
}

float VoltageDCMultGet (void) { return voltdc_max / ((float) voltdc_counts); }

/* **************************************************** *
 *           LINE VOLTAGE COMPARISON SETTINGS
 * **************************************************** */
static float voltageSpike;
float VoltageSpikeGet (void) {
	return voltageSpike;
}
void VoltageSpikeSet (const float x) {
	if ((x > 0.0f) && (x < 50.f)) voltageSpike = x;
}

static float voltageUnderflow;
float VoltageUnderflowGet (void) {
	return voltageUnderflow;
}
void VoltageUnderflowSet (const float x) {
	if ((x > 0.0f) && (x < VoltageOverflowGet())) voltageUnderflow = x;
}

static float voltageOverflow;
float VoltageOverflowGet (void) {
	return voltageOverflow;
}
void VoltageOverflowSet (const float x) {
	if ((x > 0.0f) && (x > VoltageUnderflowGet())) voltageOverflow = x;
}

static float voltageNormal;
float VoltageNormalGet (void) {
	return voltageNormal;
}
void VoltageNormalSet (const float x) {
	if ((x > VoltageUnderflowGet()) && (x < VoltageOverflowGet()))
		voltageNormal = x;
}

/* **************************************************** *
 *            EXTERNAL DEFAULT VALUE SETTER
 * **************************************************** */
char VoltageConstantsInit (void) {
	voltac_counts = VOLTAGE_AC_ADCCOUNTS;
	voltac_max = VOLTAGE_AC_MAXVOLT / 1000.f;
	voltdc_counts = VOLTAGE_DC_ADCCOUNTS;
	voltdc_max = VOLTAGE_DC_MAXVOLT / 1000.f;

	voltageSpike = VOLTAGE_SPIKE / 1000.f;
	voltageUnderflow = VOLTAGE_UNDERFLOW / 1000.f;
	voltageOverflow = VOLTAGE_OVERFLOW / 1000.f;
	voltageNormal = VOLTAGE_NORMAL / 1000.f;
}
