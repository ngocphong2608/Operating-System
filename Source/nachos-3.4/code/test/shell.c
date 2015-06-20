#include "syscall.h"
#include "util.h"
#define MAX_STRING 100

int
main()
{
	SpaceId newProc;
	char buffer[MAX_STRING];
	char temp[MAX_STRING];
	int i;
	int j;
	
	// "./test/"
	temp[0] = '.';
	temp[1] = '/';
	temp[2] = 't';
	temp[3] = 'e';
	temp[4] = 's';
	temp[5] = 't';
	temp[6] = '/';

	do {
		PrintString("\n[--]@");
		ReadString(buffer, MAX_STRING);
		
		if (buffer[0] == 'e' && buffer[1] == 'x' && buffer[2] == 'i' && buffer[3] == 't')
			break;
		
		if (buffer[0] == '&') {		// che do background
			XoaKyTuDau(buffer);
			GhepChuoi(temp, buffer);
			//PrintString(temp);
			newProc = Exec(temp);
			
			if(newProc < 0)
				PrintString("\nCan't not exec.\n");


		} else {					// che do chay thong thuong
			GhepChuoi(temp,buffer);

			newProc = Exec(temp);
			
			if(newProc < 0)
				PrintString("\nCan't not exec.\n");
			else {
				j = Join(newProc);
				if (j != 0) {
					PrintString("Join error, excode: ");
					PrintInt(j);
					PrintChar('\n');
				}
			}
		}
		
		// set lai tu dau
		memset(buffer,0,MAX_STRING);
		memset(temp,0,MAX_STRING);
		temp[0] = '.';
		temp[1] = '/';
		temp[2] = 't';
		temp[3] = 'e';
		temp[4] = 's';
		temp[5] = 't';
		temp[6] = '/';
	} while (1);
	Exit(0);
}
