#include <stdio.h>

#pragma data_seg("flag_data")
	int app_count = 0;
	char str[256]={"\0"};
#pragma data_seg()

#pragma comment(linker,"/SECTION:flag_data,RWS")


void main(){
	printf("no_%d application \n", app_count);
	printf("str=%s \n" , str);
	
	if(app_count>5)     // �����������0�����˳�Ӧ�ó���
	{
	   //MessageBox(NULL, "�Ѿ�����һ��Ӧ�ó���", "Warning", MB_OK);   
	   printf("�Ѿ�����һ��Ӧ�ó��� no%d application", app_count);
	   return ;
	}
	app_count++;
	sprintf(str, "�Ѿ����� %d  Ӧ�ó��� application", app_count);
	getchar();
}