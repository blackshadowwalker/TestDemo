// public.cpp
#pragma once

#include "public.h"

TCHAR* g_GetIniPath(void)
{
	static TCHAR* pszIniPath = NULL;
	if( NULL == pszIniPath)
	{
		TCHAR szDirBuf[512] = {0};
		pszIniPath = new TCHAR[512];
		memset(pszIniPath, 0, 512);
		GetCurrentDirectory(512, szDirBuf);
		sprintf(pszIniPath, "%s\\langchn.ini", szDirBuf);
	}
	return pszIniPath;
}

CString ConvertString(CString strText)
{
	char *val = new char[200];
	CString strIniPath,strRet;
	
	memset(val,0,200);
	GetPrivateProfileString("String",strText,"",
		val,200,g_GetIniPath());
	strRet = val;
	if(strRet.GetLength()==0)
	{
		//If there is no corresponding string in ini file ,then set it to be default value.
		strRet=strText;
	}
	delete val;
	return strRet;
}
//Set static text in dialogue box (English->current language)
void g_SetWndStaticText(CWnd * pWnd)
{
	CString strCaption,strText;
	
	//Set main window title
	pWnd->GetWindowText(strCaption);
	if(strCaption.GetLength()>0)
	{
		strText=ConvertString(strCaption);
		pWnd->SetWindowText(strText);
	}
	
	//Set small window title 
	CWnd * pChild=pWnd->GetWindow(GW_CHILD);
	CString strClassName;
	while(pChild)
	{
		//////////////////////////////////////////////////////////////////////////		
		//Added by Jackbin 2005-03-11
		strClassName = ((CRuntimeClass*)pChild->GetRuntimeClass())->m_lpszClassName;
		if(strClassName == "CEdit")
		{
			//Next small window 
			pChild=pChild->GetWindow(GW_HWNDNEXT);
			continue;
		}
		
		//////////////////////////////////////////////////////////////////////////	
		
		//Set small window current language text 
		pChild->GetWindowText(strCaption);
		strText=ConvertString(strCaption);
		pChild->SetWindowText(strText);
		
		//Next small window 
		pChild=pChild->GetWindow(GW_HWNDNEXT);
	}
}

TCHAR* g_GetSetPath(void)
{
	static TCHAR* pszIniPath = NULL;
	if( NULL == pszIniPath)
	{
		TCHAR szDirBuf[512] = {0};
		pszIniPath = new TCHAR[512];
		memset(pszIniPath, 0, 512);
		GetCurrentDirectory(512, szDirBuf);
		sprintf(pszIniPath, "%s\\set.ini", szDirBuf);
	}
	return pszIniPath;
}


bool RemoveDir(const char* szFileDir)
{
/*	std::string strDir = szFileDir;
	if (strDir.at(strDir.length()-1) != '\\');
	strDir += '\\';
	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile((strDir + "*.*").c_str(),&wfd);
	if (hFind == INVALID_HANDLE_VALUE)
		return false;
	do
	{
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (stricmp(wfd.cFileName,".") != 0 &&
				stricmp(wfd.cFileName,"..") != 0)
				RemoveDir( (strDir + wfd.cFileName).c_str());
		}
		else
		{
			DeleteFile( (strDir + wfd.cFileName).c_str());
		}
	}
	while (FindNextFile(hFind,&wfd));
	FindClose(hFind);
	RemoveDirectory(szFileDir);
*/	return true;
	
} 

