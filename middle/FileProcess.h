#ifndef FILEPROCESS_H
#define FILEPROCESS_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QUrl>

class FileProcess : public QObject {
    Q_OBJECT
   public:
    explicit FileProcess(QObject *parent = nullptr);
    Q_INVOKABLE void openFile(const QString &fileName);
    Q_INVOKABLE void saveFile(const QString &fileName, const QString &text);
    Q_INVOKABLE QString getFileName(const QString &fileName);
   signals:
    void openFileSuccess(const QString &text);
    void saveFileSuccess();
    void fail(const QString &err);
};

#endif  // FILEPROCESS_H
