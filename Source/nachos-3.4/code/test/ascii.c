#include "syscall.h"

int main()
{
	//bang ma ascii
	int i = 0;
	PrintString("Bang ma ascii: \n");
	for (i; i < 128; i = i + 1){
		PrintInt(i);
		PrintChar(32);
		PrintChar(i);
		PrintChar('\n');
	}

	Exit(0);
}
