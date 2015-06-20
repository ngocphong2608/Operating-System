#include "syscall.h"

int main()
{
	//Create File
	char buffer[100];
	ReadString(buffer, 100);
	CreateFile(buffer);
	Exit(0);
}
