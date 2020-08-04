#include <stdio.h>
#include <math.h>
#include <stdlib.h>
char * int2Char(int src){
	int lenSrc = 0;
    int i;
	//길이재기
    int cpIn = src;
	for(i = 10; cpIn > 0;){
        cpIn = (int)cpIn/i;
		lenSrc++;
	}
	//동적할ㅇ당하기
	char * out = (char *) malloc((lenSrc+1)*sizeof(char));
	int j;
	//ascii로 변환
	for(j = 0; lenSrc > j; j++ ){
		out[j] = (src/(int)pow(10, lenSrc-1-j))+48;
		src = src%((int)pow(10, lenSrc-1-j));
	}
	out[lenSrc] = NULL;
	return out;
}
int main(){
    printf(int2Char(1));
    printf("\n");
    printf(int2Char(324340));
    printf("\n");
    printf(int2Char(2349));
    printf("\n");
    return 0;
} 