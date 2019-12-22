
#include "system/syscalls.h"

#include <errno.h>

#include <tivaware/inc/hw_nvic.h>
#include <tivaware/inc/hw_types.h>

/* **************************************************** *
 *               HEAP END REACHING CHECK
 * **************************************************** */
void * _sbrk (ptrdiff_t nbytes) {
	errno = ENOMEM;
	return (void *) -1;
}

//char *heap_end = 0;
//caddr_t _sbrk(int incr) {
//	extern char heap_low; /* Defined by the linker */
//	extern char heap_top; /* Defined by the linker */
//	char *prev_heap_end;

//	if (heap_end == 0) heap_end = &heap_low;
//	prev_heap_end = heap_end;

//	if (heap_end + incr > &heap_top) return (caddr_t)0;
//	heap_end += incr;

//	return (caddr_t) prev_heap_end;
//}

/* **************************************************** *
 *         BOOTLOADER JUMP CALLING INT HANDLER
 * **************************************************** */
void JumpToBootLoader (void) {
	// disable all processor interrupts
	HWREG(NVIC_DIS0) = 0xFFFFFFFF;
	HWREG(NVIC_DIS1) = 0xFFFFFFFF;
	// call the SVC handler to return to the boot loader
	(* ((void (*)(void)) (* (uint32_t *) 0x2c)))();
}
