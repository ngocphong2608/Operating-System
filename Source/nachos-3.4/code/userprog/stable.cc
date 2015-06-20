#include "stable.h"

STable::STable() {
	bm = new BitMap(MAX_SEMAPHORE);
	
	for (int i = 0; i < MAX_SEMAPHORE; i++)
		semTab[i] = NULL;
}

STable::~STable() {
	delete bm;
	for (int i = 0; i < MAX_SEMAPHORE; i++)
			delete semTab[i];
}

int STable::Create(char* name, int init) {
	if (getSemPos(name) != -1) {
		// push notification error semaphore name already exist 
		printf("Stable::Create: Semaphore %s already exist\n", name);
		return -1;
	}
	
	int pos = FindFreeSlot();
	if (pos == -1) {
		// push notification error STable was fully
		printf("Stable::Create: Stab was full\n");
		return -1;		
	}
	
	// create new semaphore
	semTab[pos] = new Sem(name, init);
	
	return 0;
}

int STable::Wait(char* name) {
	int pos = getSemPos(name);
	if (pos == -1) {
		// push notification error semaphore name not exist 
		printf("STable::Wait: Semaphore %s do not exist\n", name);
		return -1;
	}
	semTab[pos]->wait();
	return 0;
}

int STable::Signal(char* name) {
	int pos = getSemPos(name);
	if (pos == -1) {
		// push notification error semaphore name not exist 
		printf("STable::Signal: Semaphore %s do not exist\n", name);
		return -1;
	}
	semTab[pos]->signal();
	return 0;
}

int STable::FindFreeSlot() {
/*	for (int i = 0; i < MAX_SEMAPHORE; i++)
		if (semTab[i] == NULL)
			return i;
	return -1;*/
	return bm->Find();
}

int STable::getSemPos(char* name) {
	for (int i = 0; i < MAX_SEMAPHORE; i++)
		if (semTab[i] != NULL && strcmp(semTab[i]->GetName(), name) == 0)
			return i;
	return -1;
}
