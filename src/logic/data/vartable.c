
#include "logic/data/vartable.h"

#include <stddef.h> // for NULL reference
#include <string.h>

#include "logic/comm/mbus.h"

#include "vars/linestatus.h"
#include "vars/relaystatus.h"

/* **************************************************** *
 *         MODICON BUS VARIABLES TABLE READING
 * **************************************************** */
#include "periph/gpio.h"

#include "logic/math/domain.h"

#include "vars/fourierconsts.h"
#include "vars/linestatus.h"
#include "vars/mutex.h"
#include "vars/peripheral.h"
#include "vars/period.h"
#include "vars/voltageconsts.h"

#include "util/util.h"

// CAUTION! A VERY FUCKING LONG SWITCH CASE LIST AHEAD!
int32 ConfigVarTableGet (const int32 address) {
	int32 res;
	switch (address & 0xFFFF) {


		/* **************************************************** */
		case VAR_LINE_STATUS: res = LineStatusGet(); break;
		case VAR_RELAY_STATUS: res = RelayStatusGet(); break;
		case VAR_DEVICEID: res = GpioModuleAdressGet() | (4 << 3); break;
		case VAR_UPTIME1: res = _time() >> 16; break;
		case VAR_UPTIME0: res = _time() & 0xFFFF; break;

		case VAR_PERIPH_LEDSTATE: res = GpioLedsPortGet(); break;
		case VAR_PERIPH_CANRATE1: res = PeripheralCanRateGet() >> 16; break;
		case VAR_PERIPH_CANRATE0: res = PeripheralCanRateGet()&0xFFFF; break;
		case VAR_PERIPH_SPIRATE1: res = PeripheralSpiRateGet() >> 16; break;
		case VAR_PERIPH_SPIRATE0: res = PeripheralSpiRateGet()&0xFFFF; break;
		case VAR_PERIPH_UARTRATE1: res = PeripheralUartRateGet() >> 16; break;
		case VAR_PERIPH_UARTRATE0: res = PeripheralUartRateGet()&0xFFFF; break;

		case VAR_MUTEX_PERIOD: res = MutexCheckPending() & 0xFFFF; break;
		case VAR_PERIOD_LINECHECK: res = PeriodLineVoltCheckGet(); break;
		case VAR_PERIOD_VOLTUPD: res = PeriodLineVoltUpdateGet(); break;
		case VAR_PERIOD_COMMCHECK: res = PeriodCommCheckGet(); break;

		case VAR_VOLTAC_MULT1:
			res = ((int) bil(VoltageACMultGet())) >> 16; break;
		case VAR_VOLTAC_MULT0:
			res = ((int) bil(VoltageACMultGet())) & 0xFFFF; break;
		case VAR_VOLTDC_MULT1:
			res = ((int) bil(VoltageDCMultGet())) >> 16; break;
		case VAR_VOLTDC_MULT0:
			res = ((int) bil(VoltageDCMultGet())) & 0xFFFF; break;

		case VAR_VOLT_SPIKE: res = (int) kil(VoltageSpikeGet()); break;
		case VAR_VOLT_NORMAL: res = (int) kil(VoltageNormalGet()); break;
		case VAR_VOLT_LINEBREACH: res = (int) kil(VoltageOverflowGet()); break;
		case VAR_VOLT_LINESHORT: res = (int) kil(VoltageUnderflowGet()); break;

		case VAR_FOURIER_OVERSAMPLES: res = FourierOversamplesGet(); break;
		case VAR_FOURIER_SAMPLERATE: res = FourierSamplingRateGet(); break;
		case VAR_FOURIER_VOLTMULT1:
			res = ((int) bil(VoltageACMultGet())) >> 16; break;
		case VAR_FOURIER_VOLTMULT0:
			res = ((int) bil(VoltageACMultGet())) & 0xFFFF; break;
		case VAR_FOURIER_FREQMIN: res = FourierFreqRangeMinGet(); break;
		case VAR_FOURIER_FREQMAX: res = FourierFreqRangeMaxGet(); break;
		case VAR_FOURIER_FREQREF: res = FourierFreqRefGet(); break;


		case VAR_SEARCHBUF_SIZE: /*res = FoundDomainSizeGet();*/ break;

		case VAR_SEARCHBUF_NUM01:
		case VAR_SEARCHBUF_NUM02:
		case VAR_SEARCHBUF_NUM03:
		case VAR_SEARCHBUF_NUM04:
		case VAR_SEARCHBUF_NUM05:
		case VAR_SEARCHBUF_NUM06:
		case VAR_SEARCHBUF_NUM07:
		case VAR_SEARCHBUF_NUM08:
		case VAR_SEARCHBUF_NUM09:
		case VAR_SEARCHBUF_NUM10:
		case VAR_SEARCHBUF_NUM11:
		case VAR_SEARCHBUF_NUM12:
		case VAR_SEARCHBUF_NUM13:
		case VAR_SEARCHBUF_NUM14:
		case VAR_SEARCHBUF_NUM15:
		case VAR_SEARCHBUF_NUM16:
		case VAR_SEARCHBUF_NUM17:
		case VAR_SEARCHBUF_NUM18:
		case VAR_SEARCHBUF_NUM19:
		case VAR_SEARCHBUF_NUM20:
		case VAR_SEARCHBUF_NUM21:
		case VAR_SEARCHBUF_NUM22:
		case VAR_SEARCHBUF_NUM23:
		case VAR_SEARCHBUF_NUM24:
		case VAR_SEARCHBUF_NUM25:
		case VAR_SEARCHBUF_NUM26:
		case VAR_SEARCHBUF_NUM27:
		case VAR_SEARCHBUF_NUM28:
		case VAR_SEARCHBUF_NUM29:
		case VAR_SEARCHBUF_NUM33:
		case VAR_SEARCHBUF_NUM31:
		case VAR_SEARCHBUF_NUM32:
			res = FoundDomainGetByCounter( \
						(address & 0xFFFF) - VAR_SEARCHBUF_NUM01); break;


		default: res = MBUS_ERROR_ADDRESS; break;
	}
	return res;
}

//float ConfigVarTableGetFloat (const int32 address) {

//}

/* **************************************************** *
 *         MODICON BUS VARIABLES TABLE HANDLING
 * **************************************************** */
int32 ConfigVarTableSet (const int32 address, const int16 value) {
	int32 res;
	switch (address & 0xFFFF) {
	case VAR_LINE_STATUS:
		LineStatusSet(value & 0xFF);
		res = LineStatusGet();
		break;
	case VAR_RELAY_STATUS:
		RelayStatusSet((value > 0)? RELAY_ON : RELAY_OFF);
		res = RelayStatusGet();
		break;

	default:
		res = MBUS_ERROR_ADDRESS;
		break;
	}
	return res;
}
