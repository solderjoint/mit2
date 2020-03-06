
#include "logic/data/vartable.h"

#include <locale.h>
#include <stddef.h> // for NULL reference
#include <stdlib.h>
//#include <string.h>

#include "logic/comm/mbus.h"

#include "vars/linestatus.h"
#include "vars/relaystatus.h"

/* **************************************************** *
 *         MODICON BUS VARIABLES TABLE READING
 * **************************************************** */
#include "periph/gpio.h"
#include "periph/timer.h"

#include "logic/math/linecheck.h"

#include "vars/buzzerdomain.h"
#include "vars/canmessage.h"
#include "vars/founddomain.h"
#include "vars/fourierconsts.h"
#include "vars/linestatus.h"
#include "vars/mutex.h"
#include "vars/period.h"
#include "vars/peripheral.h"
#include "vars/relaystatus.h"
#include "vars/voltageconsts.h"

#include "util/util.h"

// CAUTION! A VERY FUCKING LONG SWITCH CASE LIST AHEAD!
int32 VarTableGet (const int32 address) {
	int32 err = 0, res = 0, temp;
	const int32 offset = address & 0xFFFF;

	switch (offset) {
	/* **************************************************** */
	case VAR_LINE_VOLTDC1:
		res = mil(CheckLineVoltageMeanGet());
		res = (res >> 16) & 0xFFFF;
		break;
	case VAR_LINE_VOLTDC0:
		res = mil(CheckLineVoltageMeanGet());
		res &= 0xFFFF;
		break;
	case VAR_LINE_VOLTAC1:
		res = mil(CheckLineVoltageMeanGet());
		res = (res >> 16) & 0xFFFF;
		break;
	case VAR_LINE_VOLTAC0:
		res = mil(CheckLineVoltageMeanGet());
		res &= 0xFFFF;
		break;

	case VAR_MUTEX_REGISTER: res = MutexCheckPending(); break;
	case VAR_MUTEX_COMM: res = MutexGet(MUTEX_COMMCHECK); break;
	case VAR_MUTEX_VOLTCHECK: res = MutexGet(MUTEX_VOLTCHECK); break;
	case VAR_MUTEX_ENDPOINT: res = MutexGet(MUTEX_ENDPCHECK); break;
	case VAR_MUTEX_LINESTATE: res = MutexGet(MUTEX_STATECHECK); break;
	case VAR_MUTEX_VOLTUPDATE: res = MutexGet(MUTEX_VOLTUPDATE); break;

	case VAR_FOURIER_BUFSIZE: res = FourierBufferLengthGet(); break;
	case VAR_FOURIER_FREQRES: res = kil(FourierFreqResolutionGet()); break;

	/* **************************************************** */
	case VAR_FOUNDBUF_SIZE: res = FoundDomainGetLength(); break;

	case VAR_FOUNDBUF_NUM01:
	case VAR_FOUNDBUF_NUM02:
	case VAR_FOUNDBUF_NUM03:
	case VAR_FOUNDBUF_NUM04:
	case VAR_FOUNDBUF_NUM05:
	case VAR_FOUNDBUF_NUM06:
	case VAR_FOUNDBUF_NUM07:
	case VAR_FOUNDBUF_NUM08:
	case VAR_FOUNDBUF_NUM09:
	case VAR_FOUNDBUF_NUM10:
	case VAR_FOUNDBUF_NUM11:
	case VAR_FOUNDBUF_NUM12:
	case VAR_FOUNDBUF_NUM13:
	case VAR_FOUNDBUF_NUM14:
	case VAR_FOUNDBUF_NUM15:
	case VAR_FOUNDBUF_NUM16:
	case VAR_FOUNDBUF_NUM17:
	case VAR_FOUNDBUF_NUM18:
	case VAR_FOUNDBUF_NUM19:
	case VAR_FOUNDBUF_NUM20:
	case VAR_FOUNDBUF_NUM21:
	case VAR_FOUNDBUF_NUM22:
	case VAR_FOUNDBUF_NUM23:
	case VAR_FOUNDBUF_NUM24:
	case VAR_FOUNDBUF_NUM25:
	case VAR_FOUNDBUF_NUM26:
	case VAR_FOUNDBUF_NUM27:
	case VAR_FOUNDBUF_NUM28:
	case VAR_FOUNDBUF_NUM29:
	case VAR_FOUNDBUF_NUM33:
	case VAR_FOUNDBUF_NUM31:
	case VAR_FOUNDBUF_NUM32:
		res = BuzzerDomainGetNumByFreq( \
				FoundDomainGetByCounter(offset - VAR_FOUNDBUF_NUM01));
		break;

	/* **************************************************** */
	case VAR_FIRMWARE_REV1:
			setlocale(LC_ALL, "C");
#ifdef VERSION
			temp = atoi(VERSION);
#else
			temp = 1234;
#endif
			res = temp >> 16;
			break;
	case VAR_FIRMWARE_REV0:
			setlocale(LC_ALL, "C");
#ifdef VERSION
			temp = atoi(VERSION);
#else
			temp = 4321;
#endif
			res = temp & 0xFFFF;
			break;

	/* **************************************************** */
	case VAR_LINE_STATUS: res = LineStatusGet(); break;
	case VAR_RELAY_STATUS: res = RelayStatusGet(); break;
	case VAR_DEVICEID: res = CanMessageDeviceIdGet(); break;
	case VAR_UPTIME1: res = _time() >> 16; break;
	case VAR_UPTIME0: res = _time() & 0xFFFF; break;

	case VAR_PERIPH_LEDSTATE: res = GpioLedsPortGet(); break;
	case VAR_PERIPH_CANRATE1: res = PeriphCanRateGet() >> 16; break;
	case VAR_PERIPH_CANRATE0: res = PeriphCanRateGet()&0xFFFF; break;
	case VAR_PERIPH_SPIRATE1: res = PeriphSpiRateGet() >> 16; break;
	case VAR_PERIPH_SPIRATE0: res = PeriphSpiRateGet()&0xFFFF; break;
	case VAR_PERIPH_UARTRATE1: res = PeriphUartRateGet() >> 16; break;
	case VAR_PERIPH_UARTRATE0: res = PeriphUartRateGet()&0xFFFF; break;

	case VAR_MUTEX_PERIOD: res = MutexCheckPending() & 0xFFFF; break;
	case VAR_PERIOD_LINECHECK: res = PeriodLineVoltCheckGet(); break;
	case VAR_PERIOD_VOLTUPD: res = PeriodLineVoltUpdateGet(); break;
	case VAR_PERIOD_COMMCHECK: res = PeriodCommCheckGet(); break;
	case VAR_PERIOD_VOLTCHECK: res = PeriodLineVoltCheckGet(); break;
	case VAR_PERIOD_ENDPCHECK: res = PeriodEndpointCheckGet(); break;

	case VAR_VOLTAC_MAX:
		res = kil(VoltageACMaxGet()); break;
	case VAR_VOLTAC_ADCCOUNTS:
		res = VoltageACAdcCountsGet(); break;
	case VAR_VOLTDC_MAX:
		res = kil(VoltageDCMaxGet()); break;
	case VAR_VOLTDC_ADCCOUNTS:
		res = VoltageDCAdcCountsGet(); break;

	case VAR_VOLT_SPIKE: res = (int) kil(VoltageSpikeGet()); break;
	case VAR_VOLT_NORMAL: res = (int) kil(VoltageNormalGet()); break;
	case VAR_VOLT_LINEBREACH: res = (int) kil(VoltageOverflowGet()); break;
	case VAR_VOLT_LINESHORT: res = (int) kil(VoltageUnderflowGet()); break;

	case VAR_FOURIER_OVERSAMPLES: res = FourierSamplingPassesGet(); break;
	case VAR_FOURIER_SAMPLERATE: res = FourierSamplingRateGet(); break;
	case VAR_FOURIER_VOLTMULT1:
		res = ((int) bil(VoltageACMultGet())) >> 16; break;
	case VAR_FOURIER_VOLTMULT0:
		res = ((int) bil(VoltageACMultGet())) & 0xFFFF; break;
	case VAR_FOURIER_FREQMIN: res = FourierFreqRangeMinGet(); break;
	case VAR_FOURIER_FREQMAX: res = FourierFreqRangeMaxGet(); break;
	case VAR_FOURIER_FREQREF: res = FourierFreqReferenceGet(); break;


	/* **************************************************** */
	case VAR_SEARCHBUF_SIZE: res = BuzzerDomainGetLength(); break;

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
		res = BuzzerDomainGetFreqByNum(offset - VAR_SEARCHBUF_NUM01);
		break;

	default: err = MBUS_ERROR_ADDRESS; break;
	}

	// returns either a result or an error
	return (err == 0)? res : err;
}

