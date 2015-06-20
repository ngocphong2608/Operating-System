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
#define MAX_INT 255

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
void PrintString(char* buffer);


void
ExceptionHandler(ExceptionType which)
{
	// doc len ma lenh can thuc hien
	int type = machine->ReadRegister(2);

	switch (which)
	{
		case NoException:
			return;

		case SyscallException:
		{
			switch (type)
			{
				// tat may ao
				case SC_Halt:
				{
					DEBUG('a', "\n Shutdown, initiated by user program.");

					PrintString("\n\n Shutdown, initiated by user program.");

					interrupt->Halt();	

					break;
				}
			  
				// doc mot so nguyen
				case SC_ReadInt:
				{
					char* buffer = new char[MAX_INT];

					// doc mot chuoi thong qua lop gSynchConsole
					int numBytesRead = gSynchConsole->Read(buffer, MAX_INT);
					if (numBytesRead > 0)
					{
						// chuyen chuoi ki tu sang so
						int result = Char2Int(buffer, numBytesRead);
						
						// tra ve ket qua so da doc
						machine->WriteRegister(2, result);
					} else { // neu doc chuoi bi loi thi tra ve 0
						PrintString("\nReadInt: Error while reading string\n");
						machine->WriteRegister(2, 0);
					}
					
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
					int numBytes = gSynchConsole->Write(pCh, len);
					
					// khong the tra ve loi hoac so ki tu doc vi thiet ke ham khong co tham so tra ve
					//machine->WriteRegister(2, numBytes);
					if (numBytes < 0) {
						PrintString("\nPrintInt: Error printing string\n");
					}
					
					delete pCh;

					break;
				}
				  
				// doc vao mot ki tu
				case SC_ReadChar:
				{
					char* pCh = new char[2];

					// doc ki 1 tu, thuc chat la doc 1 chuoi co do dai 1
					int numBytes = gSynchConsole->Read(pCh, 1);
					if (numBytes > 0) // neu doc thanh cong
						machine->WriteRegister(2, pCh[0]);
					else {		// neu doc that bai
						PrintString("\nReadChar: Error while reading char\n");
						machine->WriteRegister(2, numBytes);
					}

					delete pCh;
					break;
				}

				// ghi mot ki tu ra man hinh console
				case SC_PrintChar:
				{
					char ch = machine->ReadRegister(4);
				  
					char* pCh = new char[2];
					pCh[0] = ch;
					pCh[1] = '\0';

					int numBytes = gSynchConsole->Write(pCh, 1);
					
					// khong the tra ve loi hoac so ki tu doc vi thiet ke ham khong co tham so tra ve
					// machine->WriteRegister(2, numBytes);
					
					if (numBytes < 0)
						PrintString("\nPrintChar: Error while printing char\n");
					
					delete pCh;
					
					break;
				}

				// doc mot chuoi ki tu
				case SC_ReadString:
				{
					// virAddr: dia chi bat dau vung nho chua chuoi, maxLen: gioi han so luong ki tu doc
					int virAddr = machine->ReadRegister(4);
					int maxLen = machine->ReadRegister(5);

					// khai bao buffer de doc vao chuoi voi do dai toi da maxLen
					char* buffer = new char[maxLen + 1];
					int len = gSynchConsole->Read(buffer, maxLen);
					
					if (len > 0)
					{
						buffer[len] = NULL;

						// chep noi dung trong vung nho system vao user, vung nho system chua chuoi da doc
						machine->System2User(virAddr, len, buffer);
						machine->WriteRegister(2, virAddr);
					} else { // truong hop doc chuoi gap loi
						PrintString("\nReadString: Error while reading string\n");
						machine->WriteRegister(2, virAddr);
					}
					
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

					// khong the tra ve loi hoac so ki tu doc vi thiet ke ham khong co tham so tra ve
					//machine->WriteRegister(2, len);
					if (len < 0)
						PrintString("\nPrintString: Error while printing string\n");
					
					delete buffer;
					break;
				}


				case SC_CreateFile:
				{
				    //virAddr: vung nho chua ten file
				    int virAddr = machine->ReadRegister(4);
				    
				    int limit = strlen(virAddr) + 1;
				   
				    //chep noi dung trong vung nho sang vung system
				    char* buffer = machine->User2System(virAddr, limit);

				    if(!fileSystem->Create(buffer, 0))
				    {
					DEBUG('f', "Can not create file!");
					machine->WriteRegister(2, -1);
				    }
				    else
				    {
					DEBUG('f',"Create file successfully!");
					machine->WriteRegister(2, 0);
				    }
		
				    delete [] buffer;
				    break;
				}
		
				case SC_Open:
				{
				    // doc vung nho chua file
				    int virAddr = machine->ReadRegister(4);
				    int type = machine->ReadRegister(5);
				    int limit = strlen(virAddr) + 1;
				    	
				    if(fileSystem->index > 10)	//bang mo ta file
		 		    {
					machine->WriteRegister(2, -1);
					break;
				    }
				    //chep noi dung vung nho sang vung system
				    char* buffer = machine->User2System(virAddr, limit);

				    if(strcmp(buffer, "input") == 0)
				    {
					printf("Open\n");
					machine->WriteRegister(2, 0);
					break;
				    }
				    
				    if(strcmp(buffer, "output") == 0)
				    {
					printf("Open\n");
					machine->WriteRegister(2, 1);
					break;
				    }

				   // mo file thanh cong
				    if((fileSystem->fileTable[fileSystem->index] = fileSystem->Open(buffer, type)) != NULL)
				    {
					DEBUG('f', "open file successfully!");
					machine->WriteRegister(2, fileSystem->index);
					break;
				    }
				    else
				    {
					DEBUG('f', "cannot open this file!");
					machine->WriteRegister(2, -1);
					break;
				    }
				    delete []buffer;
				}

				case SC_Close:
				{
				    // doc id duoc truyen vao
				    int id = machine->ReadRegister(4);
				    
				    // kiem tra xem file co duoc mo hay khong
			 	    if(fileSystem->fileTable[id] == NULL )
					break;
		
				    // neu file mo thi xoa khoi bang ghi file
				    delete fileSystem->fileTable[id];
				    fileSystem->fileTable[id] = NULL;
				    
				}
		
				case SC_Read:
				{
				    int virAddr = machine->ReadRegister(4);
				    int lenBuf = machine->ReadRegister(5);
				    int id = machine->ReadRegister(6);

				    int oldPos, newPos, lenFile;
				    
				    // kiem tra id co hop le
				    if(id < 0 || id > 10)
				    {
					machine->WriteRegister(2, -1);
					break;
				    }

				    // kiem tra file co ton tai
				    if(fileSystem->fileTable[id] == NULL)
				    {
					machine->WriteRegister(2, -1);
					break;
				    }

				    char* buffer = new char[lenBuf];
		
				    oldPos = fileSystem->fileTable[id]->getCurPos();
			 	    buffer = machine->User2System(virAddr, lenBuf);
				    lenFile = fileSystem->fileTable[id]->Length();	
				   

				    // Bat dau doc file
				    if(fileSystem->fileTable[id]->Read(buffer, lenBuf) > 0)
				    {
					newPos = fileSystem->fileTable[id]->getCurPos();
					int size = newPos - oldPos + 1;
					machine->System2User(virAddr, size, buffer);
					// kiem tra co dung toi cuoi file
					if(newPos == lenFile - 1){
					    machine->WriteRegister(2, -2);
					    delete [] buffer;
					    break;
					}

					//nguoc lai doc va tra ve so byte doc
					machine->WriteRegister(2, size);
					delete [] buffer;
					break;
				    }
				    else
				    {
					machine->WriteRegister(2, -1);
				    }
				    
				    delete [] buffer;
				    break;

				}


				case SC_Write:
				{
		    		    int virAddr = machine->ReadRegister(4);
				    int lenBuf = machine->ReadRegister(5);					
				    int id = machine->ReadRegister(6);

				    char* buffer = new char[lenBuf];	
				    int oldPos, newPos;

				    //kiem tra id co hop le
				    if(id < 0 || id > 10)
				    {
					machine->WriteRegister(2, -1);
					delete [] buffer;
					break;
				    }		
	
				    // kiem tra file co ton tai hoac la file chi doc
				    if(fileSystem->fileTable[id] == NULL || fileSystem->fileTable[id]->type == 1){
					machine->WriteRegister(2, -1);
					delete [] buffer;
					break;
				    }	    
				    else{
					buffer = machine->User2System(virAddr, lenBuf);
				    	int lenWrite = fileSystem->fileTable[id]->Write(buffer, lenBuf);// file doc va ghi
					machine->WriteRegister(2, lenWrite);	//tra ve so byte da ghi
					}
				    delete [] buffer;
				    break;
				}
		
				case SC_Seek:
				{
				    int pos = machine->ReadRegister(4);
				    int id = machine->ReadRegister(5);

				    if (id < 0 || id > 10)
				    {
					machine->WriteRegister(2, -1);
					break;	
				    }		
				    if (fileSystem->fileTable[id] == NULL)
				    {
					printf("seek fail \n");
					machine->WriteRegister(2, -1);
					break;
			 	    }

				    if (pos == -1)
				    {
					pos = fileSystem->fileTable[id]->Length();
					machine->WriteRegister(2, pos);
					break;
				    }	
				    if(pos <= -1 || pos > fileSystem->fileTable[id]->Length())
				    {
					machine->WriteRegister(2, -1);
					break;
				    }
				    else
				    {
					fileSystem->fileTable[id]->Seek(pos);
				  	machine->WriteRegister(2, pos);
				    }
				    break;
				}

				case SC_Exec:
				{
					// Executable file (user space)
					int execFile = machine->ReadRegister(4);
					
					int len = strlen(execFile) + 1;
					
					// change to kernel space
					char* execFile_kernel = machine->User2System(execFile, len);
					
					int id = pTab->ExecUpdate(execFile_kernel);
					
					machine->WriteRegister(2, id);
					
					delete execFile_kernel;
					break;				
				}
				
				case SC_Join:
				{
					// read join process id
					int id = machine->ReadRegister(4);
					
					int ec = pTab->JoinUpdate(id);
					
					machine->WriteRegister(2, ec);
					
					break;
				}
				
				// ket thuc chuong trinh user
				case SC_Exit:
				{
					// read exitStatus
					int exitStatus = machine->ReadRegister(4);
					int ec = pTab->ExitUpdate(exitStatus) ;
					
					currentThread->FreeSpace();
					currentThread->Finish();
					
					machine->WriteRegister(2, ec);

					for (int i = 0; i < 10; i++) if (pTab->IsExist(i)) printf("\n%s\n", pTab->GetFileName(i));
					
					break;
				}
				
				case SC_CreateSemaphore:
				{
					// read semaphore name (kernel space)
					int name = machine->ReadRegister(4);
					
					int len = strlen(name) + 1;
					
					// kernel space to user space
					char* name_kernel = machine->User2System(name, len);
					
					// read semval from argument
					int semval = machine->ReadRegister(5);
					
					int t = semTab->Create(name_kernel, semval) ;
					
					machine->WriteRegister(2, t);
					
					delete name_kernel;
					break;
				}
				
				case SC_Up:
				{
					// read  name (kernel space)
					int name = machine->ReadRegister(4);
					
					int len = strlen(name) + 1;
					
					// kernel space to user space
					char* name_kernel = machine->User2System(name, len);
					
					int pos = semTab->getSemPos(name_kernel);
					
					if (pos == -1) {
						printf("Semaphore %s not exist\n", name_kernel);
						machine->WriteRegister(2, -1);
						delete name_kernel;
						break;
					} else {
						int t =semTab->Signal(name_kernel);
						machine->WriteRegister(2, t);
						delete name_kernel;
						break;
					}
				}
				
				case SC_Down:
				{
					// read  name (kernel space)
					int name = machine->ReadRegister(4);
					
					int len = strlen(name) + 1;
					
					// kernel space to user space
					char* name_kernel = machine->User2System(name, len);
					
					int pos = semTab->getSemPos(name_kernel);
					
					if (pos == -1) {
						printf("Semaphore %s not exist\n", name_kernel);
						machine->WriteRegister(2, -1);
						delete name_kernel;
						break;
					} else {
						int t = semTab->Wait(name_kernel);
						machine->WriteRegister(2, t);
						delete name_kernel;
						break;
					}
				}

				default:
					printf("\nUnexpected user mode exception (%d %d)", which, type);
					interrupt->Halt();
					break;
			}
			// tang gia tri cua con tro lenh
			machine->registers[PrevPCReg] = machine->registers[PCReg];
			machine->registers[PCReg] = machine->registers[NextPCReg];
			machine->registers[NextPCReg] += 4;
			break;
		}
		
		case PageFaultException:
			PrintString ("\nNo valid translation found.\n");
			interrupt->Halt();
			break;
		
		case ReadOnlyException:
			PrintString ("\nWrite attempted to page marked \"read-only\".\n");
			interrupt->Halt();
			break;
			
		case BusErrorException:
			PrintString ("\nTranslation resulted in an invalid physical address.\n");
			interrupt->Halt();
			break;
			
		case AddressErrorException:
			PrintString ("\nUnaligned reference or one that was beyond the end of the address space.\n");
			interrupt->Halt();
			break;
			
		case OverflowException:
			PrintString ("\nInteger overflow in add or sub.\n");
			interrupt->Halt();
			break;
			
		case IllegalInstrException:
			PrintString ("\nUnimplemented or reserved instruction.\n");
			interrupt->Halt();
			break;
			
		default:
			printf ("\nUnexpected exception %d\n", which);
			interrupt->Halt();
			break;
	}
}


