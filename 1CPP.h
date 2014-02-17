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
volatile int   readcount = 0;   // ������Ŀ
void InitQueue(LinkQueue *HS)
{
	HS->front= new QueueNode;
	if(HS->front==NULL)
	{
		printf("�ڴ����ʧ�ܣ�\n");
		return ;
	}
	HS->rear=HS->front;
	HS->front->next=NULL;
}
int IsEmpty(LinkQueue *HS)
{
	return HS->front==HS->rear;
}
void Print(LinkQueue *l){ //Print����û�����ź�������
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
		printf("�ڴ����ʧ�ܣ�\n");
		return ;// �˴�����ǰ��û���ͷ�mutex
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
		return 0;// �˴�����ǰ��û���ͷ�mutex
	}
	Print(HS);
	first=HS->front->next;
	temp=first->data;
	HS->front->next=first->next;
	if(HS->rear=first)
		HS->rear=HS->front;
	delete first;
	Print(HS);
	return temp; //�˴����أ�û���ͷ�mutex
		ReleaseMutex(hMutex);
}
QueueNode* Find(int data,LinkQueue *l){
	WaitForSingleObject(rMutex,-1);
	if(readcount==0)
		WaitForSingleObject(hMutex,-1);
	readcount++;
	ReleaseMutex(rMutex) ;   //�ͷŻ����ź�
	QueueNode *p;
	p=l->front->next;
	WaitForSingleObject(rMutex,-1) ;  //����λ�ò���ȷ�����һ�ȱ��һЩ
		readcount-- ;//������Ŀ����
	while(p){
		if(p->data==data){return p;}// �˴�����ǰ��û���ͷ�mutex
			p=p->next;
	}
	return NULL;
}
void Clear(LinkQueue *HS)  //û��mutex����
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
		cout<<"��� : "<<j<<endl;
		EnQueue(pQ,j);
		Sleep(100);
	}
	return true;        
}
DWORD WINAPI Fun2Proc(LPVOID lpParameter)
{   
	for(j=Queue_NUM;j>0;j--){
		cout<<"����j : "<<j<<endl;
		DeQueue(pQ);
		Sleep(100);
	}
	return true;
}
DWORD WINAPI Fun3Proc(LPVOID lpParameter)
{
	cout<<"���Ҷ���";
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
	printf("1.�Զ�����");
	for(i=0;i<16;i++) printf(" ");printf("* "); printf("\n");
	for(i=0;i<10;i++) printf(" ");printf("*  ");
	printf("2.�Զ�����");
	for(i=0;i<16;i++) printf(" ");printf("* "); printf("\n");
	for(i=0;i<10;i++) printf(" ");printf("*  ");
	printf("3.ѡ�������Ŀ");
	for(i=0;i<16;i++) printf(" ");printf("* "); printf("\n");
	for(i=0;i<10;i++) printf(" ");printf("*  ");
	printf("4.ѡ�������Ŀ");
	for(i=0;i<16;i++) printf(" ");printf("* "); printf("\n");
	for(i=0;i<10;i++) printf(" ");printf("*  ");
	printf("5.�����¿�");
	for(i=0;i<16;i++) printf(" ");printf("* "); printf("\n");
	for(i=0;i<10;i++) printf(" ");printf("*  ");
	printf("6.��ӡ����");
	for(i=0;i<16;i++) printf(" ");printf("* "); printf("\n");
	for(i=0;i<10;i++) printf(" ");printf("*  ");
	printf("7.���в���");
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
		printf("��ѡ��1--2,3����Ϊ�ֶ�ִ�У�\n ");scanf("%d",&m);
		switch(m)
		{
		case 0: return;
		case 1:{
			WaitForSingleObject(hMutex,INFINITE);
		//	�����߳�ǰ����mutex������Ϊʲô��
				cout<<"�����Զ�ִ����ӣ�"<<endl;
			hThread1=CreateThread(NULL,0,Fun1Proc,NULL,0,NULL);
			//     cout<<"��ѡ��1--2,3����Ϊ�ֶ�ִ�У�"<<endl;
			ReleaseMutex(hMutex);
			//     ReleaseMutex(pMutex);//////
			break;
			   }
		case 2:{
			WaitForSingleObject(hMutex,INFINITE);
			cout<<"�����Զ�ִ�г��ӣ�"<<endl;
			hThread2=CreateThread(NULL,0,Fun2Proc,NULL,0,NULL);
			ReleaseMutex(hMutex);
			break;
			   }
		case 3:{
			cout<<"����������Ҫ��ӵĴ�С��";
			cin>>j;
			for(int i=1;i<=j;i++)
			{cout<<"the "<<i<<" number:"<<endl;
			cin>>e;
			EnQueue (pQ,e);
			}
			break;
			   }
		case 4:{
			cout<<"��������Ҫ���ӵ���Ŀ��";
			cin>>Queue_NUM;
			for(int i=1;i<=Queue_NUM;i++)
			{DeQueue (pQ);}
			cout<<endl;
			break;
			   }
		case 5:
			{
				printf("�����¿գ�\n");
				Clear(pQ);
				printf("\n");
				break;
			}
		case 6:
			{
				printf("������У�\n");
				Print(pQ);//  ǰ���Print����û��mutex����
					printf("\n");
				break;
			}
		case 7:
			{
				WaitForSingleObject(hMutex,INFINITE);
				printf("���ң�\n");
				int i;
				cin>>i;
				cout<<Find(i,pQ);
				hThread3=CreateThread(NULL,0,Fun3Proc,NULL,0,NULL);
				printf("\n");
				ReleaseMutex(hMutex);
				break;
			}
		default: return; // retrun�����ֱ���˳��ˣ����á�
		}
	}
	CloseHandle(hThread1);
	CloseHandle(hThread2);
	CloseHandle(hThread3);
	WaitForSingleObject(hMutex,INFINITE); ��һ��д��������
		ReleaseMutex(hMutex); //��ЩҲ�ǡ�
		ReleaseMutex(hMutex);
	ReleaseMutex(hMutex);
}

