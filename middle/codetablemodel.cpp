#include "codetablemodel.h"

void CodeTableModel::initTableFromBinFile(System sys, const QString &fileName) {
    beginResetModel();
    try {
        QUrl url{fileName};
        QFile file{url.toLocalFile()};
        if (!file.exists()) {
            throw std::runtime_error{"请先编译文件"};
        }
        if (!file.open(QIODevice::ReadOnly)) {
            throw std::runtime_error{"无法打开文件"};
        }
        std::vector<std::pair<std::string, std::string>> originCode =
            disassemblerOpenFile(sys, url.toLocalFile().toStdString());
        QVector<std::pair<QString, QString>> code;
        for (auto &i : originCode) {
            code.push_back({QString::fromStdString(i.first), QString::fromStdString(i.second)});
        }

        table.clear();
        table.append({{QVariant{"断点"}, PropertyType::String},
                      {QVariant{"地址"}, PropertyType::String},
                      {QVariant{"机器码"}, PropertyType::String},
                      {QVariant{"反汇编"}, PropertyType::String}});
        int PC = 0x00400000;
        for (auto &i : code) {
            table.append({{QVariant{false}, PropertyType::Boolean},
                          {QVariant{"0x" + QString::number(PC, 16)}, PropertyType::String},
                          {QVariant{i.first}, PropertyType::String},
                          {QVariant{i.second}, PropertyType::String}});
            PC += 4;
        }
        emit success();
    } catch (std::exception &e) {
        emit fail(QString{e.what()});
    }
    endResetModel();
}
