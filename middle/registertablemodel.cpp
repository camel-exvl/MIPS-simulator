#include "registertablemodel.h"

using namespace std;

extern System sys;

void RegisterTableModel::initTable() {
    beginResetModel();
    try {
        table.clear();
        table.append({"寄存器", "编号", "值"});
        for (int i = 0; i < 32; i++) {
            bitset<32> value = sys.FindRegister(i).Getvalue();
            stringstream ss;
            ss << hex << value.to_ulong();
            table.append({QString::fromStdString(sys.FindRegister(i).GetName()), QString::number(i),
                          QString::fromStdString("0x" + ss.str())});
        }
    } catch (exception &e) {
        cerr << e.what() << endl;
    }
    endResetModel();
}
