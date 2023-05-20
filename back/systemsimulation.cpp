#include"main.h"
#include"systemsimulation.h"

using namespace std;
int textline=0;//设置输入数据目前读取到第几行
//初始化寄存器组
void System::InitRegister()
{
    bitset<32> b(0);
    registers.at(0) = Register{ 0, "$zero", b };  // 插入$zero
    registers.at(1) = Register{ 1, "$at", b };  // 插入$at
    for (int i = 2; i < 4; i++) {
        registers.at(i) = Register{ i, "$v" + to_string(i - 2), b };
        // 插入$v0-%v1
    }
    for (int i = 4; i < 8; i++) {
        registers.at(i) = Register{ i, "$a" + to_string(i - 4), b };
        // 插入$a0-%a3
    }
    for (int i = 8; i < 16; i++) {
        registers.at(i) = Register{ i, "$t" + to_string(i - 8), b };
        // 插入$t1-$t7
    }
    for (int i = 16; i < 24; i++) {
        registers.at(i) = Register{ i, "$s" + to_string(i - 16), b };
        // 插入$s1-$s7
    }
    for (int i = 24; i < 26; i++) {
        registers.at(i) = Register{ i, "$t" + to_string(i - 16), b };
        // 插入$t8-$t9
    }
    for (int i = 26; i < 28; i++) {
        registers.at(i) = Register{ i, "$k" + to_string(i - 26), b };
        // 插入$k0-%k1
    }
    registers.at(28) = Register{ 28, "$gp", (bitset <32>)0x10008000 };  // 插入$gp
    registers.at(29) = Register{ 29, "$sp", (bitset <32>)0x7ffffffc };  // 插入$sp
    registers.at(30) = Register{ 30, "$fp", (bitset <32>)0x7ffffffc };  // 插入$fp
    registers.at(31) = Register{ 31, "$ra", b };  // 插入$ra
}


//根据编号找到寄存器并返回寄存器类
Register& System::FindRegister(int number)
{
    if (number < 32 && number >= 0)
    {
        return registers.at(number);
    }
    //若找不到则报错
    cerr << "寄存器不存在" << endl;
    exit(-1);
}

//根据名称找到寄存器并返回寄存器类
Register& System::FindRegister(const string& name)
{
    //遍历寄存器组
    for (auto& i : registers)
        if (i.name == name)
            return i;
    //若找不到则报错
    cerr << "寄存器不存在" << endl;
    exit(-1);
}

//访问内存，输入地址，返回内存地址内容
bitset<32>& System::AccessMemory(const bitset<32>& address)
{
    //保留区，禁止访问
    if (address.to_ulong() < 0x00400000)
    {
        cerr << "内存保留区域，不允许访问" << endl;
        exit(-1);
    }
    //代码段，返回机器指令
    else if (address.to_ulong() < 0x10000000)
    {
        if (address.to_ulong() < mem.texttop.to_ulong())
            return mem.text_segment.at(address.to_ulong());
        else//如果访问内存空代码段报错
        {
            cerr << "该地址没有代码存储，不允许访问" << endl;
            exit(-1);
        }
    }
    else if (address.to_ulong() < 0x7fffffff)
    {
        //数据段，返回存储数据
        if (address.to_ulong() < mem.datatop.to_ulong())
        {
            return mem.data_segment[address.to_ulong()];
        }
        //堆栈段，返回堆栈数据
        else if (address.to_ulong() >= registers.at(29).value.to_ulong())
        {
            return mem.stack_segment[address.to_ulong()];
        }
        else//如果访问数据段与堆栈段之间的空内存报错
        {
            cerr << "该地址没有内容存储，不允许访问" << endl;
            exit(-1);
        }
    }
    //地址越界报错
    else
    {
        cerr << "错误的内存地址，超出内存大小" << endl;
        exit(-1);
    }
}

