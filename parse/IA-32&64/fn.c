#include "../../stadfx.h"
#include "Data/Address.c"


char* getRM_bySIB(unsigned char SIB){//SIB=[SS:2][Index:3][Base:3]=>2Byte[00000000]
    unsigned char _SS=SIB>>6;
    unsigned char _Index=(SIB>>3)&7;
    unsigned char _Base=SIB&7;
    char* answer;
    if(_Base==5){
        //Manual P37 Note 1
    }
    if(SIB&&_Index!=4){//eg:[eax+ecx*2]
        answer=(char*)malloc((4+8)*sizeof(char));
        *answer=AF32bit_SIB[_Index][0];//[

        *(answer+1)=Base[_Base][0];
        *(answer+2)=Base[_Base][1];
        *(answer+3)=Base[_Base][2];
        *(answer+4)='+';

        *(answer+5)=AF32bit_SIB[_Index][1];
        *(answer+6)=AF32bit_SIB[_Index][2];
        *(answer+7)=AF32bit_SIB[_Index][3];
        *(answer+8)='*';
        *(answer+9)=(char)(1<<SIB+'0');
        *(answer+10)=AF32bit_SIB[_Index][4];//]
        *(answer+11)='\0';
    }else if(_Index==4){
        //err
    }else{//eg:[eax+ecx]
        answer=(char*)malloc((4+6)*sizeof(char));
        *answer=AF32bit_SIB[_Index][0];//[

        *(answer+1)=Base[_Base][0];
        *(answer+2)=Base[_Base][1];
        *(answer+3)=Base[_Base][2];
        *(answer+4)='+';

        *(answer+5)=AF32bit_SIB[_Index][1];
        *(answer+6)=AF32bit_SIB[_Index][2];
        *(answer+7)=AF32bit_SIB[_Index][3];
        *(answer+8)=AF32bit_SIB[_Index][4];//]
        *(answer+9)='\0';

    }
    return answer;
}