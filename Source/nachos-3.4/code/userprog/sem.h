#ifndef SEM_H
#define SEM_H
#include "synch.h"

class Sem {
private:  
	char name[50];  
	
	Semaphore *sem;			       // Tạo Semaphore để quản lý  
  
public:  
	// khởi tạo đối tượng Sem. Gán giá trị ban đầu là null  
	// nhớ khởi tạo bm sử dụng  
	Sem(char* na, int i){  
		strcpy(this->name,na);  
		sem = new Semaphore(name,i);  
	}

	~Sem(){				        // hủy các đối tượng đã tạo  
		delete sem;			           
	}  
						  
	void wait(){				// thực hiện thao tác chờ  
		sem->P();				
	}					 
						  
	void signal(){			    // thực hiện thao tác giải phóng Semaphore  
		sem->V();				
	}					 
						  
	char* GetName(){			// Trả về tên của Semaphore  
		return name;  
	}
};
#endif
