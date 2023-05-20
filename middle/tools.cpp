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
        if (mode == 0) {
            double number = text.toDouble();
            bitset<32> floatCode = turnFloatToComplementcode(number);
            bitset<64> doubleCode = turnDoubleToComplementcode(number);
            emit successFloatCalculate(number, QString::fromStdString(floatCode.to_string()),
                                       QString::fromStdString(doubleCode.to_string()));
        } else if (mode == 1) {
            bitset<32> floatCode{text.toStdString()};
            float number = turnComplementToFloat(floatCode);
            bitset<64> doubleCode = turnDoubleToComplementcode(number);
            emit successFloatCalculate(number, text, QString::fromStdString(doubleCode.to_string()));
        } else if (mode == 2) {
            bitset<64> doubleCode{text.toStdString()};
            double number = turnComplementToDouble(doubleCode);
            bitset<32> floatCode = turnFloatToComplementcode(number);
            emit successFloatCalculate(number, QString::fromStdString(floatCode.to_string()), text);
        } else {
            throw runtime_error{"mode参数错误"};
        }
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