#include "stdio.h"
#include "conio.h"
#include "windows.h"
int main()
{
	FILE *fp=0;
	char fileName[256]={0};
	long p=0;
	CreateDirectory("testFolder", 0);
	putchar(10);
	for(p=1; p<=1000000; p++)
	{
		sprintf(fileName,"testFolder/file_%d.txt", p);
		fp = fopen(fileName, "w");
		fprintf(fp,"this is the %d-th file in folder [testFolder].", p );
		fclose(fp);
		if(p%100==0)		printf("\rCreating the %d-th file....", p);
	}
	printf("\nDone!");
	getch();
	return 0;
}