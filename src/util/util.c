
#include "util/util.h"

//#include <stdlib.h>
#include "periph/timer.h"

/* **************************************************** *
 *           SYSTEM TIMER CLOCK GET FUNCTION
 * **************************************************** */
inline uint32 _time (void) { return TimerClockSecondsGet(); }
inline uint32 _time_raw (void) { return TimerClockRawGet(); }

uint32 _time_ms (void) {
//	const uint32 sec = kil(TimerClockSecondsGet());
	const uint32 ms = kil(1) - TimerClockMillisGet();
	return /*sec +*/ ms;
}

uint32 _time_us (void) {
//	const uint32 sec = mil(TimerClockSecondsGet());
	const uint32 us = mil(1) - TimerClockMicrosGet();
	return /*sec +*/ us;
}

/* **************************************************** *
 *                 SYSTEM TIMER DELAYS
 * **************************************************** */
void _delay (int32 length) {
	const uint32 start = _time();
	uint32 current = _time();
	while ((current - start) <= length) current = _time();
}

void _delay_ms (int32 length) {
	const uint32 start = _time_ms();
	uint32 current = _time_ms();
	while ((current - start) <= length) current = _time_ms();
}

void _delay_us (int32 length) {
	const uint32 start = _time_us();
	uint32 current = _time_us();
	while ((current - start) <= length) current = _time_us();
}

/*****************************************************************/
int8_t __arr_comp8 (uint8_t* new, uint8_t size1, uint8_t* old, uint8_t size2)
{
	// compare count of non-empty elements of arrays
	// return -1 if less, 0 if equal, 1 if greater
#define	ARR_COMP_MAX_SIZE	1024
	uint8_t	i, a, b;

	a = b = 0;
	for (i = 0; i < size1; i++)
		if ( *(new + i) != 0 )	a++;
	for (i = 0; i < size2; i++)
		if ( *(old + i) != 0 )	b++;

	if (a > b)
		return	1;
	else if (a == b)
		return	0;
	else
		return	-1;
#undef	ARR_COMP_MAX_SIZE	/*1024*/
}

/*****************************************************************/
void	__sort_r16 (int16_t *array, int16_t length)
{
	#define	ARR_SORT_MAX_SIZE	4096
	// usual quick recursive sort
	// input unsorted array
	// output ready array

	int16_t	pivot, temp, i, j;

	// stupid check for zero size
	if ((length < 2) || (length > ARR_SORT_MAX_SIZE) )
		return;

	pivot = array[length / 2];

	for (i = 0, j = length - 1; ; i++, j--)
	{
		while (array[i] < pivot)
			i++;

		while (array[j] > pivot)
			j--;

		if (i >= j)
			break;

		temp = array[i];
		array[i] = array[j];
		array[j] = temp;
	}

	__sort_r16(array, i);
	__sort_r16((array + i), length - i);
}
