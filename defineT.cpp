#include "iostream"
#include "string"
using namespace std;

#define example(instr) (printf("the input string is:\t%s\n",#instr))
#define example1(instr) #instr
#define	DH_error_MYERROR	"������Ϣ"
#define NET_DVR_NOERROR_MSG		1503

#define	NET_DVR_NOERROR	"û�д���"//	0
#define	NET_DVR_PASSWORD_ERROR	"�û����������ע��ʱ������û��������������"//	1

#define getmessageArg(defineMsg)	NET_DVR_##defineMsg

void main()
{
	char ch1[] = "abc";;
	char ch2[] = "fdsf";
	string str=example1( ch1 ch2);
//	DWORD data = NET_DVR_NOERROR_MSG;
	long ret = 1503;

	printf("%s \n",getmessageArg(NOERROR));
}