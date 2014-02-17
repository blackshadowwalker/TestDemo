#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <process.h>
HANDLE hMutex;
HANDLE rMutex;
//HANDLE pMutex;//////
DWORD WINAPI Fun1Proc(LPVOID lpParameter);  
DWORD WINAPI Fun2Proc(
	LPVOID lpParameter ); 
DWORD WINAPI Fun3Proc(LPVOID lpParameter); 
using namespace std;
int Queue_NUM=30;
int j;
const int NODENUM=500000;
typedef int ElemType;
typedef struct queueNode
{
	ElemType data;
	struct queueNode *next;
}QueueNode;
typedef struct
{
	QueueNode * front;
	QueueNode * rear;
}LinkQueue;
LinkQueue *pQ;
volatile int   readcount = 0;   // 读者数目
void InitQueue(LinkQueue *HS)
{
	HS->front= new QueueNode;
	if(HS->front==NULL)
	{
		printf("内存分配失败！\n");
		return ;
	}
	HS->rear=HS->front;
	HS->front->next=NULL;
}
int IsEmpty(LinkQueue *HS)
{
	return HS->front==HS->rear;
}
void Print(LinkQueue *l){ //Print函数没有用信号量保护
	QueueNode *p;
	p=l->front->next;
	while(p!=NULL){
		cout<<p->data;
		p=p->next;
	}
}
void EnQueue(LinkQueue *HS,ElemType e)
{
	WaitForSingleObject(hMutex,-1);
	QueueNode *p;
	p=new QueueNode;
	if(p==NULL)
	{
		printf("内存分配失败！\n");
		return ;// 此处返回前，没有释放mutex
	}
	p->data = e;
	p->next = NULL;
	HS->rear->next=p;
	HS->rear=p;
	Print(HS);
	ReleaseMutex(hMutex);
}
ElemType DeQueue(LinkQueue *HS)
{
	WaitForSingleObject(hMutex,-1);
	QueueNode *first;
	ElemType temp;
	if(IsEmpty(HS))
	{
		printf("\n");
		return 0;// 此处返回前，没有释放mutex
	}
	Print(HS);
	first=HS->front->next;
	temp=first->data;
	HS->front->next=first->next;
	if(HS->rear=first)
		HS->rear=HS->front;
	delete first;
	Print(HS);
	return temp; //此处返回，没有释放mutex
		ReleaseMutex(hMutex);
}
QueueNode* Find(int data,LinkQueue *l){
	WaitForSingleObject(rMutex,-1);
	if(readcount==0)
		WaitForSingleObject(hMutex,-1);
	readcount++;
	ReleaseMutex(rMutex) ;   //释放互斥信号
	QueueNode *p;
	p=l->front->next;
	WaitForSingleObject(rMutex,-1) ;  //代码位置不正确，而且还缺少一些
		readcount-- ;//读者数目减少
	while(p){
		if(p->data==data){return p;}// 此处返回前，没有释放mutex
			p=p->next;
	}
	return NULL;
}
void Clear(LinkQueue *HS)  //没有mutex保护
{
	QueueNode *p,*q;
	p=HS->front->next;
	while(p!=NULL)
	{
		q=p;
		p=p->next;
		delete q;
	}
	HS->front->next=NULL;
	HS->rear=HS->front;
}
void Destroy(LinkQueue *HS)
{
	QueueNode *p,*q;
	p=HS->front;
	while(p!=NULL)
	{
		q=p;
		p=p->next;
		delete q;
	}
	HS->rear=HS->front=NULL;
}
DWORD WINAPI Fun1Proc(LPVOID lpParameter)
{       
	for(j=0;j<Queue_NUM;j++)
	{
		cout<<"入队 : "<<j<<endl;
		EnQueue(pQ,j);
		Sleep(100);
	}
	return true;        
}
DWORD WINAPI Fun2Proc(LPVOID lpParameter)
{   
	for(j=Queue_NUM;j>0;j--){
		cout<<"出队j : "<<j<<endl;
		DeQueue(pQ);
		Sleep(100);
	}
	return true;
}
DWORD WINAPI Fun3Proc(LPVOID lpParameter)
{
	cout<<"查找队列";
	int data;
	LinkQueue *l;
	cin>>data;
	Find(data,l);
	return true;
}
void output1()
{
	int i;
	for(i=0;i<10;i++)
		printf(" ");
	for(i=0;i<32;i++)
		printf("*");
	printf("\n");
}
void mainpp()
{
	int i;
	output1 ();
	for(i=0;i<10;i++) printf(" ");printf("*  ");
	printf("1.自动进队");
	for(i=0;i<16;i++) printf(" ");printf("* "); printf("\n");
	for(i=0;i<10;i++) printf(" ");printf("*  ");
	printf("2.自动出队");
	for(i=0;i<16;i++) printf(" ");printf("* "); printf("\n");
	for(i=0;i<10;i++) printf(" ");printf("*  ");
	printf("3.选择进队数目");
	for(i=0;i<16;i++) printf(" ");printf("* "); printf("\n");
	for(i=0;i<10;i++) printf(" ");printf("*  ");
	printf("4.选择出队数目");
	for(i=0;i<16;i++) printf(" ");printf("* "); printf("\n");
	for(i=0;i<10;i++) printf(" ");printf("*  ");
	printf("5.队列致空");
	for(i=0;i<16;i++) printf(" ");printf("* "); printf("\n");
	for(i=0;i<10;i++) printf(" ");printf("*  ");
	printf("6.打印队列");
	for(i=0;i<16;i++) printf(" ");printf("* "); printf("\n");
	for(i=0;i<10;i++) printf(" ");printf("*  ");
	printf("7.队列查找");
	for(i=0;i<16;i++) printf(" ");printf("* "); printf("\n");
	output1();
}
void main()
{
	int m;
	QueueNode *pS;
	ElemType e;
	pS=new QueueNode;
	pQ=new LinkQueue;
	InitQueue(pQ);
	mainpp();
	HANDLE hThread1;
	HANDLE hThread2;
	HANDLE hThread3;
	//     for(int i=0;i<10;i++)
	while(1)
	{
		// WaitForSingleObject(pMutex,INFINITE);//////
		printf("请选择1--2,3以下为手动执行：\n ");scanf("%d",&m);
		switch(m)
		{
		case 0: return;
		case 1:{
			WaitForSingleObject(hMutex,INFINITE);
		//	建立线程前，用mutex保护，为什么？
				cout<<"程序自动执行入队："<<endl;
			hThread1=CreateThread(NULL,0,Fun1Proc,NULL,0,NULL);
			//     cout<<"请选择1--2,3以下为手动执行："<<endl;
			ReleaseMutex(hMutex);
			//     ReleaseMutex(pMutex);//////
			break;
			   }
		case 2:{
			WaitForSingleObject(hMutex,INFINITE);
			cout<<"程序自动执行出队："<<endl;
			hThread2=CreateThread(NULL,0,Fun2Proc,NULL,0,NULL);
			ReleaseMutex(hMutex);
			break;
			   }
		case 3:{
			cout<<"请输入你想要入队的大小：";
			cin>>j;
			for(int i=1;i<=j;i++)
			{cout<<"the "<<i<<" number:"<<endl;
			cin>>e;
			EnQueue (pQ,e);
			}
			break;
			   }
		case 4:{
			cout<<"请输入你要出队的数目：";
			cin>>Queue_NUM;
			for(int i=1;i<=Queue_NUM;i++)
			{DeQueue (pQ);}
			cout<<endl;
			break;
			   }
		case 5:
			{
				printf("队列致空：\n");
				Clear(pQ);
				printf("\n");
				break;
			}
		case 6:
			{
				printf("输出队列：\n");
				Print(pQ);//  前面的Print函数没有mutex保护
					printf("\n");
				break;
			}
		case 7:
			{
				WaitForSingleObject(hMutex,INFINITE);
				printf("查找：\n");
				int i;
				cin>>i;
				cout<<Find(i,pQ);
				hThread3=CreateThread(NULL,0,Fun3Proc,NULL,0,NULL);
				printf("\n");
				ReleaseMutex(hMutex);
				break;
			}
		default: return; // retrun程序就直接退出了，不好。
		}
	}
	CloseHandle(hThread1);
	CloseHandle(hThread2);
	CloseHandle(hThread3);
	WaitForSingleObject(hMutex,INFINITE); 这一句写的有问题
		ReleaseMutex(hMutex); //这些也是。
		ReleaseMutex(hMutex);
	ReleaseMutex(hMutex);
}

