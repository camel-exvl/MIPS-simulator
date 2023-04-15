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

class Register;


//Memory class
//构造函数: 0 参数
//***********类外部接口*****************
//无
//**************错误********************
//无

class Memory;

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

class System;