const bitset<32>& System::ReadMemory(const bitset<32>& address)
{
    //保留区，禁止访问
    if (address.to_ulong() < 0x00400000)
    {
        cerr << "内存保留区域，不允许访问" << endl;
        exit(-1);
    }
    //代码段，返回机器指令
    else if (address.to_ulong() < 0x10000000)
    {
        return mem.text_segment[address.to_ulong()];
    }
    else if (address.to_ulong() < 0x7fffffff)
    {
        //数据段，返回存储数据
        if (address.to_ulong() <  registers.at(29).value.to_ulong())
        {
            return mem.data_segment[address.to_ulong()];
        }
        //堆栈段，返回堆栈数据
        else
        {
            return mem.stack_segment[address.to_ulong()];
        }
    }
    //地址越界报错
    else
    {
        cerr << "错误的内存地址，超出内存大小" << endl;
        exit(-1);
    }
}

//向内存代码段添加机器指令
void System::PushCodeToMemory(const bitset<32>& code)
{
    mem.text_segment[mem.texttop.to_ulong()] = code;
    mem.texttop = bitset<32>{ mem.texttop.to_ulong() + 0x04 };
    //若代码超出内存代码段报错
    if (mem.texttop.to_ulong() > 0x10000100)
    {
        cerr << "代码数量超过内存代码段大小" << endl;
        exit(-1);
    }
}

void System::PrintSystem() const
{
    //看后期要不要做图形界面
}

//单步执行内存中的指令
bitset<32> System::OneStepExecute()
{
    if (PC.Getvalue() == mem.texttop)
    {
        cerr << "已执行完所有指令" << endl;
        return PC.Getvalue();
    }
    const bitset<32>& code = AccessMemory(PC.Getvalue());
    JudgeInstruction(code);
    return PC.Getvalue();
}

//添加断点
void System::AddBreakPoint(const bitset<32> address)
{
    breakpoints[address.to_ulong()] = true;
}

//删除断点
void System::RemoveBreakPoint(const std::bitset<32> address)
{
    breakpoints[address.to_ulong()] = false;
}

//执行内存中的指令至断点
bitset<32> System::BreakPointExecute()
{
    if (OneStepExecute() == mem.texttop)
    {
        cerr << "已执行完所有指令" << endl;
        return PC.Getvalue();
    }
    //若当前指令不是断点就继续执行
    while (breakpoints[PC.Getvalue().to_ulong()] == false)
    {
        if (OneStepExecute() == mem.texttop)
        {
            cerr << "已执行完所有指令" << endl;
            return PC.Getvalue();
        }
    }
    return PC.Getvalue();
}

//重新调试
void System::Reset()
{
    System s;
    this->mem = s.mem;
    this->registers = s.registers;
    this->PC = s.PC;
    this->breakpoints = s.breakpoints;
}

//R型指令
void System::InstructionRType(const string machineCode)
{
    int rs = stoi(machineCode.substr(6, 5), nullptr, 2);
    int rt = stoi(machineCode.substr(11, 5), nullptr, 2);
    int rd = stoi(machineCode.substr(16, 5), nullptr, 2);
    int shamt = stoi(machineCode.substr(21, 5), nullptr, 2);
    int funct = stoi(machineCode.substr(26, 6), nullptr, 2);
    if(funct== 0x20)
    Add(rs,rt,rd);
    else if(funct== 0x22)
    Sub(rs, rt, rd);
    else if(funct== 0x00)
    Sll(rt, rd, shamt);
    else if(funct== 0x24)
    And(rs, rt, rd);
    else if(funct== 0x25)
    Or(rs, rt, rd);
    else
    Jr(rs);
}

//J型指令
void System::InstructionJType(const string machineCode)//J型指令只有J，所以直接判断即可
{
    int addr = stoi(machineCode.substr(6, 26), nullptr, 2);
    string str = PC.Getvalue().to_string();
    string first = str.substr(0, 4);
    string second = to_string(addr);
    string strPC = first+second+"00";
    bitset<32> bits(strPC);
    PC.value=bits;
    //26位后面加00，前面取PC的前4位加起来组成32位   
}

inline static int complement2int(const bitset<16> complement) {
    return complement[15] ? -(int)((~complement).to_ulong() + 1) : complement.to_ulong();
}

inline static int complement2int(const bitset<32> complement) {
    return complement[31] ? -(int)((~complement).to_ulong() + 1) : complement.to_ulong();
}

