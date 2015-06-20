#include "syscall.h"

int main()
{
	OpenFileId id;
	int filesz, i;
	char c;
	char name[255];
	PrintString("Input file name:");
	ReadString(name, 255);
	if ( (id = Open(name, 1)) == -1){
		PrintString("Can not open file ");
		return 0;
	}


	id--;
	filesz = Seek(-1, id);
	Seek(0, id);
	for(i = 0; i < filesz; i++ ){
		Read(&c, 1, id);
		PrintChar(c);	
	}
	Close(id);
	Exit(0);
}
