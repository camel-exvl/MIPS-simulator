#include "memorytablemodel.h"

using namespace std;

void MemoryTableModel::initTable(System sys) {
    beginResetModel();
    try {
        table.clear();
        table.append({"地址", "值(+0)", "值(+4)", "值(+8)", "值(+c)", "值(+10)", "值(+14)", "值(+18)", "值(+1c)"});
        for (int i = 0x10010000; i <= 0x100101e0; i += 0x20) {
            QVector<QString> row;
            row.append(QString::fromStdString("0x" + to_string(i)));
            for (int j = 0; j < 8; j++) {
                bitset<32> value = sys.AccessMemory(bitset<32>(i + j * 4));
                stringstream ss;
                ss << hex << value.to_ulong();
                row.append(QString::fromStdString("0x" + ss.str()));
            }
            table.append(row);
        }
    } catch (exception &e) {
        cerr << e.what() << endl;
    }
    endResetModel();
}