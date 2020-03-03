
#include "logic/data/quick.h"

#include "logic/math/linecheck.h"

#include "vars/buzzerdomain.h"
#include "vars/founddomain.h"
#include "vars/linestatus.h"
#include "vars/relaystatus.h"

/* **************************************************** *
 *        SMOLIN QUICK INCOMING DATA PROCESSING
 * **************************************************** */
int32 QuickInputProcess (const uint8 buf[8]) {
	/*
	 * removed processing of input until further notice
	 *
	 * changing internal state without restrictions may
	 * lead to disastterous situations when crash could
	 * not be registred, because its state is always
	 * freely overwritten by input message data
	 *
	 */
	return 0;
}

/* **************************************************** *
 *        SMOLIN QUICK OUTGOING DATA PREPARATION
 * **************************************************** */
static int32 counter;

int32 QuickOutputProcess (uint8 buf[8]) {
	buf[0] = (uint8)LineStatusGet();    // line state
	buf[1] = (uint8)RelayStatusGet();   // relay state

	// return found (if any) buzzer numbers in the system
	buf[2] = BuzzerDomainGetNumByFreq(FoundDomainGetByCounter(0));
	buf[3] = BuzzerDomainGetNumByFreq(FoundDomainGetByCounter(1));

	const int32 vac = counter++;  // message counter value
//	const int32 vac = 1000 * CheckLineVoltageMeanGet();
	buf[4] = (uint8)(vac & 0xFF);       // (line volt_ac[0])
	buf[5] = (uint8)((vac >> 8) & 0xFF);// (line volt_ac[1])

	const int32 vdc = 1000 * CheckLineVoltageMeanGet();
	buf[6] = (uint8)(vdc & 0xFF);       // line volt_dc[0]
	buf[7] = (uint8)((vdc >> 8) & 0xFF);// line volt_dc[1]
	return 0;
}

