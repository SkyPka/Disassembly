#include "Instruction.c"
//by 325383-sdm-vol-2abcd.pdf

typedef struct _Standard_RegCode{
    char b[5];
    char w[5];
    char d[5];
    char q[6];//64bit only
    int REX_B:4;//[1101]=>b=>true,w=>true,d=>false,q=>true
    int id;
} RegCode;

typedef struct _Registers{
    char r8[3];
    char r16[3];
    char r32[4];
    char mm[4];
    char xmm[5];
} _RegisterGroup;
_RegisterGroup RegisterGroup[8]={
    {"AL","AX","EAX","MM0","XMM0"},
    {"CL","CX","ECX","MM1","XMM1"},
    {"DL","DX","EDX","MM2","XMM2"},
    {"BL","BX","EBX","MM3","XMM3"},
    {"AH","SP","ESP","MM4","XMM4"},
    {"CH","BP","EBP","MM5","XMM5"},
    {"DH","SI","ESI","MM6","XMM6"},
    {"BH","DI","EDI","MM7","XMM7"}
};

RegCode RegTable[20]={//from Table 3-1. Register Codes Associated With +rb, +rw, +rd, +ro
    {"AL","AX","EAX","RAX",0,0},
    {"CL","CX","ECX","RCX",0,1},
    {"DL","DX","EDX","RDX",0,2},
    {"BL","BX","EBX","RBX",0,3},
    {"AH","SP","ESP","$null",0,4},
    {"CH","BP","EBP","$null",0,5},
    {"DH","SI","ESI","$null",0,6},
    {"BH","DI","EDI","$null",0,7},
    {"SPL","SP","ESP","RSP",8,4},
    {"BPL","BP","EBP","RBP",8,5},
    {"SIL","SI","ESI","RSI",8,6},
    {"DIL","DI","EDI","RDI",8,7},
    //64 bit only
    {"R8B","R8W","R8D","R8",15,0},
    {"R9B","R9W","R9D","R9",15,1},
    {"R10B","R10W","R10D","R10",15,2},
    {"R11B","R11W","R11D","R11",15,3},
    {"R12B","R12W","R12D","R12",15,4},
    {"R13B","R13W","R13D","R13",15,5},
    {"R14B","R14W","R14D","R14",15,6},
    {"R15B","R15W","R15D","R15",15,7},
};

//ModRM

typedef struct _ModRMEleument{
    operand Operand0;
    operand Operand1;
    char attr[3];
} ModRMEleument;

ModRMEleument EffectiveAddress16Bit_ModRM[32]={//$H=disp16,$O=disp8,#0-7:RegisterGroup
    {BX,SI,0},//"[BX+SI]",
    {BX,DI,0},//"[BX+DI]",
    {BP,SI,0},//"[BP+SI]",
    {BP,DI,0},//"[BP+DI]",
    {SI,0,0},//"[SI]",
    {DI,0,0},//"[DI]",
    {0,0,"$H"},//"$H",
    {BX,0,0},//"[BX]",
    {BX,SI,"$O"},//"[BX+SI+$O]",
    {BX,DI,"$O"},//"[BX+DI+$O]",
    {BP,SI,"$O"},//"[BP+SI+$O]",
    {BP,DI,"$O"},//"[BP+DI+$O]",
    {SI,0,"$O"},//"[SI+$O]",
    {DI,0,"$O"},//"[DI+$O]",
    {BP,0,"$O"},//"[BP+$O]",
    {BX,0,"$O"},//"[BX+$O]",
    {BX,SI,"$H"},//"[BX+SI+$H]",
    {BX,DI,"$H"},//"[BX+DI+$H]",
    {BP,SI,"$H"},//"[BP+SI+$H]",
    {BP,DI,"$H"},//"[BP+DI+$H]",
    {SI,0,"$H"},//"[SI+$H]",
    {DI,0,"$H"},//"[DI+$H]",
    {BP,0,"$H"},//"[BP+$H]",
    {BX,0,"$H"},//"[BX+$H]",
    {0,0,"#0"},//"#0",
    {0,0,"#1"},//"#1",
    {0,0,"#2"},//"#2",
    {0,0,"#3"},//"#3",
    {0,0,"#4"},//"#4",
    {0,0,"#5"},//"#5",
    {0,0,"#6"},//"#6",
    {0,0,"#7"},//"#7",
};//From Table 2-1. 16-Bit Addressing Forms with the ModR/M Byte
char REG[8][5]={//use for search index
    "AL","AX","EAX","MM0","XMM0",
    "CL","CX","ECX","MM1","XMM1",
    "DL","DX","EDX","MM2","XMM2",
    "BL","BX","EBX","MM3","XMM3",
    "AH","SP","ESP","MM4","XMM4",
    "CH","BP","EBP","MM5","XMM5",
    "DH","SI","ESI","MM6","XMM6",
    "BH","DI","EDI","MM7","XMM7"
};
/*
Index from Eff...Add...[][]=24 ( 1 1  0 0 0 )
                                 Mod | RM
                Mod 11
                RM       000
                REG = 001
                -------------
/digit(Opcode)= C8H 11001000
*/
ModRMEleument EffectiveAddress32Bit_ModRM[32]={//$5=disp32(#2**5),$S=[--][--]=>SIB bytes
    {EAX,0,0},//"[EAX]",
    {ECX,0,0},//"[ECX]",
    {EDX,0,0},//"[EDX]",
    {EBX,0,0},//"[EBX]",
    {0,0,"$S"},//"$S",
    {0,0,"$5"},//"$5",
    {ESI,0,0},//"[ESI]",
    {EDI,0,0},//"[EDI]",
    {EAX,0,"$O"},//"[EAX+$O]",
    {ECX,0,"$O"},//"[ECX+$O]",
    {EDX,0,"$O"},//"[EDX+$O]",
    {EBX,0,"$O"},//"[EBX+$O]",//Change:from [EBX]+$O to [EBX+$O]
    {0,0,"$SO"},//"[$S+$O]",=>$SO import change//S should be front
    {EBP,0,"$O"},//"[EBP+$O]",
    {ESI,0,"$O"},//"[ESI+$O]",
    {EDI,0,"$O"},//"[EDI+$O]",
    {EAX,0,"$5"},//"[EAX+$5]",
    {ECX,0,"$5"},//"[ECX+$5]",
    {EDX,0,"$5"},//"[EDX+$5]",
    {EBX,0,"$5"},//"[EBX+$5]",
    {0,0,"$S5"},//"$S+$5",
    {EBP,0,"$5"},//"[EBP+$5]",
    {ESI,0,"$5"},//"[ESI+$5]",
    {EDI,0,"$5"},//"[EDI+$5]",
    {0,0,"#0"},//"#0",
    {0,0,"#1"},//"#1",
    {0,0,"#2"},//"#2",
    {0,0,"#3"},//"#3",
    {0,0,"#4"},//"#4",
    {0,0,"#5"},//"#5",
    {0,0,"#6"},//"#6",
    {0,0,"#7"},//"#7",
};//from Table 2-2. 32-Bit Addressing Forms with the ModR/M Byte

//SIB
char Base[8][4]={
    "EAX",
    "ECX",
    "EDX",
    "EBX",
    "ESP",
    "[*]",
    "ESI",
    "EDI"
};//from Table 2-3. 32-Bit Addressing Forms with the SIB Byte
char AF32bit_SIB[8][6]={//
    "[EAX]",
    "[ECX]",
    "[EDX]",
    "[EBX]",
    "$null",
    "[EBP]",
    "[ESI]",
    "[EDI]",
};