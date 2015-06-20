#include "syscall.h"

int
main()
{
	int N, i, j;
	int A[100];
	PrintString("Nhap vao so phan tu N mang (N <= 100): ");
	N = ReadInt();
	PrintString("Nhap vao cac phan tu cua mang: \n");
	for(i = 0; i < N; i++){
		PrintString("Nhap vao phan tu thu ");
		PrintInt(i+1);
		PrintString(":\n");
		A[i] = ReadInt();	
	}
	
	//bubble sort
	for(i = N-1; i >= 0; i--){
		for(j = 0; j < i; j++)
			if(A[j] > A[j+1]){
				int temp = A[j];
				A[j] = A[j+1];
				A[j+1] = temp;
			}
	}

	
	//Print Array 
	PrintString("Mang vua nhap la: ");
	for(i = 0; i < N; i++){
		PrintInt(A[i]);
		PrintChar(32);
	}
	
    /* not reached */

	return 0;
}
