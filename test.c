#include <stdio.h>
#define KEY 17
#include <string.h>

char *chyper = "qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0";

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

int main() {
    char str[1000];
    while(1) {
        scanf("%s",str);
        encode(str);
        printf("%s\n",str);
    }
    return 0;
}