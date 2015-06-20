// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

/* cac ham ho tro */
char* Int2Char(int num, int &len);
int Char2Int(char* buffer, int len);
int strlen(int virAddr);


void
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);

    switch (which) {

      case NoException:
 	return;

      case SyscallException:
 	switch (type){

	  // tat may ao
	  case SC_Halt:
	  {
	    DEBUG('a', "\n Shutdown, initiated by user program.");

	    printf ("\n\n Shutdown, initiated by user program.");

	    interrupt->Halt();

	    break;
	  }
  
	  // doc mot so nguyen
	  case SC_ReadInt:
	  {
	    int len = 11;
	    char* buffer = new char[len];

	    // doc mot chuoi thong qua lop gSynchConsole
	    int numBytesRead = gSynchConsole->Read(buffer, len);
	    
	    // chuyen chuoi ki tu sang so
	    int result = Char2Int(buffer, numBytesRead);
	    
	    // tra ve ket qua so da doc
	    machine->WriteRegister(2, result);

	    delete buffer;

	    break;
	  }

	  // ghi mot so nguyen len man hinh console
	  case SC_PrintInt:
	  {
	    // doc len so nguyen tu thanh ghi so 4
	    int number = machine->ReadRegister(4);

	    // chuyen doi so nguyen sang chuoi de ghi ra console, vi ham write chi ho tro ghi chuoi
	    int len = 0;
	    char* pCh = Int2Char(number, len);

	    // dung lop gSynchConsole de ghi mot chuoi ra console
	    gSynchConsole->Write(pCh, len);

	    delete pCh;

	    break;
	  }
	  
	  // doc vao mot ki tu
	  case SC_ReadChar:
	  {
	    char* pCh = new char[2];

	    // doc ki 1 tu, thuc chat la doc 1 chuoi co do dai 1
	    int numBytes = gSynchConsole->Read(pCh, 1);
	    
	    machine->WriteRegister(2, pCh[0]);

	    delete pCh;

	  }

	  // ghi mot ki tu ra man hinh console
	  case SC_PrintChar:
	  {
	    char ch = machine->ReadRegister(4);
	  
	    char* pCh = new char[2];
	    pCh[0] = ch;
	    pCh[1] = '\0';

	    gSynchConsole->Write(pCh, 1);

	    delete pCh;
	    
	    break;
		
	  }

	  // doc mot chuoi ki tu
	  case SC_ReadString:
	  {
	    // virAddr: dia chi bat dau vung nho chua chuoi, limit: gioi han so luong ki tu doc
	    int virAddr = machine->ReadRegister(4);
	    int maxLen = machine->ReadRegister(5);

	    // khai bao buffer de doc vao chuoi voi do dai toi da maxLen
	    char* buffer = new char[maxLen + 1];
	    int len = gSynchConsole->Read(buffer, maxLen);
	    buffer[len] = NULL;

	    // chep noi dung trong vung nho system tro ve user, vung nho system chua chuoi da doc
	    machine->System2User(virAddr, len, buffer);
	    machine->WriteRegister(2, virAddr);

	    delete buffer;
	    break;
	  }

	  // ghi mot chuoi ki tu
	  case SC_PrintString:
	  {
	    // virAddr: vung nho chua chuoi (user)
	    int virAddr = machine->ReadRegister(4);

	    int limit = strlen(virAddr) + 1;

	    // chep noi dung cua tron vung nho user sang 1 vung nho system (buffer)
	    char* buffer = machine->User2System(virAddr, limit);

	    //thuc hien ghi chuoi
	    int len = gSynchConsole->Write(buffer, limit);

	    machine->WriteRegister(2, len);

	    delete buffer;
	    break;
	  }

	  // ket thuc chuong trinh user
	  case SC_Exit:
	    printf("\n Exit by user.\n");
	    interrupt->Halt();
	    break;

	  default:
	    printf("\n Unexpected user mode exception (%d %d)", which, type);
	    interrupt->Halt();
	}
	// tang gia tri cua con tro lenh
	machine->registers[PrevPCReg] = machine->registers[PCReg];
        machine->registers[PCReg] = machine->registers[NextPCReg];
        machine->registers[NextPCReg] += 4;
    }
}


// chuyen doi tu kieu so nguyen sang chuoi
char* Int2Char(int num, int &len) {
	if (num == 0) {
		char* pCh = new char[2];
		pCh[0] = '0';
		pCh[1] = NULL;
		return pCh;	
	}

	char* pCh = new char[11];
	len = 0;

	if (num < 0) {
		pCh[len++] = '-';
		num *= -1;
	}
	
	// dem do dai cua num
	int temp = num;
	while (temp > 0) {
		len += 1;
		temp /= 10;
	}
	
	temp = len;
	pCh[len] = NULL;
	// ghi num vao chuoi
	while (num > 0) {
		pCh[--temp] = num % 10 + 48;
		num /= 10;
	}
	
	return pCh;
}

// chuyen tu chuoi sang so nguyen
int Char2Int(char* buffer, int len) {
	int result = 0;
	if (buffer[0] == '-') {
		int expo = 1;
		for (int i = len - 1; i > 0; i--) {
			result += (buffer[i] - 48) * expo;
			expo *= 10;
		}
		result *= -1;
	} else {
		int expo = 1;
		for (int i = len - 1; i >= 0; i--) {
			result += (buffer[i] - 48) * expo;
			expo *= 10;
		}	      
	}
	return result;
}

// tinh do dai chuoi
int strlen(int virAddr) {
	int len = 0;
	int ch;
	do {
		machine->ReadMem(virAddr+len, 1, &ch);
		len++;
	} while (ch != 0);
	return len;	
}
