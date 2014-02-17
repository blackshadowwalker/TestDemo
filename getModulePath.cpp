#include "stdio.h"
#include "windows.h"
#include "time.h"

#define REG_NOT_REGISTER	-1 //没注册
#define REG_INIT			-2 //首次使用
#define REG_SYSTEM_ERROR	-3 //系统故障
#define REG_UNKNOW			0 
#define REG_REGISTER		1 //已注册
#define REG_SYS_TIME_CHANGED	-4 //系统时间被改变
#define REG_SYS_REG_CHANGED		-5 //注册表数据被改变

BYTE* QueryReg(char *key,DWORD dataLen, HKEY hKEY);
void Encryption(BYTE *data, long lenData,  BYTE *key, long lenKey);

bool GetModuleFilePath(char *path)
{
	if(path==NULL) 
	{
		path = new char[MAX_PATH];
		memset(path, 0, MAX_PATH);
	}
	GetModuleFileName(0, path, MAX_PATH);
	GetSystemDirectory(path, MAX_PATH);
	int len = strlen(path);
	char *p = path+len-1;
	while(*p--!= '\\' );
	*(p+1)='\0';

	return true;
}
char *formatTMTime(struct tm *time)
{
	char *str = new char[256];
	sprintf(str,"%d-%d-%d  %d:%d:%d", time->tm_year+1900, time->tm_mon+1, time->tm_mday, time->tm_hour, time->tm_min, time->tm_sec );
	return str;
}


bool getRandString(char *str)
{
	int len,c;
	if(str==0)
		str = new char[256];
	memset(str, 0, 256);
	char *p = str;
//	randomize();
	len=rand() % 50 + 1;
	while(len--)
	{  
		switch(rand()%3){    
			case 0:c='A'+rand()%26;break;    
			case 1:c='a'+rand()%26;break;    
			case 2:c='0'+rand()%10;break;  
		}  
		*p = c;
		p++;
	}
	return true;
}


typedef struct 
{
	char desp[256];
	time_t startTime;
	time_t endTime;
	time_t lastTime;
	time_t leftTime;// Remaining  time
	int leftTimes;// Remaining  times
	int flag;
	char info[256];
	time_t getSpan()
	{
		return (time_t)(endTime - startTime);
	}
	struct tm* getTM(time_t t)
	{
		struct tm *tblock;
		tblock = localtime(&t);
		return tblock;
	}
	time_t parse(char *strTime, char *_Format="%d-%d-%d  %d:%d:%d")// %d-%d-%d  %d:%d:%d
	{
		struct tm time;
		sscanf(strTime, _Format, &time.tm_year, &time.tm_mon, &time.tm_mday, &time.tm_hour, &time.tm_min, &time.tm_sec);
		time.tm_year -= 1900;
		time.tm_mon -= 1;
		time_t t = mktime(&time);
		return t;
	}
}RegTime;


bool WriteReg(HKEY &hKEY, char *data_Set)
{
	long	ret0=(::RegOpenKeyEx(HKEY_CURRENT_USER,data_Set,0,KEY_WRITE,&hKEY));
	if(ret0!=ERROR_SUCCESS)
	{
		return false;
	}
	return true;
}
bool OpenReg(HKEY &hKEY, char *data_Set)
{
	long	ret0=(::RegOpenKeyEx(HKEY_CURRENT_USER,data_Set,0,KEY_READ,&hKEY));
	if(ret0!=ERROR_SUCCESS)
	{
		return false;
	}
	return true;
}

long CreateReg(HKEY hKEY,const char *subkey)
{
    DWORD dwDisposition;
	return RegCreateKeyEx(HKEY_CURRENT_USER, subkey,         
           0,             
           NULL,             
           REG_OPTION_NON_VOLATILE,
           KEY_ALL_ACCESS,
           NULL,           
           &hKEY,          
           &dwDisposition );
}


bool SetReg(char *key, BYTE* value, DWORD dataLen, HKEY hKEY)
{
	DWORD	type_1 = REG_BINARY;
	long ret1=::RegSetValueEx(hKEY,key,NULL,type_1,(CONST BYTE*)value,dataLen);
	if(ret1!=ERROR_SUCCESS)
	{
//		AfxMessageBox("错误：无法设置有关的注册表信息");
		return false;
	}
	return true;
}

