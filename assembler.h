#pragma once
#include <sstream>
#include "iterator"
#include "main.h"

class Instruction {
public:
    std::bitset<32> address;// 指令地址
    std::vector<std::string> assemblecode;// 汇编代码（按空格分割成数组并去掉逗号）
    std::bitset<32> machinecode;//机器码
    Instruction(const std::bitset<32> &address, const std::vector<std::string> &assemblecode, const std::bitset<32> &machinecode) : address(
            address), assemblecode(assemblecode), machinecode(machinecode) {};
};

std::vector<std::bitset<32>> assembler(System& sys, std::vector<std::string> s);

std::vector<std::bitset<32>> assemblerOpenFile(System& sys, const std::string& fileName);

// 读取汇编码并按空格分隔成vector<string>并去除逗号，
// 如果第一个子字符串不是要处理的指令码，判断其为标签，去除并加入标签库，处理后的结果插入Instruction
void InputInsToMap();

void Compilation(System& sys);//汇编的主要程序

void PrintBinaryAddress();

void PrintHexAddress();