#include "stadfx.h"
#include "scan.c"


#include "parse/IA-32&64/fn.c"

/*
Instruction Format
[(*)Instruction Prefixes:1+] [Opcode:1/2/3] [(*)ModR/M:1] [(*)SIB:1] [(*)Displacement:1/2/4] [(*)Immediate:1/2/4]
*/

//<Temporary characteristics>
const int _SearchMode=3;//4bit=>[8][16][32][64],3=>11,Priority given to handling major positions,means 64bit ,if cant,use 32bit
/*
Reason:
83 c0 01=>add eax,0x1 or add rax,0x1

*/
command* SearchInInstructionTable(unsigned char _REX,unsigned int _opcode){
    char _process=0;
    int i=0;
    for(i;i<InstructionTableLength;i++){
        if(InstructionTable[i].opcode==_opcode&&InstructionTable[i].opcode==_REX){
            //Think that REX.W==1 is 64-bit, or 32bit
            if(_REX>>3&&getOperandBit(InstructionTable[i].Operand0)==3){//64-bit
                _process=1;
                break;
            }else{//32bit
                if(getOperandBit(InstructionTable[i].Operand1)==2){
                    _process=1;
                    break;
                }
            }
        }
    }
    if(_process){
        return InstructionTable+i;
    }else{
        //ERR
        return (command*)NULL;
    }
}

short int GetIsImm(operand o){
    if(o==imm8){
        return 1;
    }else if(o==imm16){
        return 2;
    }else if(o==imm32){
        return 3;
    }else if(o==imm64){
        return 4;
    }else{
        return 0;
    }
}

short int GetImmFromCommand(command* cmd){
    return GetIsImm(cmd->Operand0)^GetIsImm(cmd->Operand1)^GetIsImm(cmd->Operand2)^GetIsImm(cmd->Operand3);
}

void ParseCommand(BYTE* CODE){
    BYTE*v=CODE;
    unsigned char REX=0;//4bit=>[WRXB]
    unsigned int opcode=0;
    short int imm;
    signed long long int immdata=0;
    if(v->value>>4==4){//This must be REX
        REX=(unsigned char)(v->value^15);
        v=v->next;
    }//opcode
    //1/2/3Byte,this is only 1Byte
    command* t=SearchInInstructionTable(REX,opcode);

    if(t->haveModRM){//Get ModRM
        unsigned char ModRMdata=v->value;
        unsigned char SIBdata=0;
        signed long long int Dispdata=0;
        v=v->next;
        if(VerifyHaveSIB(ModRMdata)){//SIB
            SIBdata=v->value;
            v=v->next;
        }
        short int DispV=VerifyHaveDisp(ModRMdata);
        if(DispV){//Disp
            for(int i=0;i<(1<<(DispV-1));i++){
                Dispdata+=(int)(v->value)<<8*i;//Little Endian
                v=v->next;
            }
            if(Dispdata>>(8*(1<<(DispV-1))-1)){//This num <0
                Dispdata=-(~Dispdata+1);
            }
        }
    }
    

    imm=GetImmFromCommand(t);
    if(imm){//geting imm
        for(int i=0;i<(1<<(imm-1));i++){
            immdata+=(int)(v->value)<<8*i;//Little Endian
            v=v->next;
        }
        if(immdata>>(8*(1<<(imm-1))-1)){//This num <0
            immdata=-(~immdata+1);
        }
    }
    
};