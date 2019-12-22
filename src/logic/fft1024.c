
#include "logic/fft1024.h"

#include "vars/fourierconsts.h"

static cf32_t temp_buf[1024];

/* **************************************************** *
 *                FFT UTILITY FUNCTIONS
 * **************************************************** */
static int _ispowerof2 (const unsigned int n) {
	if (n == 0) return 0;

	for (int i = n; i != 1; i >>= 1)
		if (i % 2 != 0) return 0;
	return 1;
}

static void cbuf_copy (cf32_t input[], cf32_t out[], const int n) {
	for (int i = 0; i < n; i++)
		out[i] = input[i];
}

/* **************************************************** *
 *               ZERO FILL BORDER POINTS
 * **************************************************** */
static void FourierAppendZeroes
(cf32_t input[], const int size, const int wanted) {
	if (size >= wanted) return;
	const int off = (wanted - size) / 2;
	for (int i = size + off; i > off; i--) {
	input[i] = input[i - off]; // right shift values
	}
	// then fill borders with zeroes
	for (int i = 0; i < off; i++) input[i] = 0;
	for (int i = off + size; i < wanted; i++) input[i] = 0;
}

/* **************************************************** *
 *             MAIN FAST FOURIER TRANSFORM
 * **************************************************** */
static void FourierTransform
(cf32_t input[], cf32_t temp[], const int n, const int step) {
	const float FFT_PI = 3.1415926535897932384626f;
	if (step < n) {
//		UsartConsolePutchar('.');
		FourierTransform(temp, input, n, step * 2);
		FourierTransform(temp + step, input + step, n, step * 2);
		for (int i = 0; i < n; i += 2 * step) {
			const cf32_t val = -I * FFT_PI * i / n;
			const cf32_t t = cexpf(val) * temp[i + step];

			input[i / 2]     = temp[i] + t;
			input[(i + n)/2] = temp[i] - t;
		}
	}
}

/* **************************************************** *
 *                   MAIN INPUT POINT
 * **************************************************** */
int FourierProcess (cf32_t input[]) {

	const int32 size = FourierBufferLengthGet();
	if (_ispowerof2(size) == 0) FourierAppendZeroes(input, size, FOURIER_POINTS);
	// copy values from input to power buffer
	cbuf_copy(input, temp_buf, size);

	const int32 step = 1;
	FourierTransform(input, temp_buf, size, step);
	return 0;
}
