#include "assembler.h"

using namespace std;

//extern vector<Register> registers;
const unsigned int StartAddress = 0x00400000;  // 0x00400000,指令的初始地址

// 记录指令数量
unsigned int Count = 0;
// 要处理的所有指令
string allinstructions[] = {"add", "addi", "sub",
                            "and", "or", "sll",
                            "j", "jr", "lw",
                            "sw", "beq"};

vector<Instruction> instructions;
vector<pair<bitset<32>, string>> Lables;//标签库，存储标签的地址及字符串

static void Add(vector<string> code, unsigned int i, System& sys);

static void Addi(vector<string> code, unsigned int i, System& sys);

static void Sub(vector<string> code, unsigned int i, System& sys);

static void Sll(vector<string> code, unsigned int i, System& sys);

static void And(vector<string> code, unsigned int i, System& sys);

static void Or(vector<string> code, unsigned int i, System& sys);

static void J(vector<string> code, unsigned int i, System& sys);

static void Jr(vector<string> code, unsigned int i, System& sys);

static void Lw(vector<string> code, unsigned int i, System& sys);

static void Sw(vector<string> code, unsigned int i, System& sys);

static void Beq(vector<string> code, unsigned int i, System& sys);

// int main() {
//     InitRegister();// 初始化寄存器
//     InputInsToMap();
//     Compilation();
//     PrintBinaryAddress();
//     //PrintHexAddress();
//     return 0;
// }
vector<bitset<32>> assembler(System& sys, vector<string> s) {
    string temp;
    for (int i = 0; i < s.size(); i++) {// 读取指令并插入，地址统一从0开始
        temp = s[i];
        string result;// 临时存放分割后的子字符串
        vector<string> words;
        stringstream iss(temp);
        int pos;//','的位置
        while (iss >> result) {// 按空格分隔后push入vector
            if ((pos = result.find(',')) >= 0)
                result.erase(pos);
            words.push_back(result);
        }
        // 如果第一个字符串不是任何我们要处理的指令字符串，将其作为标签处理
        if (count(begin(allinstructions), end(allinstructions), words.at(0)) <= 0) {
            // 将标签加入标签库并在原指令开头中去除
            words[0].pop_back();//去掉标签末尾的':'
            Lables.emplace_back(StartAddress + Count * 4, words.at(0));
            words.erase(words.begin());
        }
        Instruction tempins(bitset<32>(StartAddress + (Count++) * 4), words, bitset<32>(0));
        instructions.push_back(tempins);
    }
    Compilation(sys);
    vector<bitset<32>> re;
    for (int i = 0; i < instructions.size(); i++) {
        re.push_back(instructions[i].machinecode);
        sys.PushCodeToMemory(instructions[i].machinecode);
    }
    return re;
}

std::vector<std::bitset<32>> assemblerOpenFile(System& sys, const string& fileName){
    ifstream fin(fileName, ios::in);
    if (!fin) {
        throw "Error: Cannot open the file.";
    }
    fin.seekg(0, ios::end);
    fin.seekg(0, ios::beg);
    vector<string> assemblerCodes;
    string assemblerCode;
    while(getline(fin,assemblerCode)){
        assemblerCodes.push_back(assemblerCode);
    }
    fin.close();
    return assembler(sys,assemblerCodes);
}

void assemblerToFile(System& sys, std::vector<std::string> s,const std::string& fileName){
    ofstream ofs(fileName, ios::out | ios::binary);
    if (!ofs.is_open()) {
        throw "Error: Cannot open the file.";
    }
    vector<std::bitset<32>> result;
    result = assembler(sys,s);
    for (const auto &item: result) {
        ofs.write((char *) &item + 3, 1);
        ofs.write((char *) &item + 2, 1);
        ofs.write((char *) &item + 1, 1);
        ofs.write((char *) &item + 0, 1);
    }
    ofs.close();
}



// 读取汇编码并按空格分隔成vector<string>并去除逗号，处理标签
void InputInsToMap() {
    string temp;
    while (true) {// 读取指令并插入，地址统一从0开始
        getline(cin, temp, '\n');
        if (temp.empty())
            break;
        string result;// 临时存放分割后的子字符串
        vector<string> words;
        stringstream iss(temp);
        int pos;//','的位置
        while (iss >> result) {// 按空格分隔后push入vector
            if ((pos = result.find(',')) >= 0)
                result.erase(pos);
            words.push_back(result);
        }
        // 如果第一个字符串不是任何我们要处理的指令字符串，将其作为标签处理
        if (count(begin(allinstructions), end(allinstructions), words.at(0)) <= 0) {
            // 将标签加入标签库并在原指令开头中去除
            words[0].pop_back();//去掉标签末尾的':'
            Lables.emplace_back(StartAddress + Count * 4, words.at(0));
            words.erase(words.begin());
        }
        Instruction tempins(bitset<32>(StartAddress + (Count++) * 4), words, bitset<32>(0));
        instructions.push_back(tempins);
    }
}

