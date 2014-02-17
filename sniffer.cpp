#include<stdio.h> 
#include<winsock2.h> 
#include"mstcpip.h"
 
#pragma comment(lib,"ws2_32.lib") 
 
typedef struct iphdr 
{ 
    unsigned char ver_hlen;//4λIPv4�汾��4λͷ���ȣ���32λ�ֱ�ʾ�� 
    unsigned char tos;//�����IP���� 
    unsigned short total_len;//�ܳ� 
    unsigned short ident;//Ψһ��ʶ�� 
    unsigned short frag_and_flags;//Ƭ��ƫ���ֶ� 
    unsigned char ttl;//����ʱ�� 
    unsigned char proto;//Э�� 
    unsigned short checksum;//IPУ��� 
    unsigned int sourceIP;//Դ��ַ 
    unsigned int destIP;//Ŀ�ĵ�ַ 
 
}IPHeader; 
 
#define PACKAGE_SIZE  sizeof(IPHeader) 
#define xmalloc(s)    HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,(s)) 
#define xfree(p)      HeapFree(GetProcessHeap(),0,(p)) 
 
BOOL WINAPI CtrlHandler(DWORD dwEvent); 
void HandleError(char *); 
 
BOOL g_bExit = FALSE; 
 
int main() 
{ 
	char localAdd[256] = {"10.168.1.110"};
    if(!SetConsoleCtrlHandler(CtrlHandler,TRUE)) 
    { 
        printf("SetConsoleCtrlHandler:%d\n",GetLastError()); 
        return -1; 
    } 
 
    WSADATA wsaData; 
    WSAStartup(MAKEWORD(2,2), &wsaData); 
    SOCKET sock = socket(AF_INET,SOCK_RAW,IPPROTO_IP); 
    if(INVALID_SOCKET==sock) 
    { 
        HandleError("socket"); 
        closesocket(sock); 
        WSACleanup(); 
        return -1; 
    } 
    SOCKADDR_IN addr; 
    memset(&addr,0,sizeof(SOCKADDR_IN)); 
    addr.sin_family=AF_INET; 
    addr.sin_addr.s_addr=inet_addr(localAdd); 
    if(SOCKET_ERROR==bind(sock,(SOCKADDR*)&addr,sizeof(addr))) 
    { 
        HandleError("bind"); 
        closesocket(sock); 
        WSACleanup(); 
        return -1; 
    } 
     
    int on = RCVALL_ON; 
    DWORD num; 
    if(SOCKET_ERROR==WSAIoctl(sock,SIO_RCVALL,&on,sizeof(on),NULL,0,&num,NULL,NULL)) 
    { 
        HandleError("WSAIoctl"); 
        closesocket(sock); 
        WSACleanup(); 
        return -1; 
    } 
 
    char *buf = (char*)xmalloc(PACKAGE_SIZE); 
    SOCKADDR_IN from; 
    int fromlen; 
    while(!g_bExit) 
    { 
        memset(buf,0,PACKAGE_SIZE); 
        memset(&from,0,sizeof(from)); 
        fromlen=sizeof(from); 
        if(SOCKET_ERROR==recvfrom(sock,buf,PACKAGE_SIZE,0,(SOCKADDR*)&from,&fromlen)) 
        { 
            if(WSAEMSGSIZE!=WSAGetLastError()) 
            { 
                HandleError("recvfrom"); 
                break; 
            } 
        } 
        switch(((IPHeader*)buf)->proto) 
        { 
        case IPPROTO_ICMP: 
            printf("ICMP From %s\n",inet_ntoa(from.sin_addr)); 
            break; 
        case IPPROTO_IGMP: 
            printf("IGMP From %s\n",inet_ntoa(from.sin_addr)); 
            break; 
        case IPPROTO_TCP: 
            printf("TCP From %s\n",inet_ntoa(from.sin_addr)); 
            break; 
        case IPPROTO_UDP: 
            printf("UPD From %s\n",inet_ntoa(from.sin_addr)); 
            break; 
        default: 
            printf("Unknown datagram From %s\n",inet_ntoa(from.sin_addr)); 
        } 
    } 
 
    xfree(buf); 
    closesocket(sock); 
    WSACleanup(); 
    printf("Stopped!\n"); 
 
    system("pause"); 
    return 0; 
} 
 
BOOL WINAPI CtrlHandler(DWORD dwEvent) 
{ 
    switch(dwEvent) 
    { 
    case CTRL_C_EVENT: 
    case CTRL_LOGOFF_EVENT: 
    case CTRL_SHUTDOWN_EVENT: 
    case CTRL_CLOSE_EVENT: 
        printf("Stopping......\n"); 
        break; 
    default: 
        return FALSE; 
    } 
    return TRUE; 
} 
 
void HandleError(char * func) 
{ 
    int error = WSAGetLastError(); 
    char info[65] = {0}; 
    _snprintf(info, 64, "%s:  %d\n",func,error); 
    printf(info); 
} 