//I型指令
void System::InstructionIType(const string machineCode)
{
    int op = stoi(machineCode.substr(0, 6), nullptr, 2);
    int rs = stoi(machineCode.substr(6, 5), nullptr, 2);
    int rt = stoi(machineCode.substr(11, 5), nullptr, 2);
    bitset<16> immediate(machineCode.substr(16, 16));
    int imm = complement2int(immediate);
    if(op== 0x08)
    Addi(rs,rt,imm);
    else if(op== 0x23)                                                                                   
    Lw(rs,rt,imm);
    else if(op== 0x2B)
    Sw(rs,rt,imm);
    else if(op== 0x04)
    Beq(rs,rt,imm);
}

//判断指令类型
void System::JudgeInstruction(const bitset<32>& code)
{
    string machinecode=code.to_string();
    int op = code.to_ulong() >> 26;
    if (op == 0) //R型指令 ADD,SUB,SLL,AND,OR,JR
    {
        InstructionRType(machinecode);      
    } 
    else if (op >> 1 == 1 || op >> 2 == 0b100) //J型指令 J
    {
        InstructionJType(machinecode);  
    } 
    else //I型指令 ADDI LW SW BEQ
    {
        InstructionIType(machinecode);  
    }
}

void System::PcAutoAdd()
{
    int address=PC.Getvalue().to_ulong()+4;
    bitset<32> addr(address);
    PC.value = addr;
}

void System::Add(int rs, int rt, int rd)
{
    unsigned long temp = FindRegister(rs).value.to_ulong() + FindRegister(rt).value.to_ulong();
    FindRegister(rd).value = bitset<32>{ temp };
    PcAutoAdd();
}
void System::Sub(int rs, int rt, int rd)
{
    unsigned long temp = FindRegister(rs).value.to_ulong() + (~FindRegister(rt).value).to_ulong() + 1;
    FindRegister(rd).value = bitset<32>{ temp };
    PcAutoAdd();
}
void System::Sll(int rt,int rd,int shamt)
{
    FindRegister(rd).value = FindRegister(rt).value << shamt;
    PcAutoAdd();
}
void System::And(int rs, int rt, int rd)
{
    FindRegister(rd).value = FindRegister(rs).value & FindRegister(rt).value;
    PcAutoAdd();
}
void System::Or(int rs, int rt, int rd)
{
    FindRegister(rd).value = FindRegister(rs).value | FindRegister(rt).value;
    PcAutoAdd();
}
void System::Addi(int rs, int rt, int imm)
{
    int temp = complement2int(FindRegister(rs).value) + imm;
    FindRegister(rt).value = temp >= 0 ? bitset<32>(temp) : bitset<32>{ (~bitset<32>(-temp)).to_ulong() + 1 };
    PcAutoAdd();
}

void System::Jr(int rs)//跳转到指定寄存器存储的地址
{
    PC.value=FindRegister(rs).Getvalue();
}
void System::Lw(int rs, int rt, int offset)//lw rt rs offset, rs 加载到rt+offset
//rs 表示要加载的数据的内存地址所在的寄存器编号，rt 表示目标寄存器编号，offset 表示要加载的数据在内存中的偏移量。
{
    int address=FindRegister(rs).Getvalue().to_ulong()+offset;
    bitset<32> addr(address);
    FindRegister(rt).value=AccessMemory(addr);
    PcAutoAdd();
}
void System::Sw(int rs, int rt, int offset)//sw rt rs offset rt存到rs+offset:给目标地址赋值
{
    int address=FindRegister(rs).Getvalue().to_ulong()+offset;
    bitset<32> addr(address);
    if (addr.to_ulong()<registers.at(29).value.to_ulong() && addr.to_ulong() > mem.datatop.to_ulong())
    {
        mem.datatop = bitset<32>( address + 4 );
    }
    AccessMemory(addr) = FindRegister(rt).Getvalue();
    PcAutoAdd();
}
void System::Beq(int rs, int rt, int offset)//beq rt rs offset 如果rt值=rs值，跳转到PC+offset
{
    if(FindRegister(rs).Getvalue()==FindRegister(rt).Getvalue())
    {
        int address=PC.Getvalue().to_ulong()+offset;
        bitset<32> addr(address);
        PC.value = addr;
    }
    else
    PcAutoAdd();
}

