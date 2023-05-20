#ifndef MEMORYTABLEMODEL_H
#define MEMORYTABLEMODEL_H

#include <QAbstractTableModel>
#include "main.h"
#include <iostream>
#include <sstream>

class MemoryTableModel : public QAbstractTableModel {
    Q_OBJECT
   public:
    enum CustomRoles { ValueRole = Qt::UserRole + 1 };
    explicit MemoryTableModel(QObject *parent = nullptr) : QAbstractTableModel(parent) {
        beginResetModel();
        table.clear();
        table.append({"地址", "值(+0)", "值(+4)", "值(+8)", "值(+c)", "值(+10)", "值(+14)", "值(+18)", "值(+1c)"});
        endResetModel();
    }
    int rowCount(const QModelIndex & = QModelIndex()) const override { return table.size(); }

    int columnCount(const QModelIndex & = QModelIndex()) const override { return table[0].size(); }

    QVariant data(const QModelIndex &index, int role) const override {
        auto &property = table[index.row()][index.column()];
        switch (role) {
            case CustomRoles::ValueRole:
                return property;
            default:
                break;
        }
        return QVariant();
    }

    QHash<int, QByteArray> roleNames() const override {
        QHash<int, QByteArray> roles;
        roles[ValueRole] = "value";
        return roles;
    }

    Q_INVOKABLE void initTable();
    Q_INVOKABLE void setDisplayMemory(int id);

   private:
    QVector<QVector<QString>> table;
    unsigned int addr = 0x7ffffee0;
   signals:
    void fail(QString);
};

#endif  // MEMORYTABLEMODEL_H
