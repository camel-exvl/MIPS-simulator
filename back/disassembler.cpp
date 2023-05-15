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

static string instructionRType(System& sys, const string machineCode);                               // 处理R型指令
static string instructionIType(System& sys, const string machineCode);                               // 处理I型指令
static string instructionJType(System& sys, const string machineCode, const unsigned int insCount);  // 处理J型指令
inline static int complement2int(const bitset<16> complement);  // 补码转换为int

vector<string> disassembler(System& sys, vector<bitset<32>> s) {
    vector<string> instructions;
    for (int i = 0; i < s.size(); i++) {
        int op = s[i].to_ulong() >> 26;
        if (op == 0) {
            instructions.push_back(instructionRType(sys, s[i].to_string()));
        } else if (op >> 1 == 1 || op >> 2 == 0b100) {
            instructions.push_back(instructionJType(sys, s[i].to_string(), i));
        } else {
            instructions.push_back(instructionIType(sys, s[i].to_string()));
        }
    }
    return instructions;
}

vector<pair<string, string>> disassemblerOpenFile(System& sys, const string& fileName) {
    ifstream fin(fileName, ios::binary);
    if (!fin) {
        throw "Error: Cannot open the file.";
    }
    fin.seekg(0, ios::end);
    int fileSize = fin.tellg();
    if (fileSize % 4 != 0) {
        throw "Error: The file is not a valid MIPS binary file.";
    }
    fin.seekg(0, ios::beg);
    vector<pair<string, string>> result;
    vector<bitset<32>> machineCodes;
    bitset<32> machineCode;
    char ch;
    for (int i = 0; i < fileSize / 4; i++) {
        for (int j = 0; j < 4; j++) {
            fin.read(&ch, 1);
            machineCode <<= 8;
            for (int k = 0; k < 8; k++) {
                machineCode[k] = ch & 1;
                ch >>= 1;
            }
        }
        machineCodes.push_back(machineCode);
        stringstream ss;
        ss << hex << machineCode.to_ulong();
        result.push_back(make_pair("0x" + ss.str(), disassembler(sys, machineCodes).back()));
        machineCode.reset();
        machineCodes.clear();
    }
    fin.close();
    return result;
}

inline static int complement2int(const bitset<16> complement) {
    return complement[15] ? -((~complement).to_ulong() + 1) : complement.to_ulong();
}

static string instructionRType(System& sys, const string machineCode) {
    int rs = stoi(machineCode.substr(6, 5), nullptr, 2);
    int rt = stoi(machineCode.substr(11, 5), nullptr, 2);
    int rd = stoi(machineCode.substr(16, 5), nullptr, 2);
    int shamt = stoi(machineCode.substr(21, 5), nullptr, 2);
    int funct = stoi(machineCode.substr(26, 6), nullptr, 2);
    string instruction;
    if (funct == 0x1a || funct == 0x1b || funct == 0x18 || funct == 0x19) {  // div, divu, mult, multu
        instruction = functDic.at(funct) + " " + sys.FindRegister(rs).GetName() + ", " + sys.FindRegister(rt).GetName();
    } else if (funct == 0x00 || funct == 0x02 || funct == 0x03) {  // sll, srl, sra
        instruction = functDic.at(funct) + " " + sys.FindRegister(rd).GetName() + ", " +
                      sys.FindRegister(rt).GetName() + ", " + to_string(shamt);
    } else if (funct == 0x08 || funct == 0x11 || funct == 0x13) {  // jr, mthi, mtlo
        instruction = functDic.at(funct) + " " + sys.FindRegister(rs).GetName();
    } else if (funct == 0x09) {  // jalr
        instruction = functDic.at(funct) + " " + sys.FindRegister(rs).GetName() + ", " + sys.FindRegister(rd).GetName();
    } else if (funct == 0x10 || funct == 0x12) {  // mfhi, mflo
        instruction = functDic.at(funct) + " " + sys.FindRegister(rd).GetName();
    } else {  // add, addu, sub, subu, and, or, xor, nor, slt, sltu
        instruction = functDic.at(funct) + " " + sys.FindRegister(rd).GetName() + ", " +
                      sys.FindRegister(rs).GetName() + ", " + sys.FindRegister(rt).GetName();
    }
    return instruction;
}
static string instructionIType(System& sys, const string machineCode) {
    int op = stoi(machineCode.substr(0, 6), nullptr, 2);
    int rs = stoi(machineCode.substr(6, 5), nullptr, 2);
    int rt = stoi(machineCode.substr(11, 5), nullptr, 2);
    bitset<16> immediate(machineCode.substr(16, 16));
    int imm = complement2int(immediate);
    string instruction;
    if (op == 0x0f) {  // lui
        instruction = opDic.at(op) + " " + sys.FindRegister(rt).GetName() + ", " + to_string(imm);
    } else if (op == 0x04 || op == 0x05) {  // beq, bne
        instruction = opDic.at(op) + " " + sys.FindRegister(rs).GetName() + ", " + sys.FindRegister(rt).GetName() +
                      ", " + to_string(imm);
    } else if (op == 0x06 || op == 0x07) {  // blez, bgtz
        instruction = opDic.at(op) + " " + sys.FindRegister(rs).GetName() + ", " + to_string(imm);
    } else if (op == 0x20 || op == 0x21 || op == 0x23 || op == 0x24 || op == 0x25) {  // lb, lh, lw, lbu, lhu
        instruction = opDic.at(op) + " " + sys.FindRegister(rt).GetName() + ", " + to_string(imm) + "(" +
                      sys.FindRegister(rs).GetName() + ")";
    } else if (op == 0x28 || op == 0x29 || op == 0x2B) {  // sb, sh, sw
        instruction = opDic.at(op) + " " + sys.FindRegister(rt).GetName() + ", " + to_string(imm) + "(" +
                      sys.FindRegister(rs).GetName() + ")";
    } else {  // addi, addiu, slti, sltiu, andi, ori, xori
        instruction = opDic.at(op) + " " + sys.FindRegister(rt).GetName() + ", " + sys.FindRegister(rs).GetName() +
                      ", " + to_string(imm);
    }
    return instruction;
}

static string instructionJType(System& sys, const string machineCode, const unsigned int insCount) {
    int op = stoi(machineCode.substr(0, 6), nullptr, 2);
    int addr = stoi(machineCode.substr(6, 26), nullptr, 2);
    int PC = 0x00400000 + (insCount << 2);  // FIXME: 注意这边的PC最后要和sys统一
    string instruction;
    if (op == 0x02) {  // j
        instruction = opDic.at(op) + " " + to_string((PC & 0xf0000000) | (addr << 2));
    } else {  // jal
        instruction = opDic.at(op) + " " + to_string((PC & 0xf0000000) | (addr << 2));
    }
    return instruction;
}