char *formatTime(time_t* t)
{
	struct tm *time;
	time = localtime(t);
	char *str = new char[256];
	memset(str, 0, 256);
	sprintf(str,"%d-%d-%d  %d:%d:%d", time->tm_year+1900, time->tm_mon+1, time->tm_mday, time->tm_hour, time->tm_min, time->tm_sec );
	return str;
}
void EventLog(char* info)
{
	char *path = new char[256];
	GetModuleFilePath(path);
//	path[2]=0;
	strcat(path,"\\EventLog.log");
	FILE *out = fopen(path,"a");
	if(out==0)
		return ;
	time_t timer;//time_t就是long int 类型
	
	struct tm *tblock;
	timer = time(NULL);//这一句也可以改成time(&timer);
	tblock = localtime(&timer);
//	printf("%s \n", ctime(&timer));
	fprintf(out, "\n%s \t%s", formatTime(&timer), info);
	fclose(out);
}


long checkCanRun(void)
{
	HKEY hKEY;
	long hr=0;
	char *data_Set="Software\\Microsoft\\Windows\\CurrentVersion";
	char *data_Set2="Software\\Microsoft\\Windows";
	char *fileName = "9070492E-9D46-4436-BEEA-2CA820ACA6B8";
	char info[512]={0};

	RegTime regtime;


	FILE *fp = fopen( fileName, "rb");
	if(fp!=NULL)
	{
		fclose(fp);
		hr =  REG_REGISTER;
	}
	else
	{
		if( !OpenReg(hKEY, data_Set))
		{
			EventLog("Open reg  [CV] --- >false");
			if(OpenReg(hKEY, data_Set2))
			{
				EventLog("CreateReg  [CV]");
				CreateReg(hKEY, "CurrentVersion");
				hr =  REG_SYSTEM_ERROR;
			}
		}
		else
		{
			EventLog("Open reg  --- >true");
			DWORD dataLen = 0 ;
			int days = 7;//可以使用的天数
			dataLen = sizeof(RegTime);

			BYTE *data = new BYTE[dataLen];
			data = QueryReg("default", dataLen, hKEY);
			if(data==NULL)
			{
				//属性不存在，首次写入
				RegCloseKey(hKEY);
				EventLog("属性不存在，首次写入");
				if(!WriteReg(hKEY, data_Set))
				{
					EventLog("WriteReg ==> false");
					hr = REG_SYSTEM_ERROR;
				}
				else
				{
					memset(&regtime, 0, sizeof(regtime));
					time_t startTime = time(NULL);
					localtime(&startTime);
					regtime.startTime = startTime;
					regtime.lastTime = startTime;
					regtime.endTime = regtime.startTime + days*24*60*60;
					regtime.flag = 852;
					regtime.leftTime = regtime.getSpan();
					sprintf(regtime.desp,"teletrameId:%s", fileName);
					sprintf(regtime.info,"info:%s", fileName);
					BYTE *data = new  BYTE[sizeof(regtime)];
					memcpy(data, &regtime, sizeof(regtime));
					
					char *key = fileName;
					Encryption(data, dataLen  , (BYTE*)(key) , strlen(key));
					
					sprintf(info, "[dataLen : %d]", dataLen );
					EventLog(info);
					SetReg("default", data, dataLen, hKEY);
					RegCloseKey(hKEY);
					hr = REG_INIT;
					return hr;
				}//if(!WriteReg(hKEY, data_Set))
			}
			else
			{
				//读取配置
				
				EventLog("属性存在，读取配置");
				
				char *key = fileName;
				Encryption(data, dataLen , (BYTE*)(key) , strlen(key));
				
				memcpy(&regtime, data,  sizeof(regtime));

				time_t localTime = time(NULL);
				localtime(&localTime);
				sprintf(info, "local time: [ %s ]",formatTime(&localTime) );
				EventLog(info);

				if( regtime.flag != 852 )
				{
					sprintf(info, "Error :  [flag: %d] [leftTime:%d]",regtime.flag, regtime.leftTime );
					EventLog(info);
					regtime.leftTime = 0;
					hr = REG_SYS_REG_CHANGED;
				}
				else if(regtime.lastTime >= regtime.endTime || regtime.leftTime <60)
				{
					sprintf(info, "使用期限已到:[localTime: %d] [lastTime:%d] [from:%d to:%d]",localTime, regtime.lastTime, regtime.startTime, regtime.endTime );
					EventLog(info);
					regtime.lastTime += 2;
					hr = REG_NOT_REGISTER;
				}
				else if( localTime < regtime.lastTime)
				{
					sprintf(info, "Error Local time has been changed to pre:  [localTime: %d] [lastTime:%d] ",localTime, regtime.lastTime );
					EventLog(info);
					printf("time has been changed: \n localTime: %s \n lastTime:%s \n", formatTime(&localTime), formatTime(&regtime.lastTime) );
					regtime.lastTime += 60;
					hr = REG_SYS_TIME_CHANGED;
				}
				else
				{
					regtime.lastTime = localTime;
					regtime.leftTime = regtime.endTime - regtime.lastTime ; 
					hr = REG_REGISTER;
				}
				
				double leftTime = regtime.leftTime*1.0;
				sprintf(info, "试用期还剩 %.3f days( %.2f hours ( %.1f min ( %.0f sec ))) ", leftTime/60/60/24 , leftTime/60/60,  leftTime/60, leftTime);
				EventLog(info);

				RegCloseKey(hKEY);
				if(WriteReg(hKEY, data_Set))
				{
					BYTE *data = new  BYTE[sizeof(regtime)];
					memcpy(data, &regtime, sizeof(regtime));
					
					char *key = fileName;
					Encryption(data, dataLen  , (BYTE*)(key) , strlen(key));
					
					SetReg("default", data, dataLen, hKEY);
					RegCloseKey(hKEY);
				}
				
				
			}//if(QueryReg(info, dataLen, hKEY)==NULL)
			
		}//if( !OpenReg(hKEY, data_Set))
	}
	return hr;
}

