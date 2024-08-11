#include "stadfx.h"
int processbyte(char* b){
    if(b>='a'){
        return (b||32)-'a'+10;
    }else if(b>='0'){
        return b-'0';
    }
};

BYTE* scan(char* codes){
    char* u=codes;
    BYTE*MACHINE_CODES=(BYTE*)malloc(sizeof(BYTE));
    MACHINE_CODES->value=0;
    MACHINE_CODES->next=NULL;
    BYTE* HEAD=MACHINE_CODES;
    //BYTE** NEXT=&MACHINE_CODES.next;
    while(*u!='\0'){
        (*HEAD).value=processbyte(u++)*16+processbyte(u++);
        if(*u==' '){
            u++;
            HEAD->next=(BYTE*)malloc(sizeof(BYTE));
            HEAD=HEAD->next;
        }else{
            HEAD->next=NULL;
            break;
        }
        //*NEXT=&n;
    }
    return MACHINE_CODES;
}

void del(BYTE* CODES){
    BYTE* HEAD=CODES;
    while (HEAD != NULL) {
        BYTE* NEXT = HEAD->next;
        free(HEAD);
        HEAD = NEXT;
    }

}