bool KCreateFolder(CString folder)
{
	folder += "/";
	folder.Replace("/","\\");
	int index=2;
	bool isOk = true;
	while(1)
	{
	//	folder.Delete(0, index+1);
		index = folder.Find("\\",index+1);
		index = index<0? folder.Find("/") : index;
		if(index<0) break;
		CString str = folder.Left(index);
		isOk = CreateDirectory( str, 0 );
		//if(!isOk） 
		//	return false;
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
BYTE* QueryReg(char *key,DWORD dataLen, HKEY hKEY)
{
	LPBYTE	data=new BYTE[dataLen];
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

bool WriteReg(HKEY &hKEY, char *data_Set)
{
	long	ret0=(::RegOpenKeyEx(HKEY_CURRENT_USER,data_Set,0,KEY_WRITE,&hKEY));
	if(ret0!=ERROR_SUCCESS)
	{
		return false;
	}
	return true;
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


char* GetWindowPath()
{
	char *path = new char[MAX_PATH];
	memset(path, 0, MAX_PATH);
	GetSystemDirectory(path, MAX_PATH);
//	GetModuleFileName(0, path, MAX_PATH);
	path[2]='\0';//
	/*for(int i=strlen(path); i>0; i--)
		if(path[i]=='/' || path[i]=='\\')
		{
			path[i+1]='\0';
			break;
		}*/
	strcat(path,"\\");
	return path;
}
bool GetModuleFilePath(char *path)
{
	if(path==NULL) 
	{
		path = new char[MAX_PATH];
		memset(path, 0, MAX_PATH);
	}
	GetModuleFileName(0, path, MAX_PATH);
	int len = strlen(path);
	char *p = path+len-1;
	while(*p--!= '\\' );
	*(p+1)='\0';
	return path;
}

#include "time.h"
char *formatTime(time_t* t)
{
	struct tm *time;
	time = localtime(t);
	char *str = new char[256];
	memset(str, 0, 256);
	sprintf(str,"%d-%d-%d  %d:%d:%d", time->tm_year+1900, time->tm_mon+1, time->tm_mday, time->tm_hour, time->tm_min, time->tm_sec );
	return str;
}
char *formatTMTime(struct tm *time)
{
	char *str = new char[256];
	memset(str, 0, 256);
	sprintf(str,"%d-%d-%d  %d:%d:%d", time->tm_year+1900, time->tm_mon+1, time->tm_mday, time->tm_hour, time->tm_min, time->tm_sec );
	return str;
}

void EventLog(char* info)
{
	char *path = new char[256];
	memset(path, 0, 256);
	GetModuleFilePath(path);
	char *path2 = GetWindowPath();
	sprintf(path,"d:\\EventLog.log");

	FILE *out = fopen(path,"a");
	if(out==0)
	{
		MessageBox(0, path, "Cannot Open EventLog.log",MB_OK);
		return ;
	}
	time_t timer;//time_t就是long int 类型
	struct tm *tblock;
	timer = time(NULL);//这一句也可以改成time(&timer);
	tblock = localtime(&timer);
//	fprintf(out, "\n%s\t%s",ctime(&timer), info);
	fprintf(out, "\n【%s】\t%s",formatTime(&timer), info);
	fclose(out);
}

//
void Decryption(BYTE *data, char *key)
{
	
}
void Encryption(BYTE *data, char *key)
{
	
}


long checkCanRun(void)
{
	HKEY hKEY;
	long hr=0;
	char *data_Set="Software\\Microsoft\\Windows\\CurrentVersion";
	char *data_Set2="Software\\Microsoft\\Windows";
	char *fileName = "9070492E-9D46-4436-BEEA-2CA820ACA6B8";
	FILE *fp = fopen( fileName, "rb");
	if(fp!=NULL)
	{
		fclose(fp);
		return 1;
	}

	if( !OpenReg(hKEY, data_Set))
	{
		EventLog("Open reg  [Software\\Microsoft\\Windows\\CurrentVersion] --- >false");
		if(OpenReg(hKEY, data_Set2))
		{
			EventLog("CreateReg  [Software\\Microsoft\\Windows\\CurrentVersion]");
			CreateReg(hKEY, "CurrentVersion");
			goto end;
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
				goto end;
			}
			else
			{
				RegTime regtime;
				memset(&regtime, 0, sizeof(regtime));
				time_t startTime = time(NULL);
				localtime(&startTime);
				regtime.startTime = startTime;
				regtime.lastTime = startTime;
				regtime.endTime = regtime.startTime + days*24*60*60;
				regtime.flag = 852;
				regtime.leftTime = regtime.getSpan();
				sprintf(regtime.desp,"%s", fileName);
				BYTE *data = new  BYTE[sizeof(regtime)];
				memcpy(data, &regtime, sizeof(regtime));
			//	Encryption((BYTE*)data);
				SetReg("default", data, dataLen, hKEY);
				RegCloseKey(hKEY);
				return REG_INIT;
			}//if(!WriteReg(hKEY, data_Set))
		}
		else
		{
			//读取配置
			char info[256]={0};
			EventLog("属性存在，读取配置");
			RegTime regtime;
			memcpy(&regtime, data,  sizeof(regtime));

			time_t localTime = time(NULL);
			localtime(&localTime);
			sprintf(info, "local time: [ %s ]",formatTime(&localTime) );
			EventLog(info);

			long span = localTime - regtime.startTime;
			
			if( regtime.flag != 852 || span<0  || regtime.leftTime < 60)
			{
				
				sprintf(info, "Error :  [flag: %d] [span:%d] [leftTime:%d]",regtime.flag, span, regtime.leftTime );
				EventLog(info);
				regtime.leftTime = 0;
				regtime.flag = -1;
				hr = REG_NOT_REGISTER;
			}
			
			if( localTime < regtime.lastTime )
			{
				sprintf(info, "Error Local time has been changed to pre:  [localTime: %d] [lastTime:%d] ",localTime, regtime.lastTime );
				EventLog(info);
				regtime.lastTime += 60;
				hr = REG_NOT_REGISTER;
			}
			else
			{
				regtime.lastTime = localTime;
				regtime.leftTime = regtime.getSpan() - span ; 
				if( span/3600 <= days*24)
				{
					hr = REG_REGISTER;
				}
			}
			
			double leftTime = regtime.leftTime*1.0;
			sprintf(info, "试用期还剩 %.3g days( %.2g hours ( %.2g min ( %.1g sec ))) ", leftTime/60/60/24 , leftTime/60/60,  leftTime/60, leftTime);
			EventLog(info);

			RegCloseKey(hKEY);
			if(WriteReg(hKEY, data_Set))
			{
				BYTE *data = new  BYTE[sizeof(regtime)];
				memcpy(data, &regtime, sizeof(regtime));
			//	Encryption((BYTE*)data);
				SetReg("default", data, dataLen, hKEY);
				RegCloseKey(hKEY);
			}
			
			return hr;
		}//if(QueryReg(info, dataLen, hKEY)==NULL)
		return REG_NOT_REGISTER;
	}//if( !OpenReg(hKEY, data_Set))

end:
	return REG_SYSTEM_ERROR;

return 0;
}