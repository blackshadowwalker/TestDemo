#include "stdio.h"
#include "windows.h"

#define CONNOT_OPEN_REG	 L"无法打开注册表"
#define CONNOT_SET_REG	 L"无法设置注册表"

char* serverName;

bool ChangeServiceConfig3(
	 SC_HANDLE    hService,
	 DWORD        dwInfoLevel,
	 LPVOID       lpInfo)
{
//	EnumServicesStatus();

/*	QUERY_SERVICE_CONFIG *lpServiceConfig = new QUERY_SERVICE_CONFIG[8*1024];
	DWORD retLen= sizeof(QUERY_SERVICE_CONFIG);
	QueryServiceConfig(hService, lpServiceConfig, 8*1024, &retLen );
*/	
	SERVICE_FAILURE_ACTIONS *info = (SERVICE_FAILURE_ACTIONS*)lpInfo;
	
//	serverName = lpServiceConfig->lpServiceStartName;
	char data_Set[MAX_PATH] = "SYSTEM\\CurrentControlSet\\services\\";
	strcat(data_Set, serverName);
	
	long ret=0;
	char key[MAX_PATH]={0};
	switch(dwInfoLevel)
	{
	case SERVICE_CONFIG_FAILURE_ACTIONS:
		memcpy(key, "FailureActions", strlen("FailureActions"));
		HKEY hKEY;
		int len1 = sizeof(SERVICE_FAILURE_ACTIONS) ;
		int len2 = 3*sizeof(SC_ACTION);
		DWORD dataLen = len1 + len2;
		
		BYTE  *data=new BYTE[dataLen];
		memset(data, 0, dataLen);
		DWORD	type_1=REG_BINARY;
		memcpy( data, info, len1);
		memcpy( data+len1, info->lpsaActions, len2);
		
		ret = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,data_Set,0,KEY_WRITE,&hKEY);
		
		if(ret!=ERROR_SUCCESS)
		{
			delete data;
	//		SetLastError(CONNOT_OPEN_REG);
			return false;
		}
		ret = ::RegSetValueEx(hKEY,key,NULL,type_1,(CONST BYTE*)data,dataLen);
		RegCloseKey(hKEY);
		if(ret!=ERROR_SUCCESS)
		{
			delete data;
	//		SetLastError(CONNOT_SET_REG);
			return false;
		}
		delete data;
		break;

	}

	return true;
}

int main()
{
	int resetDays = 5;//重置（天）
	int delay[3]={0};//delay(分钟)
	SC_ACTION_TYPE type[3] ;//SC_ACTION_TYPE
	
	delay[0] = 2;
	delay[1] = 5;
	delay[2] = 1;
	type[0] = SC_ACTION_REBOOT;
	type[1] = SC_ACTION_REBOOT;
	type[2] = SC_ACTION_NONE;
	
	
	SC_ACTION sc[3] ;
	memset(&sc, 0 ,sizeof(SC_ACTION)*3);
	for(int k=0; k<3; k++)
	{
		sc[k].Type = type[k]; 
		sc[k].Delay = delay[k] * 60 * 1000;
	}
	SERVICE_FAILURE_ACTIONS info;
	info.dwResetPeriod = resetDays * 3600 * 24;
	info.cActions = 3;
	info.lpCommand = NULL;
	info.lpRebootMsg = NULL;
	info.lpsaActions = sc;
	
	SC_HANDLE sch=OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
	if (sch==NULL)
	{
		return -1;
	}

	serverName = new char[MAX_PATH];
	memset(serverName, 0, MAX_PATH);
	memcpy(serverName, "SQLBrowser", strlen("SQLBrowser"));

    SC_HANDLE schService = OpenService( 
        sch,           // SCManager database 
        serverName,    // name of service 
        SC_MANAGER_ALL_ACCESS); // need CHANGE access
    if (schService == NULL) 
    {
        printf("OpenService failed (%d)\n", GetLastError()); 
        return FALSE;
    }
	
	if( !ChangeServiceConfig3(
        schService,                 
        SERVICE_CONFIG_FAILURE_ACTIONS, 
        &info) )                   
    {
        printf("%s Change Service Config failed (%d) \n",serverName , GetLastError());
        return -1;
    }
    else
        printf("%s Change Service Config succeeded\n",serverName);
	
	delete serverName;
    CloseServiceHandle(schService);
	
	getchar();getchar();
	
	return 0;
}