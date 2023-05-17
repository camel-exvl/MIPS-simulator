#ifndef CODETABLEMODEL_H
#define CODETABLEMODEL_H

#include <QAbstractTableModel>
#include <QFile>
#include <QUrl>
#include "main.h"
#include "back/disassembler.h"

class CodeTableModel : public QAbstractTableModel {
    Q_OBJECT

   public:
    enum PropertyType { String, Boolean };
    Q_ENUM(PropertyType)
    struct Property {
        QVariant value;
        PropertyType type;
    };
    enum CustomRoles { ValueRole = Qt::UserRole + 1, TypeRole };

    explicit CodeTableModel(QObject *parent = nullptr) : QAbstractTableModel(parent) {
        table.append({{QVariant{"断点"}, PropertyType::String},
                      {QVariant{"地址"}, PropertyType::String},
                      {QVariant{"机器码"}, PropertyType::String},
                      {QVariant{"反汇编"}, PropertyType::String}});
    }

    int rowCount(const QModelIndex & = QModelIndex()) const override { return table.size(); }

    int columnCount(const QModelIndex & = QModelIndex()) const override { return table[0].size(); }

    QVariant data(const QModelIndex &index, int role) const override {
        auto &property = table[index.row()][index.column()];
        switch (role) {
            case CustomRoles::TypeRole:
                return property.type;
            case CustomRoles::ValueRole:
                return property.value;
            default:
                break;
        }
        return QVariant();
    }

    QHash<int, QByteArray> roleNames() const override {
        QHash<int, QByteArray> roles;
        roles[ValueRole] = "value";
        roles[TypeRole] = "type";
        return roles;
    }

    Q_INVOKABLE void initTableFromBinFile(System sys, const QString &fileName);
    Q_INVOKABLE void setBreakpoint(int row, bool value);

   private:
    QVector<bool> breakpoint;
    QVector<QVector<Property>> table;
   signals:
    void success();
    void fail(const QString &err);
};

#endif  // CODETABLEMODEL_H
