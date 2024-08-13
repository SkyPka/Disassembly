#include "../../stadfx.h"
#include "Data/Address.c"

//#include "Data/instruction.c"

char *getReigisterString(operand Reg){
    unsigned char bit=(Reg-OperandlistRegisterGroupIndex)%4;//0=>8bit,1=>16bit,2=>32bit,3=>64bit
    if(bit>>1){
        if(bit&1){//64bit
            return &(RegTable[(Reg-OperandlistRegisterGroupIndex-bit)/4].q);
        }else{//32bit
            return &(RegTable[(Reg-OperandlistRegisterGroupIndex-bit)/4].d);
        }
    }else{
        if(bit&1){//16bit
            return &(RegTable[(Reg-OperandlistRegisterGroupIndex-bit)/4].w);
        }else{//8bit
            return &(RegTable[(Reg-OperandlistRegisterGroupIndex-bit)/4].b);
        }
    }
}

char* getRM_bySIB(unsigned char SIB){//SIB=[SS:2][Index:3][Base:3]=>2Byte[00000000]
    unsigned char _SS=SIB>>6;
    unsigned char _Index=(SIB>>3)&7;
    unsigned char _Base=SIB&7;
    char* answer;
    if(_Base==5){
        //Manual P37 Note 1
    }
    if(SIB&&_Index!=4){//eg:[eax+ecx*2]
        answer=(char*)malloc(10*sizeof(char));
        //*answer=AF32bit_SIB[_Index][0];//[

        *answer=Base[_Base][0];
        *(answer+1)=Base[_Base][1];
        *(answer+2)=Base[_Base][2];
        *(answer+3)='+';

        *(answer+4)=AF32bit_SIB[_Index][1];
        *(answer+5)=AF32bit_SIB[_Index][2];
        *(answer+6)=AF32bit_SIB[_Index][3];
        *(answer+7)='*';
        *(answer+8)=(char)(1<<SIB+'0');
        //*(answer+9)=AF32bit_SIB[_Index][4];//]
        *(answer+9)='\0';
    }else if(_Index==4){
        //err
    }else{//eg:eax+ecx
        answer=(char*)malloc(8*sizeof(char));
        //*answer=AF32bit_SIB[_Index][0];//[

        *answer=Base[_Base][0];
        *(answer+1)=Base[_Base][1];
        *(answer+2)=Base[_Base][2];
        *(answer+3)='+';

        *(answer+4)=AF32bit_SIB[_Index][1];
        *(answer+5)=AF32bit_SIB[_Index][2];
        *(answer+6)=AF32bit_SIB[_Index][3];
        //*(answer+8)=AF32bit_SIB[_Index][4];//]
        *(answer+7)='\0';

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
unsigned short int VerifyHaveDisp(unsigned char ModRM){//[0,1,2,3]=>[Null,Disp8,Disp16,Disp32]
    unsigned char _Mod=ModRM>>6;
    unsigned char _RM=ModRM&7;
    char* p=EffectiveAddress32Bit_ModRM[8*_Mod+_RM].attr;
    if(*p=='$'){
        p++;
        while(*p!='\0'){
            if(*p=='H'){
                return 2;
            }else if(*p=='O'){
                return 1;
            }else if(*p=='5'){
                return 3;
            }
            p++;
        }
    }
    return 0;
}
unsigned short int VerifyHaveSIB(unsigned char ModRM){
    unsigned char _Mod=ModRM>>6;
    unsigned char _RM=ModRM&7;
    char* p=EffectiveAddress32Bit_ModRM[8*_Mod+_RM].attr;
    if(*p=='$'){
        p++;
        while(*p!='\0'){
            if(*p=='S'){
                return 1;
            }
            p++;
        }
    }
    return 0;
}

typedef struct{
    char* RM;
    operand type;//rm8,rm16....
    _RegisterGroup* RegGroup;//{r8,r16,r32,mm,xmm}
    //unsigned int haveSIB:1;
    //unsigned int havedisp:1;
} ModRMFnRet;
#define MAX_RMString_LENGTH 40
ModRMFnRet getRM_byModRM(unsigned char ModRM,unsigned char SIB,long long int Disp,unsigned short int BIT){//Bit=>[0,1,2,3]=>[8,16,32,64]//support 16bit,32bit
    unsigned char _Mod=ModRM>>6;
    unsigned char _RM=ModRM&7;
    unsigned char _REG=(ModRM>>3)&7;
    ModRMEleument*modrme;
    operand _type=NULL;
    if(BIT==1){
        _type=rm16;
        modrme=&EffectiveAddress16Bit_ModRM[8*_Mod+_RM];
    }else if(BIT==2){
        _type=rm32;
        modrme=&EffectiveAddress32Bit_ModRM[8*_Mod+_RM];
    }else{
        //NotSupportError
    }
    //analyse ModRMEleument
    char* RMString;
    if(modrme->attr[0]=='#'){//only support get r* Reg
        if(BIT>>1){
            if(BIT&1){//64BIT
                //NotSupportError
            }else{//32BIT
                RMString=RegisterGroup[modrme->attr[1]-'0'].r32;
            }
        }else{
            if(BIT&1){//16BIT
                RMString=RegisterGroup[modrme->attr[1]-'0'].r16;
            }else{//8BIT
                //NotSupportError
            }
        }
    }else{
        RMString=(char*)malloc(MAX_RMString_LENGTH*sizeof(char));
        *RMString='[';
        *(RMString+1)='\0';
        char _yTool='+';
        unsigned short int num=0;
        if(modrme->Operand0>=OperandlistRegisterGroupIndex){
            strcat(RMString,getReigisterString(modrme->Operand0));
            num++;
            //strcat(RMString,&_yTool);
        }
        if(modrme->Operand1>=OperandlistRegisterGroupIndex){
            if(num){strcat(RMString,&_yTool);}
            strcat(RMString,getReigisterString(modrme->Operand1));
            num++;
        }
        if(modrme->attr[0]=='$'){
            unsigned int p=1;
            while(modrme->attr[p]!='\0'){
                if(modrme->attr[p]=='O'||modrme->attr[p]=='H'||modrme->attr[p]=='5'){//Must be the last block in RMString
                    char* DispString=(char*)malloc(MAX_RMString_LENGTH*sizeof(char));
                    Extend_itoa(Disp,DispString,16);//radix 16
                    strcat(RMString,DispString);
                    free(DispString);
                //}else if(modrme->attr[p]=='H'){//Disp16

                //}else if(modrme->attr[p]=='5'){//Disp32

                }else if(modrme->attr[p]=='S'){
                    char*SIBdata=getRM_bySIB(SIB);
                    if(num){strcat(RMString,&_yTool);}
                    num++;
                    strcat(RMString,SIBdata);
                    free(SIBdata);
                    num++;
                }
                p++;
            }
        }
    }
    ModRMFnRet RET={RMString,_type,&RegisterGroup[_REG]};
    return RET;
}

unsigned char getOperandBit(operand _operand){
    return (unsigned char)((_operand+3)%4);//[0,1,2,3]=>[8-bit,16-bit,32-bit,64-bit]
}