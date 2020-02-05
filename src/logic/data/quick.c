
#include "logic/data/quick.h"

#include "vars/linestatus.h"
#include "vars/relaystatus.h"

/* **************************************************** *
 *        SMOLIN QUICK INCOMING DATA PROCESSING
 * **************************************************** */
int32 QuickInputProcess (const uint8 buf[8]) {
	// line state (0 ok, >1 error)
	if (buf[0] != LineStatusGet()) LineStatusSet(buf[0]);
	// relay state (1 on, 0 off, -1 broken)
	(buf[1] > 0)? RelayStatusSet() : RelayStatusReset();
	// ..
	// ..
	// line reaction time delay (ms)
	// line mean voltage capture state
	// ..
	// ..
	return 0;
}

/* **************************************************** *
 *        SMOLIN QUICK OUTGOING DATA PREPARATION
 * **************************************************** */
int32 QuickOutputProcess (uint8 buf[8]) {
	buf[0] = (uint8)LineStatusGet();    // line state
	buf[1] = (uint8)RelayStatusGet();   // relay state
//	buf[2] = (uint8)FoundDomainGet(1);  // prev sensor number
//	buf[3] = (uint8)FoundDomainGet(0);  // curr sensor number
	const int32 vac = (1000 * (1.23456f));
	buf[4] = (uint8)((vac >> 8) & 0xFF);// (line volt_ac[1])
	buf[5] = (uint8)(vac & 0xFF);       // (line volt_ac[0])
	const int32 vdc = (1000 * (12.3456f));
	buf[6] = (uint8)((vdc >> 8) & 0xFF);// line volt_dc[1]
	buf[7] = (uint8)(vdc & 0xFF);       // line volt_dc[0]
	return 0;
}

