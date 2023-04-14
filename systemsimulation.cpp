#include"systemsimulation.h"

using namespace std;

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
        if (address.to_ulong() < mem.texttop.to_ulong())
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