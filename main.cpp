#include "main.h"
#include "assembler.h"
#include "disassembler.h"

using namespace std;

const unsigned int StartAddress = 0x00400000;  // 0x00400000,指令的初始地址
vector<Register> registers;

static void InitRegister();  // 初始化寄存器

int main() {
    InitRegister();
    // 汇编器
    InputInsToMap();
    Compilation();
    PrintBinaryAddress();

    // 反汇编器
    vector<string> res = disassembler("0000100000010000000000000000000100100001001010001111111110011100");
    for (auto &i : res) {
        cout << i << endl;
    }
    return 0;
}

static void InitRegister() {
    bitset<32> b(0);
    Register temp(0, "$zero", b);
    registers.push_back(temp);  // 插入$zero
    temp.Reset(1, "at");
    registers.push_back(temp);  // 插入$at
    temp.Reset(2, "$v0");
    registers.push_back(temp);  // 插入$v0
    temp.Reset(3, "$v1");
    registers.push_back(temp);  // 插入$v1
    for (int i = 4; i < 8; i++) {
        temp.Reset(i, "$a" + to_string(i - 4));
        registers.push_back(temp);
        ;  // 插入$a
    }
    for (int i = 8; i < 16; i++) {
        temp.Reset(i, "$t" + to_string(i - 8));
        registers.push_back(temp);
        ;  // 插入$t
    }
    for (int i = 16; i < 24; i++) {
        temp.Reset(i, "$s" + to_string(i - 16));
        registers.push_back(temp);
        ;  // 插入$s
    }
    for (int i = 24; i < 26; i++) {
        temp.Reset(i, "$t" + to_string(i - 16));
        registers.push_back(temp);
        ;  // 插入$t
    }
    for (int i = 26; i < 28; i++) {
        temp.Reset(i, "$k" + to_string(i - 26));
        registers.push_back(temp);
        ;  // 插入$k
    }
    temp.Reset(28, "$gp");
    registers.push_back(temp);
    ;  // 插入$gp
    temp.Reset(29, "$sp");
    registers.push_back(temp);
    ;  // 插入$sp
    temp.Reset(30, "$fp");
    registers.push_back(temp);
    ;  // 插入$fp
    temp.Reset(31, "$ra");
    registers.push_back(temp);
    ;  // 插入$ra
}