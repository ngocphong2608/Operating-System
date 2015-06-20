#include "pcb.h"
#include "system.h"
#include "addrspace.h"



PCB::PCB() {
	joinsem = new Semaphore("join semaphore", 0);
	exitsem = new Semaphore("exit semaphore", 0);
	mutex  = new Semaphore("mutex", 1);
	thread = NULL;
	pid = -1;
	IsBG = true;		
	parentID = -1;
	numwait = 0;
	exitcode = 0;
	pName = NULL;
}

PCB::PCB(int id) {
	joinsem = new Semaphore("join semaphore", 0);
	exitsem = new Semaphore("exit semaphore", 0);
	mutex  = new Semaphore("mutex", 1);
	thread = NULL;
	pid = id;
	IsBG = true;		
	parentID = currentThread->processID;
	numwait = 0;
	exitcode = 0;
	pName = NULL;
}

PCB::~PCB(){
	delete joinsem;
	delete exitsem;
	delete mutex;
	delete pName;
	thread->FreeSpace();
	thread->Finish();
}

int PCB::Exec(char *filename, int pid) {
	// Gọi mutex->P(); để giúp tránh tình trạng nạp 2 tiến trình cùng 1 lúc.
	mutex->P();
	
	// Kiểm tra thread đã khởi tạo thành công chưa
	thread = new Thread(filename);
	if (thread == NULL) {
		// push notification not enough memory
		printf("\nPCB::Exec: Thread did not create. Not enough memory space\n");
		mutex->V();
		return -1;
	}
	
	// Đặt processID của thread này là id.
	this->pid = pid;
	thread->processID = pid;
	
	// Đặt parentID của thread này là processID của thread gọi thực thi Exec
	if (pid == 0) {
		parentID = -1; // truong hop thread dau tien
	} else {
		parentID = currentThread->processID;
	}
	
	//Gọi thực thi Fork(StartProcess_2,id) => Ta cast thread thành kiểu int, sau đó khi xử ký hàm
	//StartProcess ta cast Thread về đúng kiểu của nó.
	thread->Fork(StartProcess_2, pid);
	
	mutex->V();
	
	return pid;
}

int PCB::GetID() {
	return pid;
}

int PCB::GetNumWait() {
	return numwait;
}

void PCB::JoinWait() {
	joinsem->P();
}

void  PCB::ExitWait() {
	exitsem->P();
}

void PCB::JoinRelease() {
	joinsem->V();
}

void  PCB::ExitRelease() {
	exitsem->V();
}

void  PCB::IncNumWait() {
	mutex->P();
	++numwait;
	mutex->V();
}

void  PCB::DecNumWait() {
	mutex->P();
	if (numwait > 0)
		--numwait;
	mutex->V();
}

void PCB::SetExitCode(int ec) {
	exitcode = ec;
}

int PCB::GetExitCode() {
	return exitcode;
}

void PCB::SetFileName(char* fn) {
	int len = 0;
	while (fn[len] != NULL)
		len++;
	pName = new char[len + 1];
	
	do {
		pName[len] = fn[len];
		len--;
	} while (len >= 0);
}

char* PCB::GetFileName() {
	return pName;
}
