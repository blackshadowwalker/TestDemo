
#include <stdio.h>  
#include <iostream>  
using namespace std;  
#define M 105  
struct node  
{  
    double x, y;  
}p[M];  

int inpolygon(node t, int n)  
{  
    int i, t1, t2, sum, f;  
    for(i = 0; i <= n; i ++) 
	{
		p[i].x -= t.x, p[i].y -= t.y; 
	}
    t1 = p[0].x>=0 ?(p[0].y>=0?0:3) :(p[0].y>=0?1:2);    
    for(sum = 0, i = 1; i <= n; i ++)  
    {  
        if(!p[i].x && !p[i].y) break;              
        f = p[i].y * p[i-1].x - p[i].x * p[i-1].y;    
        if(!f && p[i-1].x*p[i].x <= 0 && p[i-1].y*p[i].y <= 0) break;    
        t2 = p[i].x>=0 ?(p[i].y>=0?0:3) :(p[i].y>=0?1:2);    
        if(t2 ==(t1 + 1) % 4) sum += 1;          
        else if(t2 ==(t1 + 3) % 4) sum -= 1;    
        else if(t2 ==(t1 + 2) % 4)                
        {                                              
            if(f > 0) sum += 2;  
            else sum -= 2;  
        }  
        t1 = t2;  
    }  
    if(i<=n || sum) return 1;  
    return 0;  
}  
int main()  
{  
    int n, i;  
    node t;  
    while(scanf("%d", &n) && n)  
    {  
        for(i = 0; i < n; i ++)  
            scanf("%lf,%lf;", &p[i].x, &p[i].y);  
        p[n] = p[0];  
        scanf("%lf,%lf", &t.x, &t.y);  
        if(inpolygon(t, n)) printf("inside\n");  
        else printf("outside\n");  
    }  
    return 0;  
} 

