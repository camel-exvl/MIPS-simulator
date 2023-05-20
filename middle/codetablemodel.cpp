#include "codetablemodel.h"

extern System sys;

void CodeTableModel::initTableFromBinFile(const QString &fileName) {
    beginResetModel();
    try {
        sys.Reset();
        QUrl url{fileName};
        QFile file{url.toLocalFile()};
        if (!file.exists()) {
            throw std::runtime_error{"请先编译文件"};
        }
        if (!file.open(QIODevice::ReadOnly)) {
            throw std::runtime_error{"无法打开文件"};
        }
        std::vector<std::pair<std::string, std::string>> originCode =
            disassemblerOpenFile(sys, url.toLocalFile().toStdString());
        QVector<std::pair<QString, QString>> code;
        for (auto &i : originCode) {
            std::string machineCodeString = i.first.substr(2, i.first.size() - 2);
            std::bitset<32> machineCode{std::stoul(machineCodeString, nullptr, 16)};
            sys.PushCodeToMemory(machineCode);
            code.push_back({QString::fromStdString(i.first), QString::fromStdString(i.second)});
        }

        table.clear();
        table.append({{QVariant{"断点"}, PropertyType::String},
                      {QVariant{"地址"}, PropertyType::String},
                      {QVariant{"机器码"}, PropertyType::String},
                      {QVariant{"反汇编"}, PropertyType::String}});
        int PC = 0x00400000;
        for (auto &i : code) {
            table.append({{QVariant{false}, PropertyType::Boolean},
                          {QVariant{"0x" + QString::number(PC, 16)}, PropertyType::String},
                          {QVariant{i.first}, PropertyType::String},
                          {QVariant{i.second}, PropertyType::String}});
            PC += 4;
        }
        breakpoint.resize(table.size());
        emit success();
    } catch (std::exception &e) {
        emit fail(QString{e.what()});
    }
    endResetModel();
}

void CodeTableModel::setBreakpoint(int row, bool value) {
    try {
        breakpoint[row] = value;
        std::string addressString = table[row][1].value.toString().toStdString();
        addressString = addressString.substr(2, addressString.size() - 2);
        std::bitset<32> address{std::stoul(addressString, nullptr, 16)};
        if (value) {
            sys.AddBreakPoint(address);
        } else {
            sys.RemoveBreakPoint(address);
        }
        emit success();
    } catch (std::exception &e) {
        emit fail(QString{e.what()});
    }
}

void CodeTableModel::executeToNextBreakpoint() {
    try {
        std::bitset<32> address = sys.BreakPointExecute();
        QString targetAddress = "0x" + QString::number(address.to_ulong(), 16);
        for (int i = 0; i < table.size(); ++i) {
            if (table[i][1].value.toString() == targetAddress) {
                emit successExecute(i);
                return;
            }
        }
        emit successExecute(-1);
    } catch (std::exception &e) {
        emit fail(QString{e.what()});
    }
}

void CodeTableModel::executeOneStep() {
    try {
        std::bitset<32> address = sys.OneStepExecute();
        QString targetAddress = "0x" + QString::number(address.to_ulong(), 16);
        for (int i = 0; i < table.size(); ++i) {
            if (table[i][1].value.toString() == targetAddress) {
                emit successExecute(i);
                return;
            }
        }
        emit successExecute(-1);
    } catch (std::exception &e) {
        emit fail(QString{e.what()});
    }
}