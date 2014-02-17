

//public.h

/**************** defines ***************************/

#define REG_NOT_REGISTER	-1 //没注册
#define REG_INIT			-2 //首次使用
#define REG_SYSTEM_ERROR	-3 //系统故障
#define REG_UNKNOW			0 
#define REG_REGISTER		1 //已注册

/*************************************************/

void EventLog(char* info);//历史记录文件

//注册表相关操作
bool		OpenReg(HKEY &hKEY, char *data_Set);
BYTE*		QueryReg(char *key, DWORD dataLen, HKEY hKEY);
long		CreateReg(HKEY hKEY,const char *subkey);
bool		WriteReg(HKEY &hKEY, char *data_Set);
bool		SetReg(char *key, BYTE* value, DWORD dataLen, HKEY hKEY);
//\**************************************************************/

//******** date time classes **************
char *formatTime(time_t* t);
char *formatTMTime(struct tm *time);
void Encryption(BYTE *data, char *key="karl.li@teleframe.cn");
void Decryption(BYTE *data, char *key="karl.li@teleframe.cn");

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

long checkCanRun(void);
