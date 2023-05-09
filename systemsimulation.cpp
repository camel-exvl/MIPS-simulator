#include"systemsimulation.h"

using namespace std;
int textline=0;//设置输入数据目前读取到第几行
//初始化寄存器组
void System::InitRegister()
{
    bitset<32> b(0);
    registers.at(0) = Register{ 0, "$zero", b };  // 插入$zero
    registers.at(1) = Register{ 1, "at", b };  // 插入$at
    for (int i = 2; i < 4; i++) {
        registers.at(i) = Register{ i, "$v" + to_string(i - 2), b };
        // 插入$v0-%v1
    }
    for (int i = 4; i < 8; i++) {
        registers.at(i) = Register{ i, "$a" + to_string(i - 4), b };
        // 插入$a0-%a3
    }
    for (int i = 8; i < 16; i++) {
        registers.at(i) = Register{ i, "$t" + to_string(i - 8), b };
        // 插入$t1-$t7
    }
    for (int i = 16; i < 24; i++) {
        registers.at(i) = Register{ i, "$s" + to_string(i - 16), b };
        // 插入$s1-$s7
    }
    for (int i = 24; i < 26; i++) {
        registers.at(i) = Register{ i, "$t" + to_string(i - 16), b };
        // 插入$t8-$t9
    }
    for (int i = 26; i < 28; i++) {
        registers.at(i) = Register{ i, "$k" + to_string(i - 26), b };
        // 插入$k0-%k1
    }
    registers.at(28) = Register{ 28, "$gp", (bitset <32>)0x10008000 };  // 插入$gp
    registers.at(29) = Register{ 29, "$sp", (bitset <32>)0x7ffffffc };  // 插入$sp
    registers.at(30) = Register{ 30, "$fp", (bitset <32>)0x7ffffffc };  // 插入$fp
    registers.at(31) = Register{ 31, "$ra", b };  // 插入$ra
}

void System::InitMemory()   
{
    
}

//根据编号找到寄存器并返回寄存器类
Register& System::FindRegister(int number)
{
    //遍历寄存器组
    for (auto& i : registers)
        if (i.number == number)
            return i;
    //若找不到则报错
    cerr << "寄存器不存在" << endl;
    exit(-1);
}

//根据名称找到寄存器并返回寄存器类
Register& System::FindRegister(const string& name)
{
    //遍历寄存器组
    for (auto& i : registers)
        if (i.name == name)
            return i;
    //若找不到则报错
    cerr << "寄存器不存在" << endl;
    exit(-1);
}

//访问内存，输入地址，返回内存地址内容
bitset<32>& System::AccessMemory(const bitset<32>& address)
{
    //保留区，禁止访问
    if (address.to_ulong() < 0x00400000)
    {
        cerr << "内存保留区域，不允许访问" << endl;
        exit(-1);
    }
    //代码段，返回机器指令
    else if (address.to_ulong() < 0x10000000)
    {
        if (address.to_ulong() < mem.texttop.to_ulong())//<=?//
            return mem.text_segment.at(address.to_ulong());
        else
        {
            cerr << "该地址没有代码存储，不允许访问" << endl;
            exit(-1);
        }
    }
    //数据段，返回存储数据
    else if (address.to_ulong() < registers.at(29).value.to_ulong())
    {
        return mem.data_segment.at(address.to_ulong());
    }
    //堆栈段，返回堆栈数据
    else if (address.to_ulong() < 0x7fffffff)
    {
        return mem.stack_segment[address.to_ulong()];
    }
    //地址越界报错
    else
    {
        cerr << "错误的内存地址，超出内存大小" << endl;
        exit(-1);
    }
}

//向内存代码段添加机器指令
void System::PushCodeToMemory(const bitset<32>& code)
{
    mem.text_segment[mem.texttop.to_ulong()] = code;
    mem.texttop = bitset<32>{ mem.texttop.to_ulong() + 0x04 };
    //若代码超出内存代码段报错
    if (mem.texttop.to_ulong() > 0x10000100)
    {
        cerr << "代码数量超过内存代码段大小" << endl;
        exit(-1);
    }
}

void System::PrintSystem() const
{
    //看后期要不要做图形界面
}

void System::InstructionRType(const string machineCode)
{
    int rs = stoi(machineCode.substr(6, 5), nullptr, 2);
    int rt = stoi(machineCode.substr(11, 5), nullptr, 2);
    int rd = stoi(machineCode.substr(16, 5), nullptr, 2);
    int shamt = stoi(machineCode.substr(21, 5), nullptr, 2);
    int funct = stoi(machineCode.substr(26, 6), nullptr, 2);
    if(funct== 0x20)
    Add();
    else if(funct== 0x22)
    Sub();
    else if(funct== 0x00)
    Sll();
    else if(funct== 0x24)
    And();
    else if(funct== 0x25)
    Or();
    else
    Jr();
}

void System::InstructionJType(const string machineCode)//J型指令只有J，所以直接判断即可
{
    int addr = stoi(machineCode.substr(6, 26), nullptr, 2);
    string str = PC.Getvalue().to_string();
    string first = str.substr(0, 4);
    string second = to_string(addr);
    string strPC = first+second+"00";
    bitset<32> bits(strPC);
    PC.Getvalue()=bits;
    //26位后面加00，前面取PC的前4位加起来组成32位   
}

inline static int complement2int(const bitset<16> complement) {
    return complement[15] ? -((~complement).to_ulong() + 1) : complement.to_ulong();
}

void System::InstructionIType(const string machineCode)
{
    int op = stoi(machineCode.substr(0, 6), nullptr, 2);
    int rs = stoi(machineCode.substr(6, 5), nullptr, 2);
    int rt = stoi(machineCode.substr(11, 5), nullptr, 2);
    bitset<16> immediate(machineCode.substr(16, 16));
    int imm = complement2int(immediate);
    if(op== 0x08)
    Addi();
    else if(op== 0x23)
    Lw();
    else if(op== 0x2B)
    Sw();
    else if(op== 0x04)
    Beq();
}

void System::JudgeInstruction(const bitset<32>& code)
{
    string machinecode=code.to_string();
    int op = code.to_ulong() >> 26;
    if (op == 0) //R型指令 ADD,SUB,SLL,AND,OR,JR
    {
        InstructionRType(machinecode);      
    } 
    else if (op >> 1 == 1 || op >> 2 == 0b100) //J型指令 J
    {
        InstructionJType(machinecode);  
    } 
    else //I型指令 ADDI LW SW BEQ
    {
        InstructionIType(machinecode);  
    }
}
