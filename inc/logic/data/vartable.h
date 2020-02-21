/*
 * Modicon Bus like variables table.
 * Every used ID is represented in included enumerator.
 */

#ifndef __DATA_VARTABLE_H
#define __DATA_VARTABLE_H

#include "util/typedefs.h"

/* **************************************************** *
 *                HEADER SPECIFIC MACROS
 * **************************************************** */
enum mbusROVarTableEnum {
	VAR_READONLY_OFFSET  = 30000,
	VAR_LINE_VOLTDC1,
	VAR_LINE_VOLTDC0,
	VAR_LINE_VOLTAC1,
	VAR_LINE_VOLTAC0,

	VAR_MUTEX_REG        = 30011,
	VAR_STREAM_REG,

	VAR_FOURIER_BUFSIZE  = 30408,
	VAR_FOURIER_FREQRES,

	VAR_FOUNDBUF_SIZE    = 31000,
	VAR_FOUNDBUF_NUM01,  VAR_FOUNDBUF_NUM02,  VAR_FOUNDBUF_NUM03,
	VAR_FOUNDBUF_NUM04,  VAR_FOUNDBUF_NUM05,  VAR_FOUNDBUF_NUM06,
	VAR_FOUNDBUF_NUM07,  VAR_FOUNDBUF_NUM08,  VAR_FOUNDBUF_NUM09,
	VAR_FOUNDBUF_NUM10,  VAR_FOUNDBUF_NUM11,  VAR_FOUNDBUF_NUM12,
	VAR_FOUNDBUF_NUM13,  VAR_FOUNDBUF_NUM14,  VAR_FOUNDBUF_NUM15,
	VAR_FOUNDBUF_NUM16,  VAR_FOUNDBUF_NUM17,  VAR_FOUNDBUF_NUM18,
	VAR_FOUNDBUF_NUM19,  VAR_FOUNDBUF_NUM20,  VAR_FOUNDBUF_NUM21,
	VAR_FOUNDBUF_NUM22,  VAR_FOUNDBUF_NUM23,  VAR_FOUNDBUF_NUM24,
	VAR_FOUNDBUF_NUM25,  VAR_FOUNDBUF_NUM26,  VAR_FOUNDBUF_NUM27,
	VAR_FOUNDBUF_NUM28,  VAR_FOUNDBUF_NUM29,  VAR_FOUNDBUF_NUM33,
	VAR_FOUNDBUF_NUM31,  VAR_FOUNDBUF_NUM32,
};

/* **************************************************** */
enum mbusRWVarTableEnum {
	VAR_READWRITE_OFFSET = 40000,
	VAR_LINE_STATUS,
	VAR_RELAY_STATUS,
	VAR_DEVICEID         = 40006,

	VAR_UPTIME1          = 40008,
	VAR_UPTIME0,

	VAR_PERIPH_LEDSTATE  = 40100,
	VAR_PERIPH_CANRATE1,
	VAR_PERIPH_CANRATE0,
	VAR_PERIPH_SPIRATE1,
	VAR_PERIPH_SPIRATE0,
	VAR_PERIPH_UARTRATE1,
	VAR_PERIPH_UARTRATE0,

	VAR_MUTEX_PERIOD     = 40200,
	VAR_PERIOD_LINECHECK,
	VAR_PERIOD_VOLTUPD,
	VAR_PERIOD_COMMCHECK,

	VAR_VOLTAC_MULT1     = 40300,
	VAR_VOLTAC_MULT0,
	VAR_VOLTDC_MULT1,
	VAR_VOLTDC_MULT0,
	VAR_VOLT_SPIKE,
	VAR_VOLT_NORMAL,
	VAR_VOLT_LINEBREACH,
	VAR_VOLT_LINESHORT,

	VAR_FOURIER_OVERSAMPLES = 40400,
	VAR_FOURIER_SAMPLERATE,
	VAR_FOURIER_VOLTMULT1,
	VAR_FOURIER_VOLTMULT0,
	VAR_FOURIER_FREQMIN,
	VAR_FOURIER_FREQMAX,
	VAR_FOURIER_FREQREF,

	VAR_SEARCHBUF_SIZE   = 41000,
	VAR_SEARCHBUF_NUM01,  VAR_SEARCHBUF_NUM02,  VAR_SEARCHBUF_NUM03,
	VAR_SEARCHBUF_NUM04,  VAR_SEARCHBUF_NUM05,  VAR_SEARCHBUF_NUM06,
	VAR_SEARCHBUF_NUM07,  VAR_SEARCHBUF_NUM08,  VAR_SEARCHBUF_NUM09,
	VAR_SEARCHBUF_NUM10,  VAR_SEARCHBUF_NUM11,  VAR_SEARCHBUF_NUM12,
	VAR_SEARCHBUF_NUM13,  VAR_SEARCHBUF_NUM14,  VAR_SEARCHBUF_NUM15,
	VAR_SEARCHBUF_NUM16,  VAR_SEARCHBUF_NUM17,  VAR_SEARCHBUF_NUM18,
	VAR_SEARCHBUF_NUM19,  VAR_SEARCHBUF_NUM20,  VAR_SEARCHBUF_NUM21,
	VAR_SEARCHBUF_NUM22,  VAR_SEARCHBUF_NUM23,  VAR_SEARCHBUF_NUM24,
	VAR_SEARCHBUF_NUM25,  VAR_SEARCHBUF_NUM26,  VAR_SEARCHBUF_NUM27,
	VAR_SEARCHBUF_NUM28,  VAR_SEARCHBUF_NUM29,  VAR_SEARCHBUF_NUM33,
	VAR_SEARCHBUF_NUM31,  VAR_SEARCHBUF_NUM32,
};

/* **************************************************** *
 *                 FUNCTION PROTOTYPES
 * **************************************************** */
int32 ConfigVarTableGet (const int32 address);
int32 ConfigVarTableSet (const int32 address, const int16 value);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__DATA_VARTABLE_H
