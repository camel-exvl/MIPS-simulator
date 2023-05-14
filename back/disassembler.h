#pragma once
#include "main.h"
#include <cstring>
#include <map>
#include <fstream>

// 反汇编
// Input sys: System系统类 s: 二进制机器码(长度不为32的倍数时会抛出异常)
// Output 反汇编后的指令
std::vector<std::string> disassembler(System& sys, std::vector<std::bitset<32>> s);

// 反汇编文件读入
// Input sys: System系统类 fileName: 文件名
// Output 反汇编后的指令
std::vector<std::string> disassemblerOpenFile(System& sys, const std::string& fileName);