void Encryption(BYTE *data, long lenData,  BYTE *key, long lenKey)
{
	int j=0;
	for(int i=0; i<lenData; i++,j++)
	{
		if(j==lenKey)
			j=0;
		data[i] = data[i] ^ key[j];
	}
}

BYTE* QueryReg(char *key,DWORD dataLen, HKEY hKEY)
{
	BYTE *data=new BYTE[dataLen+1];
	memset(data, 0, dataLen);
	DWORD	type_1=REG_BINARY;

	long ret1=::RegQueryValueEx(hKEY,key,NULL,&type_1,data,&dataLen);
	if(ret1!=ERROR_SUCCESS)
	{
		return NULL;
	}
	BYTE* str = new BYTE[dataLen];
	memcpy(str, data, dataLen);
//	delete data;
	return str;
}
int main()
{
	printf("start ");
	checkCanRun();
	
	HKEY hKEY;
	char *data_Set="Software\\Microsoft\\Windows\\CurrentVersion";
	char *fileName = "9070492E-9D46-4436-BEEA-2CA820ACA6B8";
	
	if( !OpenReg(hKEY, data_Set))
	{
		printf("hKEY error \n");
	}
	DWORD dataLen = 0 ;
	int days = 7;//可以使用的天数
	dataLen = sizeof(RegTime);
	printf("dataLen:%d \n" , dataLen);
	return 0;
	BYTE *data=0;
	data = QueryReg("default", dataLen, hKEY);

	char *key = fileName;
	Encryption(data, dataLen , (BYTE*)(key) , strlen(key));
	
	char info[256]={0};
	RegTime regtime;
	memcpy(&regtime, data,  sizeof(regtime));


	time_t localTime = time(NULL);
	localtime(&localTime);
	sprintf(info, "local time: [ %s ]",formatTime(&localTime) );
	RegTime rtime ;
	memset(&rtime , 0, sizeof(rtime));
	int len = sizeof(rtime);
	BYTE *strtime= new BYTE[100];
	len = strlen("2012-09-16 11:50:21");
	memcpy(strtime, "2012-09-16 11:50:21",len );
	strtime[len]=0;

	Encryption(strtime,len , (BYTE*)("karl") , 4);
	Encryption(strtime,len , (BYTE*)("karl") , 4);
	
	rtime.startTime = rtime.parse((char*)strtime);

	printf("len=%d \n", len);


	for(int i=0; i<10; i++)
	{

		getRandString(info);
		EventLog(info);
	//	Sleep(500);
	}
	return 0;
}