// 汇编的主程序
void Compilation(System& sys) {
    for (int i = 0; i < Count; i++) {
        if (instructions.at(i).assemblecode.at(0) == "add") {
            Add(instructions.at(i).assemblecode, i, sys);
            continue;
        }
        if (instructions.at(i).assemblecode.at(0) == "addi") {
            Addi(instructions.at(i).assemblecode, i, sys);
            continue;
        }
        if (instructions.at(i).assemblecode.at(0) == "sub") {
            Sub(instructions.at(i).assemblecode, i, sys);
            continue;
        }
        if (instructions.at(i).assemblecode.at(0) == "sll") {
            Sll(instructions.at(i).assemblecode, i, sys);
            continue;
        }
        if (instructions.at(i).assemblecode.at(0) == "and") {
            And(instructions.at(i).assemblecode, i, sys);
            continue;
        }
        if (instructions.at(i).assemblecode.at(0) == "or") {
            Or(instructions.at(i).assemblecode, i, sys);
            continue;
        }
        if (instructions.at(i).assemblecode.at(0) == "j") {
            J(instructions.at(i).assemblecode, i, sys);
            continue;
        }
        if (instructions.at(i).assemblecode.at(0) == "jr") {
            Jr(instructions.at(i).assemblecode, i, sys);
            continue;
        }
        if (instructions.at(i).assemblecode.at(0) == "lw") {
            Lw(instructions.at(i).assemblecode, i, sys);
            continue;
        }
        if (instructions.at(i).assemblecode.at(0) == "sw") {
            Sw(instructions.at(i).assemblecode, i, sys);
            continue;
        }
        if (instructions.at(i).assemblecode.at(0) == "beq") {
            Beq(instructions.at(i).assemblecode, i, sys);
            continue;
        }
    }
}

//// 根据寄存器名字找到编号
//static int FindNumByName(const string &name) {
//    for (auto &i: registers) {
//        if (i.name == name)
//            return i.number;
//    }
//    return -1;// 未找到
//}

//// 根据寄存器名字找到值
//static bitset<32> FindValueByName(const string &name) {
//    for (auto &i: registers) {
//        if (i.name == name)
//            return i.value;
//    }
//    return NULL;// 未找到
//}

// 根据标签名找到对应的地址
static bitset<32> FindAddressByName(const string &name) {
    for (auto &i: Lables) {
        if (i.second == name)
            return i.first;
    }
    return NULL;// 未找到
}

//add rd, rs, rt
static void Add(vector<string> code, unsigned int i, System& sys) {
    int rd = sys.FindRegister(code.at(1)).Getnumber();
    int rs = sys.FindRegister(code.at(2)).Getnumber();
    int rt = sys.FindRegister(code.at(3)).Getnumber();
    unsigned int decimal = 32 + rd * 2048 + rt * 65536 + rs * 2097152;
    bitset<32> result(decimal);// 二进制的机器码
    instructions.at(i).machinecode = result;
}

static void Addi(vector<string> code, unsigned int i, System& sys) {
    int rt = sys.FindRegister(code.at(1)).Getnumber();
    int rs = sys.FindRegister(code.at(2)).Getnumber();
    int imm = stoi(code.at(3));
    unsigned int decimal;
    if (imm >= 0) {
        decimal = imm + rt * 65536 + rs * 2097152 + 536870912;
    } else {
        decimal = 65536 + imm + rt * 65536 + rs * 2097152 + 536870912;
    }

    bitset<32> result(decimal);// 二进制的机器码
    instructions.at(i).machinecode = result;
}

static void Sub(vector<string> code, unsigned int i, System& sys) {
    int rd = sys.FindRegister(code.at(1)).Getnumber();
    int rs = sys.FindRegister(code.at(2)).Getnumber();
    int rt = sys.FindRegister(code.at(3)).Getnumber();
    unsigned int decimal = 34 + rd * 2048 + rt * 65536 + rs * 2097152;
    bitset<32> result(decimal);// 二进制的机器码
    instructions.at(i).machinecode = result;
}

static void Sll(vector<string> code, unsigned int i, System& sys) {
    int rd = sys.FindRegister(code.at(1)).Getnumber();
    int rt = sys.FindRegister(code.at(2)).Getnumber();
    int shame = stoi(code.at(3));
    unsigned int decimal = shame * 64 + rd * 2048 + rt * 65536;
    bitset<32> result(decimal);// 二进制的机器码
    instructions.at(i).machinecode = result;
}

