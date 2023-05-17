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
            int number = turnComplementToInt(complementcode);
            emit successIntCalculate(number, text);
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

void Tools::calculate(const QString &text) {
    try {
        double result = calculateRPN(toRPN(text.toStdString()));
        emit successCalculate(result);
    } catch (exception &e) {
        emit fail(QString{e.what()});
    }
}