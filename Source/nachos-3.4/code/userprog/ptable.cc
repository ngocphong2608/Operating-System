#include "ptable.h"
#include "system.h"

PTable::PTable() {
	for (int i = 0; i < MAX_PROCESS; i++)
		pcb[i] = NULL;
	
	bm = new BitMap(MAX_PROCESS);
	bm->Mark(0);
	
	bmsem = new Semaphore("PTable->Semaphore", 1);
	
	// tao pcb cua process dau tien
	pcb[0] = new PCB(0);
	pcb[0]->SetFileName("./test/shell");
	pcb[0]->thread = currentThread;
	pcb[0]->parentID = -1;
}

PTable::~PTable() {
	for (int i = 0; i < MAX_PROCESS; i++)
		delete pcb[i];
	delete bm;
	delete bmsem;
}

int  PTable::ExecUpdate(char* name) {
	// để giúp tránh tình trạng nạp 2 tiến trình cùng 1 lúc.
	bmsem->P();
	
	// Kiểm tra sự tồn tại của chương trình “name” bằng cách gọi phương thức Open của lớp fileSystem
	OpenFile *executable = fileSystem->Open(name);
	
	if (executable == NULL) {
		printf("PTable::ExecUpdate: Unable to open file %s\n", name);
		bmsem->V();
		return -1;
	}
	
	// So sánh tên chương trình và tên của currentThread để chắc chắn rằng chương trình này không gọi
	//thực thi chính nó.
	if (strcmp(name, currentThread->getName()) == 0) {
		printf("PTable::ExecUpdate: Thread %s executes itself\n", name);
		bmsem->V();
		return -1;
	}
	
	//Tìm slot trống trong bảng Ptable
	int pos = GetFreeSlot();
	//printf("\n%d\n", pos);
	
	if (pos == -1) {
		printf("PTable::ExecUpdate: PTable was full size\n");
		bmsem->V();
		return -1;
	}
	
	// khởi tạo một PCB mới với processID chính là index của slot này
	pcb[pos] = new PCB(pos);
	pcb[pos]->SetFileName(name);
	
	// parrentID là processID của currentThread.
	pcb[pos]->parentID = currentThread->processID;
	//printf("\n%d\n", currentThread->processID);
	
	// Gọi thực thi phương thức Exec của lớp PCB.
	int id = pcb[pos]->Exec(name, pos); 
	
	bmsem->V();
	
	return id;
}

int  PTable::ExitUpdate(int ec) {
	int id = currentThread->processID;
	if (id == 0) {
		//printf("\n%s\n", currentThread->getName());
		interrupt->Halt();
	}
	
	//kiem tra che do background
	if(pcb[id]->IsBG == true)
	{
		Remove(id);
		return ec;
	}
	
	//kiem tra id co ton tai ko
	if(IsExist(id) == false)
	{
		printf("\nPTable::ExitUpdate : ID %d do not exist ",id);
		return -1;
	}

	pcb[id]->SetExitCode(ec);
	pcb[pcb[id]->parentID]->DecNumWait();
	
	pcb[id]->JoinRelease();

	pcb[id]->ExitWait();

	Remove(id);
	return ec;
}

// tra ve exitcode
int  PTable::JoinUpdate(int id) {
	// kiểm tra tính hợp lệ của processID id
	if (id < 0 || id >= MAX_PROCESS) {
		printf("PTable::JoinUpdate: Id was out of range\n");
		return -1;
	}
	if (pcb[id] == NULL) {
		printf("PTable::JoinUpdate: Process %d not exist\n", id);
		return -1;
	}
	
	// kiểm tra tiến trình gọi Join có phải là cha của tiến trình
	// có processID là id hay không
	if (pcb[id]->parentID != currentThread->processID) {
		printf("PTable::JoinUpdate: Current thread is not parent of thread %d", id);
		return -1;
	}
	
	// Tăng numwait và gọi JoinWait() để chờ tiến trình con thực hiện
	pcb[pcb[id]->parentID]->IncNumWait();

	// khong phai mode background
	pcb[id]->IsBG = false;
	
	//doi den khi tien trinh con ket thuc
	pcb[id]->JoinWait();
	
	//Xử lý exitcode
	int ec = pcb[id]->GetExitCode();
	
	// ExitRelease() để cho phép tiến trình con thoát
	pcb[id]->ExitRelease();
	
	return ec;
}

int  PTable::GetFreeSlot() {
/*	for (int i = 0; i < MAX_PROCESS; i++)
		if (pcb[i] == NULL)
			return i;
	return -1;*/
	return bm->Find();
}

bool  PTable::IsExist(int pid) {
/*	if (pid < 0 || pid >= MAX_PROCESS)
		return false;
	return pcb[pid] != NULL;*/
	return bm->Test(pid);
}

void  PTable::Remove(int pid) {
/*	if (pid < 0 || pid >= MAX_PROCESS)
		return;
	if (pcb[pid] != NULL) {
		//printf("\n%s\n", pcb[pid]->GetFileName());
		delete pcb[pid];
		pcb[pid] = NULL;
	}*/
	return bm->Clear(pid);
}

char*  PTable::GetFileName(int id) {
	if (id < 0 || id >= MAX_PROCESS || pcb[id] == NULL)
		return NULL;
	return pcb[id]->GetFileName();
}

/*
int PTable::GetID(char* name) {
	for (int i = 0; i < MAX_PROCESS; i++)
		if (pcb[i] != NULL and strcmp(pcb[i]->getFileName(). name) == 0)
			return i;
	return -1;
}*/