// chuyen doi tu kieu so nguyen sang chuoi
char* Int2Char(int num, int &len) {
	if (num == 0) {
		len = 2;
		char* pCh = new char[len];
		pCh[0] = '0';
		pCh[1] = NULL;
		return pCh;
	}

	char* pCh = new char[11];
	len = 0;
	
	// ghi them dau tru vao chuoi ket qua neu la so am
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

	if (buffer == NULL)
		return 0;
	
	int result = 0;
	if (buffer[0] == '-') { // truong hop so am
		int expo = 1;
		for (int i = len - 1; i > 0; i--) {
			if (buffer[i] < '0' || buffer[i] > '9')
				return 0;
			result += (buffer[i] - 48) * expo;
			expo *= 10;
		}
		result *= -1;
	} else {		// truong hop so duong
		int expo = 1;
		for (int i = len - 1; i >= 0; i--) {
			if (buffer[i] < '0' || buffer[i] > '9')
				return 0;
			result += (buffer[i] - 48) * expo;
			expo *= 10;
		}		  
	}
	return result;
}

// tinh do dai chuoi
int strlen(int virAddr) {
	if (virAddr == 0)
			return 0;
	
	int len = 0;
	int ch;
	do {
		machine->ReadMem(virAddr+len, 1, &ch);
		len++;
	} while (ch != NULL);
	return len;
}

// ham in chuoi ra man hinh
void PrintString(char* buffer) {
	gSynchConsole->Write(buffer, strlen(buffer));
	gSynchConsole->Write("\n", 1);
}
