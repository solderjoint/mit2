
#include "vars/voltageconsts.h"
#include "project.h"

/* **************************************************** *
 *          LINE AND FREQ VOLTAGE MULTIPLIERS
 * **************************************************** */
static float voltageACMult;
float VoltageACMultGet (void) { return voltageACMult; }
void VoltageACMultSet (const float x) {
	if ((x > 0.0f) && (x < 1.0f)) voltageACMult = x;
}

static float voltageDCMult;
float VoltageDCMultGet (void) { return voltageDCMult; }
void VoltageDCMultSet (const float x) {
	if ((x > 0.0f) && (x < 1.0f)) voltageDCMult = x;
}

/* **************************************************** *
 *           LINE VOLTAGE COMPARISON SETTINGS
 * **************************************************** */
static float voltageSpike;
float VoltageSpikeGet (void) { return voltageSpike; }
void VoltageSpikeSet (const float x) {
	if ((x > 0.0f) && (x < 50.f)) voltageSpike = x;
}

static float voltageUnderflow;
float VoltageUnderflowGet (void) { return voltageUnderflow; }
void VoltageUnderflowSet (const float x) {
	if ((x > 0.0f) && (x < VoltageOverflowGet())) voltageUnderflow = x;
}

static float voltageOverflow;
float VoltageOverflowGet (void) { return voltageOverflow; }
void VoltageOverflowSet (const float x) {
	if ((x > 0.0f) && (x > VoltageUnderflowGet())) voltageOverflow = x;
}

static float voltageNormal;
float VoltageNormalGet (void) { return voltageNormal; }
void VoltageNormalSet (const float x) {
	if ((x > VoltageUnderflowGet()) && (x < VoltageOverflowGet()))
		voltageNormal = x;
}
