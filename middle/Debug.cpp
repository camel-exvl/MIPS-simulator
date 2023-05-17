#include "Debug.h"

using namespace std;

extern System sys;

Debug::Debug(QObject *parent) : QObject{parent} {}

void Debug::assemble(const QString &text, const QString &fileName) {
    try {
        QUrl url{fileName};
        QFile file{url.toLocalFile()};
        vector<string> s;
        for (const auto &i : text.split('\n')) {
            s.push_back(i.toStdString());
        }
        QString newFileName = file.fileName().replace(".asm", ".bin");
        assemblerToFile(sys, s, newFileName.toStdString());
        emit success();
    } catch (exception &e) {
        emit fail(QString{e.what()});
    }
}