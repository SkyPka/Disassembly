//by 325383-sdm-vol-2abcd.pdf

typedef struct _Standard_RegCode{
    char* b;
    char* w;
    char* d;
    char* q;//64bit only
    int REX_B:4;//[1101]=>b=>true,w=>true,d=>false,q=>true
    int id;
} RegCode;



struct _RegTable{
    RegCode RC0;
    RegCode RC1;
    RegCode RC2;
    RegCode RC3;
    RegCode RC4;
    RegCode RC5;
    RegCode RC6;
    RegCode RC7;
    RegCode RC8;
    RegCode RC9;
    RegCode RC10;
    RegCode RC11;
    RegCode RC12;
    RegCode RC13;
    RegCode RC14;
    RegCode RC15;
    RegCode RC16;
    RegCode RC17;
    RegCode RC18;
    RegCode RC19;
    RegCode RC20;
} RegTable={//from Table 3-1. Register Codes Associated With +rb, +rw, +rd, +ro
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

char EffectiveAddress16Bit_ModRM[32][19]={//$H=disp16,$O=disp8
    "[BX+SI]",
    "[BX+DI]",
    "[BP+SI]",
    "[BP+DI]",
    "[SI]",
    "[DI]",
    "$H",
    "[BX]",
    "[BX+SI]+$O",
    "[BX+DI]+$O",
    "[BP+SI]+$O",
    "[BP+DI]+$O",
    "[SI]+$O",
    "[DI]+$O",
    "[BP]+$O",
    "[BX]+$O",
    "[BX+SI]+$H",
    "[BX+DI]+$H",
    "[BP+SI]+$H",
    "[BP+DI]+$H",
    "[SI]+$H",
    "[DI]+$H",
    "[BP]+$H",
    "[BX]+$H",
    "EAX/AX/AL/MM0/XMM0",
    "ECX/CX/CL/MM1/XMM1",
    "EDX/DX/DL/MM2/XMM2",
    "EBX/BX/BL/MM3/XMM3",
    "ESP/SP/AH/MM4/XMM4",
    "EBP/BP/CH/MM5/XMM5",
    "ESI/SI/DH/MM6/XMM6",
    "EDI/DI/BH/MM7/XMM7",
};//From Table 2-1. 16-Bit Addressing Forms with the ModR/M Byte
char REG[8][19]={
    "AL/AX/EAX/MM0/XMM0",//r8/r16/r32/mm/xmm
    "CL/CX/ECX/MM1/XMM1",
    "DL/DX/EDX/MM2/XMM2",
    "BL/BX/EBX/MM3/XMM3",
    "AH/SP/ESP/MM4/XMM4",
    "CH/BP/EBP/MM5/XMM5",
    "DH/SI/ESI/MM6/XMM6",
    "BH/DI/EDI/MM7/XMM7"
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
char EffectiveAddress32Bit_ModRM[32][19]={//$5=disp32(#2**5),$S=[--][--]=>SIB bytes
    "[EAX]",
    "[ECX]",
    "[EDX]",
    "[EBX]",
    "$S",
    "$5",
    "[ESI]",
    "[EDI]",
    "[EAX]+$O",
    "[ECX]+$O",
    "[EDX]+$O",
    "[EBX]+$O",
    "$S+$O",
    "[EBP]+$O",
    "[ESI]+$O",
    "[EDI]+$O",
    "[EAX]+$5",
    "[ECX]+$5",
    "[EDX]+$5",
    "[EBX]+$5",
    "$S+$5",
    "[EBP]+$5",
    "[ESI]+$5",
    "[EDI]+$5",
    "EAX/AX/AL/MM0/XMM0",
    "ECX/CX/CL/MM1/XMM1",
    "EDX/DX/DL/MM2/XMM2",
    "EBX/BX/BL/MM3/XMM3",
    "ESP/SP/AH/MM4/XMM4",
    "EBP/BP/CH/MM5/XMM5",
    "ESI/SI/DH/MM6/XMM6",
    "EDI/DI/BH/MM7/XMM7",
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