#include "String.h"

void String::strcpy(char* des, char* source)
{
	int i = 0;
	do {
		des[i] = source[i];
	} while (source[i++] != 0);
}
