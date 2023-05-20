#include "memorytablemodel.h"

using namespace std;

extern System sys;

void MemoryTableModel::initTable() {
    beginResetModel();
    try {
        table.clear();
        table.append({"地址", "值(+0)", "值(+4)", "值(+8)", "值(+c)", "值(+10)", "值(+14)", "值(+18)", "值(+1c)"});
        for (unsigned int i = addr; i <= addr + 0x100; i += 0x20) {
            QVector<QString> row;
            stringstream ss;
            ss << hex << i;
            row.append(QString::fromStdString("0x" + ss.str()));
            for (int j = 0; j < 8; j++) {
                bitset<32> value = sys.ReadMemory(bitset<32>(i + j * 4));
                stringstream ss;
                ss << hex << value.to_ulong();
                row.append(QString::fromStdString("0x" + ss.str()));
            }
            table.append(row);
        }
    } catch (exception &e) {
        emit fail(QString{e.what()});
    }
    endResetModel();
}

void MemoryTableModel::setDisplayMemory(int id) {
    switch (id) {
        case 0:
            addr = 0x7ffffee0;
            break;
        case 1:
            addr = 0x10000000;
            break;
        case 2:
            addr = 0x00400000;
            break;
        default:
            break;
    }
    initTable();
}