//整数转原码
std::bitset<32> turnIntToTruecode(int number)
{
    bitset<32> num(number);
    if (number < 0) {
        num = num.flip();
        for (int i = 0; i < 31; ++i) {
            num[i] = !num[i];
            if (num[i] == 1) {
                break;
            }
        }
        num[31]=1;
    }
    return num;
}

//整数转补码
std::bitset<32> turnIntToComplementcode(int number)
{
    bitset<32> num(number);
    return num;
}

//补码转整数
int turnComplementToInt(bitset<32> complement)
{
    int num=static_cast<int>(complement.to_ulong());
    return num;
}

//浮点数转原码
std::bitset<32> turnFloatToTruecode(float number)
{
    unsigned int intValue = *reinterpret_cast<unsigned int*>(&number);
    return bitset<32>(intValue);
}

//浮点数转单精度表示
std::bitset<32> turnFloatToComplementcode(float number)
{
    if(number>=0)
    return turnFloatToTruecode(number);
    else
    {
        bitset<32> num = turnFloatToTruecode(number);
        num = num.flip();
        for (int i = 0; i < 31; ++i) {
            num[i] = !num[i];
            if (num[i] == 1) {
                break;
            }
        }
        num[31]=1;
        return num;
    }

}

//浮点数转双精度表示
std::bitset<64> turnDoubleToComplementcode(double number)
{
    return std::bitset<64>(*reinterpret_cast<long long*>(&number));
}

//单精度转浮点数表示
float turnComplementToFloat(bitset<32> complement)
{
    unsigned int num = complement.to_ulong();
    float result;
    std::memcpy(&result, &num, sizeof(float));
    return result;
}
//双精度转浮点数表示
double turnComplementToDouble(bitset<64> complement)
{
    unsigned long long num = complement.to_ullong();
    double result;
    std::memcpy(&result, &num, sizeof(double));
    return result;
}

int getPriority(char op) {
    if (op == '+' || op == '-') {
        return 1;
    } else if (op == '*' || op == '/') {
        return 2;
    } else {
        return 0;
    }
}

std::vector<std::string> toRPN(std::string s) {
    std::map<char, int> priority;
    priority['+'] = 1;
    priority['-'] = 1;
    priority['*'] = 2;
    priority['/'] = 2;
    std::stack<char> stack;
    std::vector<std::string> rpn;
    std::string number;

    if (s[0] == '-') {
        number += s[0];
    }

    for (int i = (s[0] == '-') ? 1 : 0; i < s.size(); ++i) {
        if (isdigit(s[i]) || s[i] == '.') {
            number += s[i];
        } else {
            if (s[i] == '-' && s[i - 1] != ')' && !isdigit(s[i - 1])) {
                number += s[i];
                continue;
            }
            if (!number.empty()) {
                rpn.push_back(number);
                number = "";
            }
            while (!stack.empty() && getPriority(s[i]) <= getPriority(stack.top())) {
                rpn.push_back(std::string(1, stack.top()));
                stack.pop();
            }
            stack.push(s[i]);
        }
    }
    if (!number.empty()) {
        rpn.push_back(number);
    }
    while (!stack.empty()) {
        rpn.push_back(std::string(1, stack.top()));
        stack.pop();
    }
    return rpn;
}

double calculateRPN(std::vector<std::string> rpn) {
    std::stack<double> stack;
    for (const auto &s : rpn) {
        if (isdigit(s[0]) || s[0] == '-' || s.size() > 1) {
            stack.push(stod(s));
        } else {
            double right = stack.top();
            stack.pop();
            double left = stack.top();
            stack.pop();
            switch (s[0]) {
                case '+': stack.push(left + right); break;
                case '-': stack.push(left - right); break;
                case '*': stack.push(left * right); break;
                case '/': stack.push(left / right); break;
            }
        }
    }
    return stack.top();
}