/* **************************************************** *
 *         MODICON BUS VARIABLES TABLE HANDLING
 * **************************************************** */
int32 VarTableSet (const int32 address, const int32 value) {
	int32 err = 0, res = 0, temp;
	const int32 offset = address & 0xFFFF;
	const uint16 uval = (uint16) value;
	const int16 ival = (int16) value;

	switch (offset) {
	/* **************************************************** */
	case VAR_LINE_STATUS:
		LineStatusSet(uval & 0xFF);
		res = LineStatusGet();
		break;

	case VAR_RELAY_STATUS:
		RelayStatusSet((ival > 0)? RELAY_ON : RELAY_OFF);
		res = RelayStatusGet();
		break;

	case VAR_DEVICEID:
		CanMessageDeviceIdSet(uval);
		res = CanMessageDeviceIdGet();
		break;

	case VAR_UPTIME1:
		temp = TimerClockSecondsGet() & 0xFFFF;
		TimerClockSecondsSet((uval << 16) | temp);
		res = TimerClockSecondsGet() >> 16;
		break;
	case VAR_UPTIME0:
		temp = TimerClockSecondsGet() >> 16;
		TimerClockSecondsSet((temp << 16) | uval);
		res = TimerClockSecondsGet() & 0xFFFF;
		break;

	/* **************************************************** */
	case VAR_PERIPH_LEDSTATE:
		GpioLedsSet(GPIO_LED1, ival & GPIO_LED1);
		GpioLedsSet(GPIO_LED2, ival & GPIO_LED2);
		GpioLedsSet(GPIO_LED3, ival & GPIO_LED3);
		res = GpioLedsPortGet();
		break;

	case VAR_PERIPH_CANRATE1:
		temp = PeriphCanRateGet() & 0xFFFF;
		PeriphCanRateSet((uval << 16) | temp);
		res = PeriphCanRateGet() >> 16;
		break;
	case VAR_PERIPH_CANRATE0:
		temp = PeriphCanRateGet() >> 16;
		PeriphCanRateSet((temp << 16) | uval);
		res = PeriphCanRateGet() & 0xFFFF;
		break;

	case VAR_PERIPH_SPIRATE1:
		temp = PeriphSpiRateGet() & 0xFFFF;
		PeriphSpiRateSet((uval << 16) | temp);
		res = PeriphSpiRateGet() >> 16;
		break;
	case VAR_PERIPH_SPIRATE0:
		temp = PeriphSpiRateGet() >> 16;
		PeriphSpiRateSet((temp << 16) | uval);
		res = PeriphSpiRateGet() & 0xFFFF;
		break;

	case VAR_PERIPH_UARTRATE1:
		temp = PeriphUartRateGet() & 0xFFFF;
		PeriphUartRateSet((uval << 16) | temp);
		res = PeriphUartRateGet() >> 16;
		break;
	case VAR_PERIPH_UARTRATE0:
		temp = PeriphUartRateGet() >> 16;
		PeriphUartRateSet((temp << 16) | uval);
		res = PeriphUartRateGet() & 0xFFFF;
		break;

	/* **************************************************** */
	case VAR_MUTEX_PERIOD:
		PeriodSemaphoreFreqSet(uval);
		res = PeriodSemaphoreFreqGet();
		if (res != uval) err = MBUS_ERROR_VALUE;
		break;
	case VAR_PERIOD_LINECHECK:
		PeriodLineStateUpdateSet(uval);
		res = PeriodLineStateUpdateGet();
		if (res != uval) err = MBUS_ERROR_VALUE;
		break;
	case VAR_PERIOD_VOLTUPD:
		PeriodLineVoltUpdateSet(uval);
		res = PeriodLineVoltUpdateGet();
		if (res != uval) err = MBUS_ERROR_VALUE;
		break;
	case VAR_PERIOD_COMMCHECK:
		PeriodCommCheckSet(uval);
		res = PeriodCommCheckGet();
		if (res != uval) err = MBUS_ERROR_VALUE;
		break;
	case VAR_PERIOD_VOLTCHECK:
		PeriodLineVoltCheckSet(uval);
		res = PeriodLineVoltCheckGet();
		if (res != uval) err = MBUS_ERROR_VALUE;
		break;
	case VAR_PERIOD_ENDPCHECK:
		PeriodEndpointCheckSet(uval);
		res = PeriodEndpointCheckGet();
		if (res != uval) err = MBUS_ERROR_VALUE;
		break;

	/* **************************************************** */
	case VAR_VOLTAC_MAX:
		VoltageACMaxSet(kil(ival));
		res = kil(VoltageACMaxGet());
		break;
	case VAR_VOLTAC_ADCCOUNTS:
		VoltageACAdcCountsSet(uval);
		res = VoltageACAdcCountsGet();
		break;
	case VAR_VOLTDC_MAX:
		VoltageDCMaxSet(kil(ival));
		res = kil(VoltageDCMaxGet());
		break;
	case VAR_VOLTDC_ADCCOUNTS:
		VoltageDCAdcCountsSet(uval);
		res = VoltageDCAdcCountsGet();
		break;

	// TODO: stopped here
	/* **************************************************** */
	case VAR_VOLT_SPIKE: res = (int) kil(VoltageSpikeGet()); break;
	case VAR_VOLT_NORMAL: res = (int) kil(VoltageNormalGet()); break;
	case VAR_VOLT_LINEBREACH: res = (int) kil(VoltageOverflowGet()); break;
	case VAR_VOLT_LINESHORT: res = (int) kil(VoltageUnderflowGet()); break;

	case VAR_FOURIER_OVERSAMPLES: res = FourierSamplingPassesGet(); break;
	case VAR_FOURIER_SAMPLERATE: res = FourierSamplingRateGet(); break;
	case VAR_FOURIER_VOLTMULT1:
		res = ((int) bil(VoltageACMultGet())) >> 16; break;
	case VAR_FOURIER_VOLTMULT0:
		res = ((int) bil(VoltageACMultGet())) & 0xFFFF; break;
	case VAR_FOURIER_FREQMIN: res = FourierFreqRangeMinGet(); break;
	case VAR_FOURIER_FREQMAX: res = FourierFreqRangeMaxGet(); break;
	case VAR_FOURIER_FREQREF: res = FourierFreqReferenceGet(); break;


	/* **************************************************** */
	case VAR_SEARCHBUF_SIZE: res = BuzzerDomainGetLength(); break;

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
		res = BuzzerDomainGetFreqByNum(offset - VAR_SEARCHBUF_NUM01);
		break;

	default: err = MBUS_ERROR_ADDRESS; break;
	}

	// returns either a result or an error
	return (err == 0)? res : err;

}
