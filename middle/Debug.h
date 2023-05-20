#ifndef DEBUG_H
#define DEBUG_H

#include <QObject>
#include <QUrl>
#include <QFile>
#include "../back/assembler.h"
#include "main.h"

class Debug : public QObject {
    Q_OBJECT
   public:
    explicit Debug(QObject *parent = nullptr);
    Q_INVOKABLE void assemble(const QString &text, const QString &fileName);
   signals:
    void success();
    void fail(const QString &err);
};

#endif  // DEBUG_H
