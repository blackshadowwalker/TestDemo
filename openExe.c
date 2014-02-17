#include "stdio.h"
#define MAX_PATH 256
int main(int argc, char* argv[])
{
	char cmd[MAX_PATH]={0};
	if(argc<2)
	{
		return;
	}
	sprintf(cmd, "start %s", argv[1]);
	system(cmd);
	return 0;
}