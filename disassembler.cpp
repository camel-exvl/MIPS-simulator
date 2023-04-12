#include "disassembler.h"

using namespace std;

extern vector<Register> registers;
// 常见指令机器码: https://en.wikibooks.org/wiki/MIPS_Assembly/Instruction_Formats#Opcodes
static const map<int, string> functDic = {
    {0x00, "sll"},  {0x02, "srl"},  {0x03, "sra"},  {0x08, "jr"},   {0x09, "jalr"},  {0x10, "mfhi"},
    {0x11, "mthi"}, {0x12, "mflo"}, {0x13, "mtlo"}, {0x18, "mult"}, {0x19, "multu"}, {0x1A, "div"},
    {0x1B, "divu"}, {0x20, "add"},  {0x21, "addu"}, {0x22, "sub"},  {0x23, "subu"},  {0x24, "and"},
    {0x25, "or"},   {0x26, "xor"},  {0x27, "nor"},  {0x2A, "slt"},  {0x2B, "sltu"}};
static const map<int, string> opDic = {{0x04, "beq"},   {0x05, "bne"},  {0x06, "blez"},  {0x07, "bgtz"}, {0x08, "addi"},
                                       {0x09, "addiu"}, {0x0A, "slti"}, {0x0B, "sltiu"}, {0x0C, "andi"}, {0x0D, "ori"},
                                       {0x0E, "xori"},  {0x0F, "lui"},  {0x20, "lb"},    {0x21, "lh"},   {0x23, "lw"},
                                       {0x24, "lbu"},   {0x25, "lhu"},  {0x28, "sb"},    {0x29, "sh"},   {0x2B, "sw"},
                                       {0x02, "j"},     {0x03, "jal"}};

inline static string findNameByNum(const unsigned int num);  // 根据寄存器编号查找寄存器名称
static string instructionRType(const string machineCode);    // 处理R型指令
static string instructionIType(const string machineCode, const unsigned int insCount);  // 处理I型指令
static string instructionJType(const string machineCode, const unsigned int insCount);  // 处理J型指令
inline static int complement2int(const bitset<16> complement);                          // 补码转换为int

vector<string> disassembler(string s) {
    if (s.length() % 32) {
        throw "Error: The length of the binary code is not a multiple of 32.";
    }
    vector<string> instructions;
    for (int insCount = 0, n = s.length() / 32; insCount < n; insCount++) {
        int op = stoi(s.substr(insCount * 32, 6), nullptr, 2);
        if (op == 0) {
            instructions.push_back(instructionRType(s.substr(insCount * 32, 32)));
        } else if (op >> 1 & 1 || op >> 4 & 1) {
            instructions.push_back(instructionJType(s.substr(insCount * 32, 32), insCount));
        } else {
            instructions.push_back(instructionIType(s.substr(insCount * 32, 32), insCount));
        }
    }
    return instructions;
}

inline static string findNameByNum(const unsigned int num) { return registers[num].name; }

inline static int complement2int(const bitset<16> complement) {
    return complement[15] ? -((~complement).to_ulong() + 1) : complement.to_ulong();
}

static string instructionRType(const string machineCode) {
    int rs = stoi(machineCode.substr(6, 5), nullptr, 2);
    int rt = stoi(machineCode.substr(11, 5), nullptr, 2);
    int rd = stoi(machineCode.substr(16, 5), nullptr, 2);
    int shamt = stoi(machineCode.substr(21, 5), nullptr, 2);
    int funct = stoi(machineCode.substr(26, 6), nullptr, 2);
    string instruction;
    if (funct == 0x1a || funct == 0x1b || funct == 0x18 || funct == 0x19) {  // div, divu, mult, multu
        instruction = functDic.at(funct) + " " + findNameByNum(rs) + ", " + findNameByNum(rt);
    } else if (funct == 0x00 || funct == 0x02 || funct == 0x03) {  // sll, srl, sra
        instruction = functDic.at(funct) + " " + findNameByNum(rd) + ", " + findNameByNum(rt) + ", " + to_string(shamt);
    } else if (funct == 0x08 || funct == 0x11 || funct == 0x13) {  // jr, mthi, mtlo
        instruction = functDic.at(funct) + " " + findNameByNum(rs);
    } else if (funct == 0x09) {  // jalr
        instruction = functDic.at(funct) + " " + findNameByNum(rs) + ", " + findNameByNum(rd);
    } else if (funct == 0x10 || funct == 0x12) {  // mfhi, mflo
        instruction = functDic.at(funct) + " " + findNameByNum(rd);
    } else {  // add, addu, sub, subu, and, or, xor, nor, slt, sltu
        instruction =
            functDic.at(funct) + " " + findNameByNum(rd) + ", " + findNameByNum(rs) + ", " + findNameByNum(rt);
    }
    return instruction;
}
static string instructionIType(const string machineCode, const unsigned int insCount) {
    int op = stoi(machineCode.substr(0, 6), nullptr, 2);
    int rs = stoi(machineCode.substr(6, 5), nullptr, 2);
    int rt = stoi(machineCode.substr(11, 5), nullptr, 2);
    bitset<16> immediate(machineCode.substr(16, 16));
    int imm = complement2int(immediate);
    int PC = 0x00400000 + (insCount << 2);
    string instruction;
    if (op == 0x0f) {  // lui
        instruction = opDic.at(op) + " " + findNameByNum(rt) + ", " + to_string(imm);
    } else if (op == 0x04 || op == 0x05) {  // beq, bne
        instruction =
            opDic.at(op) + " " + findNameByNum(rs) + ", " + findNameByNum(rt) + ", " + to_string(PC + 4 + (imm << 2));
    } else if (op == 0x06 || op == 0x07) {  // blez, bgtz
        instruction = opDic.at(op) + " " + findNameByNum(rs) + ", " + to_string(PC + 4 + (imm << 2));
    } else if (op == 0x20 || op == 0x21 || op == 0x23 || op == 0x24 || op == 0x25) {  // lb, lh, lw, lbu, lhu
        instruction = opDic.at(op) + " " + findNameByNum(rt) + ", " + to_string(imm) + "(" + findNameByNum(rs) + ")";
    } else if (op == 0x28 || op == 0x29 || op == 0x2B) {  // sb, sh, sw
        instruction = opDic.at(op) + " " + findNameByNum(rt) + ", " + to_string(imm) + "(" + findNameByNum(rs) + ")";
    } else {  // addi, addiu, slti, sltiu, andi, ori, xori
        instruction = opDic.at(op) + " " + findNameByNum(rt) + ", " + findNameByNum(rs) + ", " + to_string(imm);
    }
    return instruction;
}

static string instructionJType(const string machineCode, const unsigned int insCount) {
    int op = stoi(machineCode.substr(0, 6), nullptr, 2);
    int addr = stoi(machineCode.substr(6, 26), nullptr, 2);
    int PC = 0x00400000 + (insCount << 2);
    string instruction;
    if (op == 0x02) {  // j
        instruction = opDic.at(op) + " " + to_string((PC & 0xf0000000) | (addr << 2));
    } else {  // jal
        instruction = opDic.at(op) + " " + to_string((PC & 0xf0000000) | (addr << 2));
    }
    return instruction;
}