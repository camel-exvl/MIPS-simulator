#pragma once
#include"main.h"
#include <stack>
static int complement2int(const std::bitset<16> complement);

static int complement2int(const std::bitset<32> complement);

//整数转原码
std::bitset<32> turnIntToTruecode(int number);

//整数转补码
std::bitset<32> turnIntToComplementcode(int number);

//浮点数转原码
std::bitset<32> turnFloatToTruecode(float number);

//浮点数转补码
std::bitset<32> turnFloatToComplementcode(float number);

int getPriority(char op);

std::vector<std::string> toRPN(std::string s);//传入字符串将中缀表达式转为后缀表达式

double calculateRPN(std::vector<std::string> rpn);//传入后缀表达式并计算出值

/*
int main() {
    std::string expression;
    std::getline(std::cin, expression);
    std::vector<std::string> rpn = toRPN(expression);
    double result = calculateRPN(rpn);
    std::cout << "The result is: " << result << std::endl;
    return 0;
}
*/