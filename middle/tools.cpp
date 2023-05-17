#include "tools.h"

using namespace std;

Tools::Tools(QObject *parent) : QObject{parent} {}

void Tools::intCalculate(int mode, const QString &text) {
    try {
        if (mode == 0) {
            int number = text.toInt();
            bitset<32> truecode = turnIntToComplementcode(number);
            emit successIntCalculate(number, QString::fromStdString(truecode.to_string()));
        } else if (mode == 1) {
            bitset<32> complementcode{text.toStdString()};
            // TODO:
            // int number = complement2int(complementcode);
            // emit successIntCalculate(QString::number(number));
        } else {
            throw runtime_error{"mode参数错误"};
        }
    } catch (exception &e) {
        emit fail(QString{e.what()});
    }
}

void Tools::floatCalculate(int mode, const QString &text) {
    try {
        // TODO:
    } catch (exception &e) {
        emit fail(QString{e.what()});
    }
}