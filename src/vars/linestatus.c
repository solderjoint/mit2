
#include "vars/linestatus.h"

#include "util/securevar.h"

/* **************************************************** *
 *             SECURE LINE STATUS VARIABLE
 * **************************************************** */
svar_init(static int32, lineStatus, STATUS_OK);

int32 LineStatusSet (enum lineStatusEnum val) {
	svar_set(lineStatus, val);
	return val;
}

int32 LineStatusGet (void) {
	int32 val;
	svar_get(lineStatus, val);
	return val;
}
