void memset(char* str,int k,int n)
{
	int i;
	for(i = 0; i< n ; i++)
		str[i] = (char)k;
}

void XoaKyTuDau(char* str)
{
	int i = 0;
	while(str[i] != '\0')
	{
		str[i] = str[i+1];
		i++;
	}
}

int GhepChuoi(char* str1,char* str2)
{
	int dem1 = 0;
	int dem2 = 0;
	//tim do dai 2 chuoi ky tu
	
	while(str1[dem1] != '\0')
		++dem1;

	//kiem tra xau 1 con du cho chua xau2 ko
	
	//copy chuoi 2 vao chuoi 1
	while(str2[dem2] != '\0')
		str1[dem1++] = str2[dem2++];
	str1[dem1] = '\0';
}

