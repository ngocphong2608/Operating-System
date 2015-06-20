#include "syscall.h"

int main()
{
	char buffer[100];
	PrintString("Echo:\n");
	PrintString("Nhap vao mot chuoi: \n");
	ReadString(buffer, 99);
	PrintString(buffer);
	Exit(0);
}
