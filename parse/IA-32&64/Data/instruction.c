

/*
Instruction Format
[(*)Instruction Prefixes:1+] [Opcode:1/2/3] [(*)ModR/M:1] [(*)SIB:1] [(*)Displacement:1/2/4] [(*)Immediate:1/2/4]

//66H,F2H,F3H,VEX,REX.... Prefixes
*/
const int OperandlistRegisterGroupIndex=29;
typedef enum _o{//from 3.1.1.3 Instruction Column in the Opcode Summary Table
    NULL=0,
    r8,r16,r32,r64,
    imm8,imm16,imm32,imm64,
    rm8,/*r/m8*/rm16,rm32,rm64,
    //m,
    m8,m16,m32,m64,
    //m128,//idq
    moffs8,moffs16,moffs32,moffs64,
    //mm,
    $null0,
    $null1,
    mm32,//mm/m32
    mm64,//mm/m64
    //xmm,
    $null2,
    $null3,
    xmm32,//xmm/m32
    xmm64,//xmm/m64


    AL,AX,EAX,RAX,//from Address.c/RegTable
    CL,CX,ECX,RCX,
    DL,DX,EDX,RDX,
    BL,BX,EBX,RBX,
    AH,SP,ESP,$null4,
    CH,BP,EBP,$null5,
    DH,SI,ESI,$null6,
    BH,DI,EDI,$null7,
    SPL,SP,ESP,RSP,
    BPL,BP,EBP,RBP,
    SIL,SI,ESI,RSI,
    DIL,DI,EDI,RDI,
    //64 bit only
    R8B,R8W,R8D,R8,
    R9B,R9W,R9D,R9,
    R10B,R10W,R10D,R10,
    R11B,R11W,R11D,R11,
    R12B,R12W,R12D,R12,
    R13B,R13W,R13D,R13,
    R14B,R14W,R14D,R14,
    R15B,R15W,R15D,R15,
} operand;

typedef struct _command{//command for each 
    unsigned char REX:4;//[WRXB]=>real REX:[0100WRXB]//Table 2-4. REX Prefix Fields [BITS: 0100WRXB]
    //VEX;
    //rel8,rel16,rel32
    //ptr16:16, ptr16:32 
    int haveModRM:1;
    int ModRM_REG:4;//need 3  Byte to use,if (ModRM_REG>>3)>0 this value will be ignored.
    //int haveSIB:1;
    unsigned int opcode:3*8;//3Byte=>MAX_Instruction_Opcode_Byte=3Byte:opcode=[0][1][2] or 0[0][1] or 00[0]
    //int haveimm:1;
    //int imm:2;//[0,1,2,3]=>2**[3,4,5,6]=>8,16,32,64=>ib,iw,id,iq
    char name[21];//The longest may be the SGX Instructions < ENCLV[EDECVIRTCHILD] >length:20
    operand Operand0;
    operand Operand1;
    operand Operand2;
    operand Operand3;
} command;

/*
typedef struct _instruct{
    unsigned short int commandnum;
    command* TheFirstCommand;
} instruction;
*/

const int InstructionTableLength=19;
command InstructionTable[]={
    //ADD
    {0,0,0,0x04,"ADD",AL,imm8,0,0},//04 ib ADD AL, imm8 I Valid Valid Add imm8 to AL.
    {0,0,0,0x05,"ADD",AX,imm16,0,0},//05 iw ADD AX, imm16 I Valid Valid Add imm16 to AX.
    {0,0,0,0x05,"ADD",EAX,imm32,0,0},//05 id ADD EAX, imm32 I Valid Valid Add imm32 to EAX.
    {8,0,0,0x05,"ADD",RAX,imm32,0,0},//REX.W + 05 id ADD RAX, imm32 I Valid N.E. Add imm32 sign-extended to 64-bits to RAX.
    {0,1,0,0x80,"ADD",rm8,imm8,0,0},//80 /0 ib ADD r/m8, imm8 MI Valid Valid Add imm8 to r/m8.
    //{,,,,,,,0,0},//REX + 80 /0 ib ADD r/m8*, imm8 MI Valid N.E. Add sign-extended imm8 to r/m8.
    {0,1,0,0x81,"ADD",rm16,imm16,0,0},//81 /0 iw ADD r/m16, imm16 MI Valid Valid Add imm16 to r/m16.
    {0,1,0,0x81,"ADD",rm32,imm32,0,0},//81 /0 id ADD r/m32, imm32 MI Valid Valid Add imm32 to r/m32.
    {8,1,0,0x81,"ADD",rm64,imm32,0,0},//REX.W + 81 /0 id ADD r/m64, imm32 MI Valid N.E. Add imm32 sign-extended to 64-bits to r/m64.
    {0,1,0,0x83,"ADD",rm16,imm8,0,0},//83 /0 ib ADD r/m16, imm8 MI Valid Valid Add sign-extended imm8 to r/m16.
    {0,1,0,0x83,"ADD",rm32,imm8,0,0},//83 /0 ib ADD r/m32, imm8 MI Valid Valid Add sign-extended imm8 to r/m32.
    {8,1,0,0x83,"ADD",rm64,imm8,0,0},//REX.W + 83 /0 ib ADD r/m64, imm8 MI Valid N.E. Add sign-extended imm8 to r/m64.
    {0,1,8,0x00,"ADD",rm8,r8,0,0},//00 /r ADD r/m8, r8 MR Valid Valid Add r8 to r/m8.
    //{,,,,,,,0,0},//REX + 00 /r ADD r/m8*, r8* MR Valid N.E. Add r8 to r/m8.
    {0,1,8,0x01,"ADD",rm16,r16,0,0},//01 /r ADD r/m16, r16 MR Valid Valid Add r16 to r/m16.
    {0,1,8,0x01,"ADD",rm32,r32,0,0},//01 /r ADD r/m32, r32 MR Valid Valid Add r32 to r/m32.
    {8,1,8,0x01,"ADD",rm64,r64,0,0},//REX.W + 01 /r ADD r/m64, r64 MR Valid N.E. Add r64 to r/m64.
    {0,1,8,0x02,"ADD",r8,rm8,0,0},//02 /r ADD r8, r/m8 RM Valid Valid Add r/m8 to r8.
    //{,,,,,,,0,0},//REX + 02 /r ADD r8*, r/m8* RM Valid N.E. Add r/m8 to r8.
    {0,1,8,0x03,"ADD",r16,rm16,0,0},//03 /r ADD r16, r/m16 RM Valid Valid Add r/m16 to r16.
    {0,1,8,0x03,"ADD",r32,rm32,0,0},//03 /r ADD r32, r/m32 RM Valid Valid Add r/m32 to r32.
    {8,1,8,0x03,"ADD",r64,rm64,0,0},//REX.W + 03 /r ADD r64, r/m64 RM Valid N.E. Add r/m64 to r64
};