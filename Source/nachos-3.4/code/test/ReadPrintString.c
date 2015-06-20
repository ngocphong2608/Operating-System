
#include "syscall.h"

int
main()
{
    char buffer[100];
    ReadString(buffer, 99);
    PrintString(buffer);
}
