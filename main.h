#pragma once
#include <iostream>
#include <algorithm>
#include <cstring>
#include <bitset>
#include <vector>

// 寄存器
class Register {
public:
    int number;
    std::string name;
    std::bitset<32> value;

    Register(int number, const std::string &name, const std::bitset<32> &value) : number(number), name(name), value(value) {}

    void Reset(int number, std::string name) {
        this->number = number;
        this->name = name;
    }
};