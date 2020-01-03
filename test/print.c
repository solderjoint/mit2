#include <stdio.h>
#include <string.h>

#define to_str(XX)  (#XX)

void perform (const char buf[80]) {
	const int size = strnlen (buf, 80);
	for (int i = 0; i < size; i++) printf ("%c", buf[i]);
	printf ("\nrest:\n");
	for (int i = size; i < 80; i++) printf ("%i", buf[i]);
}

int main (void) {
	const int blackVal = 10;
	const char buf[] = "blackVal";
	const char str[] = to_str(blackVaL);

	printf ("buf>\t%s\nstr>\t%s\n", buf, str);
	printf ("s1>\t%i\ts2>\t%i\n", sizeof(buf), strlen(buf));
	perform (to_str(blackVal));

	return 0; 
}