static void And(vector<string> code, unsigned int i, System& sys) {
    int rd = sys.FindRegister(code.at(1)).Getnumber();
    int rs = sys.FindRegister(code.at(2)).Getnumber();
    int rt = sys.FindRegister(code.at(3)).Getnumber();
    unsigned int decimal = 36 + rd * 2048 + rt * 65536 + rs * 2097152;
    bitset<32> result(decimal);// 二进制的机器码
    instructions.at(i).machinecode = result;
}

static void Or(vector<string> code, unsigned int i, System& sys) {
    int rd = sys.FindRegister(code.at(1)).Getnumber();
    int rs = sys.FindRegister(code.at(2)).Getnumber();
    int rt = sys.FindRegister(code.at(3)).Getnumber();
    unsigned int decimal = 37 + rd * 2048 + rt * 65536 + rs * 2097152;
    bitset<32> result(decimal);// 二进制的机器码
    instructions.at(i).machinecode = result;
}

static void J(vector<string> code, unsigned int i, System& sys) {
    bitset<32> address = FindAddressByName(code.at(1));// 二进制的标签地址
    // 如果指令后的地址是用标签表示的,address不会为null
    if (address == NULL) {// 十进制地址值跳转
        address = bitset<32>(stoi(code.at(1)));
    }
    address = address >> 2;// 右移两位
    int result = address.to_ulong();
    result = result + 134217728;
    instructions.at(i).machinecode = bitset<32>(result);
}

static void Jr(vector<string> code, unsigned int i, System& sys) {
    int rs = sys.FindRegister(code.at(1)).Getnumber();
    unsigned int decimal = 8 + rs * 2097152;
    bitset<32> result(decimal);// 二进制的机器码
    instructions.at(i).machinecode = bitset<32>(result);
}

//lw $t0, 0($t1)
static void Lw(vector<string> code, unsigned int i, System& sys) {
    int rt = sys.FindRegister(code.at(1)).Getnumber();
    int startpos = code.at(2).find_first_of('(');// 找出第一个左括号的位置，进而分理出rs
    int endpos = code.at(2).find_last_of(')');// 找出第一个左括号的位置，进而分理出rs
    string rs_s = code.at(2).substr(startpos + 1, endpos - startpos - 1);
    int rs = sys.FindRegister(rs_s).Getnumber();
    int offset = stoi(code.at(2).substr(0, startpos));
    unsigned int decimal;
    if (offset >= 0) {
        decimal = offset + rt * 65536 + rs * 2097152 + 2348810240;
    } else {
        decimal = 65536 + offset + rt * 65536 + rs * 2097152 + 2348810240;
    }
    bitset<32> result(decimal);// 二进制的机器码
    instructions.at(i).machinecode = bitset<32>(result);
}

static void Sw(vector<string> code, unsigned int i, System& sys) {
    int rt = sys.FindRegister(code.at(1)).Getnumber();
    int startpos = code.at(2).find_first_of('(');// 找出第一个左括号的位置，进而分理出rs
    int endpos = code.at(2).find_last_of(')');// 找出第一个左括号的位置，进而分理出rs
    string rs_s = code.at(2).substr(startpos + 1, endpos - startpos - 1);
    int rs = sys.FindRegister(rs_s).Getnumber();
    int offset = stoi(code.at(2).substr(0, startpos));
    unsigned int decimal;
    if (offset >= 0) {
        decimal = offset + rt * 65536 + rs * 2097152 + 2885681152;
    } else {
        decimal = 65536 + offset + rt * 65536 + rs * 2097152 + 2885681152;
    }
    bitset<32> result(decimal);// 二进制的机器码
    instructions.at(i).machinecode = bitset<32>(result);
}

static void Beq(vector<string> code, unsigned int i, System& sys) {
    int rs = sys.FindRegister(code.at(1)).Getnumber();
    int rt = sys.FindRegister(code.at(2)).Getnumber();
    bitset<32> label = FindAddressByName(code.at(3));// 标签对应的二进制地址值
    if (label != NULL) {
        long dec_label = label.to_ulong();
        long pc = instructions.at(i).address.to_ulong();
        int offset = (dec_label - pc - 4) / 4;
        unsigned int decimal;
        if (offset >= 0) {
            decimal = offset + rt * 65536 + rs * 2097152 + 268435456;
        } else {
            decimal = 65536 + offset + rt * 65536 + rs * 2097152 + 268435456;
        }

        bitset<32> result(decimal);// 二进制的机器码
        instructions.at(i).machinecode = bitset<32>(result);
    }
}

void PrintBinaryAddress() {
    for (auto &instruction: instructions) {
        cout << instruction.machinecode << endl;
    }
}

void PrintHexAddress() {
    for (auto &instruction: instructions) {
        cout << hex << instruction.machinecode.to_ulong() << endl;
    }
}





