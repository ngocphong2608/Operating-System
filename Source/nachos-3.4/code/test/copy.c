#include "syscall.h"


int main()
{
	OpenFileId srcID, desID;
	int filesz, i, t;
	char c;
	char srcName[255];
	char desName[255];
	PrintString("Input file name of source file:");
	ReadString(srcName, 255);
	
	PrintString("Input file name of destination file:");
	ReadString(desName, 255);
	

	//tao file ghi
	CreateFile(desName);

	//mo file
	srcID = Open(srcName, 1); //mo file source chi doc
	desID = Open(desName, 0); //mo file destination doc ghi
	if ( srcID == -1 || desID == -1){// neu mo 1 trong 2 file nay bi loi
		PrintString("Can not open source file ");
		Halt();
		return 0;
	}
	
	srcID--;
	desID--; 
	
	
	Seek(0, srcID);

	do{
		t = Read(&c, 1, srcID);
		Write(&c, 1, desID);
		PrintChar(c);
	  }while(t != -2);
	PrintString("\nCopy done!\n");
	Close(srcID);
	Close(desID);

	
	Exit(0);
}
