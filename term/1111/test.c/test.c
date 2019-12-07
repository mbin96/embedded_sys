
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

char *int2Char(int src)
{
	int lenSrc = 0, i;
	for (i = 1; (src / i) > 0; i = i * 10)
		lenSrc++;
	char *out = (char *)malloc(lenSrc * sizeof(char));
	int j;
	for (j = 0; lenSrc > j; j++)
	{
		out[j] = src / pow(10, lenSrc - j - 1) + 48;
		src = src % ((int)pow(10, lenSrc - j -1));
	}
	return out;
}

int main(){
    printf(int2Char(10023));
    printf("\n");
}