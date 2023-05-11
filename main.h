#pragma once
#include <iostream>
#include <algorithm>
#include <cstring>
#include <bitset>
#include <vector>
#include <map>
#include <cstdlib>
#include <string>
#include <utility>

//Register class
//构造函数: 0 参数 或 3 参数
//***********类外部接口*****************
//无
//**************错误********************
//无
class Register
{
private:
    int number;//编号
    std::string name;//名称
    std::bitset<32> value;//存储内容

public:
    //默认构造函数
    Register() : number{ -1 } {}

    //含参构造函数，接受左值
    Register(int number, const std::string& name, const std::bitset<32>& value) : number{ number }, name{ name }, value{ value } {}

    //含参构造函数，接受右值
    Register(int number, std::string&& name, std::bitset<32>&& value) : number{ number }, name{ std::move(name) }, value{ std::move(value) } {}

    std::string& GetName() const { return name; }
    int Getnumber() const { return number; }
    std::bitset<32> Getvalue() const { return value; }
    
    friend class System;
};


//Memory class
//构造函数: 0 参数
//***********类外部接口*****************
//无
//**************错误********************
//无
class Memory
{
private:
    //内存以map<地址,存储内容>的映射来模拟
    std::map<unsigned long, std::bitset<32>> text_segment;//代码段，储存代码，地址从0x00400000开始往上增长
    std::map<unsigned long, std::bitset<32>> data_segment;//数据段，储存静态数据，地址从0x10000000开始往上增长
    std::map<unsigned long, std::bitset<32>> stack_segment;//堆栈段，地址从0x7ffffffc开始往下递减
    std::bitset<32> texttop;//指向代码段的空内存的最小地址

public:
    //默认构造函数，初始化内存地址
    Memory() :text_segment{ make_pair(0x00400000,std::bitset<32>{}) },
        data_segment{ make_pair(0x10000000,std::bitset<32>{}) },
        stack_segment{ make_pair(0x7ffffffc,std::bitset<32>{}) }, texttop{ 0x00400000 } {};

    friend class System;
};

//System class
//构造函数: 0 参数
//**************************************类外部接口**************************************
//Register& FindRegister(int number)                    --> 根据编号寻找寄存器返回左值
//Register& FindRegister(const string& name)            --> 根据名称寻找寄存器返回左值
//bitset<32>& AccessMemory(const bitset<32>& address)   --> 输入地址访问内存返回存储内容
//void PushCodeToMemory(const bitset<32>& code)         --> 向内存代码段增加代码
//void PrintSystem() const                              --> 输出系统状态
//****************************************错误******************************************
//FindRegister时寄存器不存在
//AccessMemory中访问错误的内存地址
//PushCodeToMemory时代码大小超出内存代码段
class System
{
private:
    Memory mem;//内存
    std::vector<Register> registers{ 32 };//寄存器组
    Register PC;//程序计数器

public:
    //默认构造函数
    System()
    {
        PC.value = std::bitset <32>{ 0x00400000 };
        InitRegister();
        InitMemory();
    }

private:
    //初始化寄存器组
    void InitRegister();
    //将代码全部存入内存部分
    void InitMemory();

public:
    //根据编号找到寄存器并返回寄存器类
    Register& FindRegister(int number);

    //根据名称找到寄存器并返回寄存器类
    Register& FindRegister(const std::string& name);

    //访问内存，输入地址，返回内存地址内容
    std::bitset<32>& AccessMemory(const std::bitset<32>& address);

    //向内存代码段添加机器指令
    void PushCodeToMemory(const std::bitset<32>& code);

    //显示内存状态
    void PrintSystem() const;

    //判断指令类型
    void JudgeInstruction(const std::bitset<32>& code);
    
    //I型指令
    void InstructionIType(const std::string machineCode);

    //J型指令
    void InstructionJType(const std::string machineCode);//J型指令只有J，所以直接判断即可

    //R型指令
    void InstructionRType(const std::string machineCode);

    //各种指令类型对的系统操作
    void Add(int rs, int rt, int rd);
    void Addi(int rs, int rt, int imm);
    void Sub(int rs, int rt, int rd);
    void Sll(int rt, int rd, int shamt);
    void And(int rs, int rt, int rd);
    void Or(int rs, int rt, int rd);
    void Jr(int rs);//
    void Lw(int rs, int rt, int offset);//
    void Sw(int rs, int rt, int offset);//
    void Beq(int rs, int rt, int offset);//

};
