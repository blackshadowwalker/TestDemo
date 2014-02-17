#include "stdio.h"
#include "stdlib.h"
#define cs(var) #var

void func()
{
	printf("\n--------------------debug ----------------- \n");
} 

int main(int argn , char **argc)
{
#if (1) 
#pragma message ("NOTE:WINVER has been defined as 0x0500 or greater.")	
#endif 

	bool debug=false; 
//	putenv("DBG=true");
	if (getenv("DBG"))
		debug=true;
	if (debug)
		printf("calling func\n");
	func(); 
	if (debug)
		printf("returned from func\n");
	char *path;
	path = getenv( "path" );
	printf("path=%s\n", path);
	getchar();
	getchar();
	getchar();
}