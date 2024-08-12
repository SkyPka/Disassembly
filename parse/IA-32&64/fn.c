#include "../../stadfx.h"
#include "Data/Address.c"
#include "Data/instruction.c"

char *getOperandString(){}

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
/*
Index from Eff...Add...[][]=24 ( 1 1  0 0 0 )
                                 Mod | RM
                Mod 11
                RM       000
                REG = 001
                -------------
/digit(Opcode)= C8H 11001000
*/
char* getRM_byModRM(unsigned char ModRM,operand Operand0,operand Operand1,operand Operand2,operand Operand3){
    unsigned char _Mod=ModRM>>6;
    unsigned char _RM=ModRM&7;
    unsigned char _REG=(ModRM>>3)&7;
    //EffectiveAddress32Bit_ModRM[8*_Mod+_RM]
}

unsigned char getOperandBit(operand _operand){
    return (unsigned char)((_operand+3)%4);//[0,1,2,3]=>[8-bit,16-bit,32-bit,64-bit]
}