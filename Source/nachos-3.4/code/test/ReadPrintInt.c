#include "syscall.h"

int
main()
{
    PrintInt(ReadInt());
	Halt();
    /* not reached */
}
