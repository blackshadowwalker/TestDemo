#include "iostream"
using namespace std;

typedef struct _stu1
{
	char name[20];
}stu1;

#pragma pack(24)
typedef struct _stu
{
	char a;
	char b;
	double c;
	stu1 *ptr;
}stu;

stu Stu(int _a, int _b, double _c)
{
	stu t = {_a, _b, _c};
	return t;
}
static int num=0;

struct QueueImage
{
	long			length;
	char	*buffer;
	char  name[256];
	char *fdsk;
	char  sex[64];

	QueueImage()
	{
		length=0;
		buffer = 0;
			this->buffer = new char[10];
		num = 0;
		printf("this is  QueueImage! \n");
	}
	~QueueImage()
	{
		length=0;
		buffer = 0;
		printf("this is  ~QueueImage!  num = %d\n", num++);
		delete buffer;
		buffer = 0;
	}
	
};//Í¼ÏñÖ¡
//typedef QueueImage QueueImage;

int main(int argc, char* argv[])
{
	printf(" %%  %d \n", sizeof(stu));

	QueueImage *ptimage;
	ptimage = new QueueImage;
//	memset(ptimage, 0, sizeof(QueueImage));

	int p = 0;
	p = 0x003a1558;
	sprintf(ptimage->buffer, "this!");
	delete ptimage;
	sprintf((char*)p, "this is data222222222222!");
	printf("");
/*
	stu **p;
	stu1 *p2 = new stu1;
	sprintf(p2->name, "karl.li");
	p = new stu*[5];
	for(int k=0; k<5; k++)
	{
		p[k] = new stu;
		memset(p[k], 0, sizeof(stu));
		p[k]->ptr = p2;
	}
	*/
/*	stu a ;
	a = Stu(1 , 2 , -3);
//	memcpy( &a, &b, sizeof(stu));
	printf(" %d %d %f ", a.a, a.b, a.c);
	*/
	return 0;
}