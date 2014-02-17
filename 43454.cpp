#include "stdio.h"

int main()
{
	const unsigned short code = 43454;
	char num[10] = {0};
	num[0] = (code & 0x000000FF);
	num[1] = (code & 0x0000FF00) >> 8;
//	sprintf(, "%d", code);
	printf("%s\0\n", num );
}