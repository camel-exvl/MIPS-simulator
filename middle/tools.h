#ifndef TOOLS_H
#define TOOLS_H

#include <QObject>
#include "back/systemsimulation.h"

class Tools : public QObject {
    Q_OBJECT
   public:
    explicit Tools(QObject *parent = nullptr);
    Q_INVOKABLE void intCalculate(int mode,
                                   const QString &text);  // mode = 0: true->complement; mode = 1: complement->true
    Q_INVOKABLE void floatCalculate(int mode,
                                     const QString &text);  // mode = 0: 10-floating; mode = 1: float; mode = 2: double
   signals:
    void success();
    void successIntCalculate(const int &integer, const QString &text);
    void fail(const QString &err);
};

#endif  // TOOLS_H
