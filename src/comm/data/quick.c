
#include "comm/data/quick.h"

#include "vars/linestatus.h"
#include "vars/relaystatus.h"

/* **************************************************** *
 *        SMOLIN QUICK INCOMING DATA PROCESSING
 * **************************************************** */
void QuickInputProcess (const uint8 buf[8]) {
	// line state (0 ok, >1 error)
	if (buf[0] != LineStatusGet()) LineStatusSet(buf[0]);
	// relay state (1 on, 0 off, -1 broken)
	(buf[1] > 0)? RelayStatusReset() : RelayStatusSet();
	// ..
	// ..
	// line reaction time delay (ms)
	// line mean voltage capture state
	// ..
	// ..

}

/* **************************************************** *
 *        SMOLIN QUICK OUTGOING DATA PREPARATION
 * **************************************************** */
void QuickOutputPrepare (uint8 buf[8]) {
	buf[0] = (uint8)LineStatusGet();    // line state
	buf[1] = (uint8)RelayStatusGet();   // relay state
//	buf[2] = (uint8)FoundDomainGet(1);  // prev sensor number
//	buf[3] = (uint8)FoundDomainGet(0);  // curr sensor number
	buf[4] = 0;                  // (line volt_ac[1])
	buf[5] = 0;                  // (line volt_ac[0])
//	const int32 v = (1000 * LineVoltageGet()) & 0xFFFF;
//	buf[6] = (uint8)(v >> 8);    // line volt_dc[1]
//	buf[7] = (uint8)(v && 0xFF); // line volt_dc[0]
}

