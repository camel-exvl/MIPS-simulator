#include "FileProcess.h"

FileProcess::FileProcess(QObject *parent) : QObject{parent} {}

void FileProcess::openFile(const QString &fileName) {
    try {
        QUrl url{fileName};
        QFile file{url.toLocalFile()};
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            throw std::runtime_error{"文件打开失败"};
        }
        QTextStream in{&file};
        QString text{in.readAll()};
        emit openFileSuccess(text);
    } catch (std::exception &e) {
        emit fail(QString{e.what()});
    }
}