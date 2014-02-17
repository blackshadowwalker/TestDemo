#include <stdio.h>
#include <windows.h>

int main(int argn , char **argc)
{
	HKEY hKEY;
	char	*data_Set="Software\\Microsoft\\Windows\\CurrentVersion";
	long	ret0=(::RegOpenKeyEx(HKEY_CURRENT_USER,data_Set,0,KEY_WRITE,&hKEY));
	if(ret0!=ERROR_SUCCESS)
	{
		printf("ERROR[RegOpenKeyEx]\n");
		return false;
	}
	DWORD	type_1 = REG_BINARY;
	BYTE	*value=new BYTE[1];
	memset(value, 0, 1);
	char	*key = "default";
	DWORD	dataLen=1;
	long ret1=::RegDeleteValue(hKEY,key);
	if(ret1!=ERROR_SUCCESS)
	{
		printf("[Error]RegSetValueEx\n");
		return false;
	}
	return 0;
}