#include "stdio.h"
#include "windows.h"
#include "conio.h"
typedef struct
{
	char name[50];
	char content[100];
}Test;

int main(int argn, char** argc)
{
	Test t  = {"abc.txt", "this is content"};
//	memset(&t, 0, sizeof(t));
	FILE *fp = fopen("readExeSelf1.exe", "ab");//"readExeSelf1"  argc[0]
	if(fp==0) 
	{
		printf("connot open [%s] \n" , argc[0]);
		return -1;
	}
//	fseek(fp, sizeof(t), 2);	fread(&t, 1, sizeof(t),fp);
	fseek(fp, 0, 2);	fwrite(&t, 1, sizeof(t), fp);
	printf("name:	\t[%s] \n", t.name);
	printf("content:	\t[%s] \n",t.content);
	fclose(fp);
	getch();
}