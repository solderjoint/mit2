#include <stdio.h>

int main (void) {
//	const int i = 312;
//	const float f = *(float *)&i;
//	printf ("i>\t%i\tf>\t%f\n", i, f);

	const float f = 312.f;
	const int i = * (int *) &f;
	printf ("i>\t%i\tf>\t%f\n", i, f);
	return 0;
}
