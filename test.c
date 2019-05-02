#include <stdio.h>
#include <string.h>
#include <libgen.h>

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

int main()
{
    char x[1000], new[1000], ext[1000];
    while(1) {
        scanf("%s",x);
        char *dir = dirname(x);
        char *p = strrchr(dir, '/');
        printf("%s %s\n",dir, p);
    }
}