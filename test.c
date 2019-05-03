#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <sys/types.h>
#include <dirent.h>
#define KEY 17

char *chyper = "qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0";


int find_ext(char *str) {
	int len = strlen(str);
	for(int i=len-1; i>=0; i--) {
		if (str[i] == '.') return i;
	}
	return 0;
}


void strfindext(char *dest, char *src, char *ext) {
     
    int len = strlen(src);
    int a = find_ext(src);
    for(int i=1, j=0; i<a; i++, j++) {
        dest[j] = src[i];
    }
    for(int i=a+1, j=0; i<len; i++,j++) {
        ext[j] = src[i];
    }
}

void encode(char *str) {
    if (!strcmp(str, ".")) return;
    if (!strcmp(str, "..")) return;
    
    int len = strlen(str), i, j;
    for(i=0; i<len; i++) 
    {
        char tmp = str[i];
        for(j=0; j<94; j++) {
            str[i] = (tmp == chyper[j]? chyper[(j+KEY)%94] : str[i]);
        }
    }
}

void decode(char *str) {
    if (!strcmp(str, ".")) return;
    if (!strcmp(str, "..")) return;
    
    int len = strlen(str), i, j;
    for(i=0; i<len; i++)
    {
        char tmp = str[i];
        for(j=0; j<94; j++) {
            str[i] = (tmp == chyper[j]? chyper[(j+94-KEY)%94] : str[i]);
        }
    }
}

int main()
{
    char x[1000];
    scanf("%s",x);
    decode(x);
    printf("%s\n",x);
}