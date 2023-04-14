#pragma once
#include<iostream>
#include<vector>
#include<map>
#include<utility>
#include<string>
#include<bitset>
#include<cstdlib>

//寄存器
class Register
{
public:
    int number;//编号
    std::string name;//名称
    std::bitset<32> value;//存储内容

    //默认构造函数
    Register() : number{ -1 } {}

    //含参构造函数，接受左值
    Register(int number, const std::string& name, const std::bitset<32>& value) : number{ number }, name{ name }, value{ value } {}

    //含参构造函数，接受右值
    Register(int number, std::string&& name, std::bitset<32>&& value) : number{ number }, name{ std::move(name) }, value{ std::move(value) } {}

    //重新设置寄存器
    void Reset(int number, const std::string& name)
    {
        this->number = number;
        this->name = name;
    }
};

//内存
class Memory
{
public:
    //内存以map<地址,存储内容>的映射来模拟
    std::map<unsigned long, std::bitset<32>> text_segment;//代码段，储存代码，地址从0x00400000开始往上增长
    std::map<unsigned long, std::bitset<32>> data_segment;//数据段，储存静态数据，地址从0x10000000开始往上增长
    std::map<unsigned long, std::bitset<32>> stack_segment;//堆栈段，地址从0x7ffffffc开始往下递减

    //指向代码段的空内存的最小地址
    std::bitset<32> texttop;

    //默认构造函数，初始化内存地址
    Memory() :text_segment{ make_pair(0x00400000,std::bitset<32>{}) },
        data_segment{ make_pair(0x10000000,std::bitset<32>{}) },
        stack_segment{ make_pair(0x7ffffffc,std::bitset<32>{}) }, texttop{ 0x00400000 } {};
};

//系统
class System
{
public:
    Memory mem;//内存
    std::vector<Register> registers{ 32 };//寄存器组
    Register PC;//程序计数器

    //默认构造函数
    System()
    {
        PC.value = std::bitset <32>{ 0x00400000 };
        InitRegister();
    }

private:
    //初始化寄存器组
    void InitRegister();

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
};

