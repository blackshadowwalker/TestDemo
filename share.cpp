#include <stdio.h>

#pragma data_seg("flag_data")
	int app_count = 0;
	char str[256]={"\0"};
#pragma data_seg()

#pragma comment(linker,"/SECTION:flag_data,RWS")


void main(){
	printf("no_%d application \n", app_count);
	printf("str=%s \n" , str);
	
	if(app_count>5)     // 如果计数大于0，则退出应用程序。
	{
	   //MessageBox(NULL, "已经启动一个应用程序", "Warning", MB_OK);   
	   printf("已经启动一个应用程序 no%d application", app_count);
	   return ;
	}
	app_count++;
	sprintf(str, "已经启动 %d  应用程序 application", app_count);
	getchar();
}