#ifndef REGISTERTABLEMODEL_H
#define REGISTERTABLEMODEL_H

#include <QAbstractTableModel>
#include "main.h"
#include <iostream>
#include <sstream>

class RegisterTableModel : public QAbstractTableModel {
    Q_OBJECT
   public:
    enum CustomRoles { ValueRole = Qt::UserRole + 1 };
    explicit RegisterTableModel(QObject *parent = nullptr) : QAbstractTableModel(parent) {
        table.append({"寄存器", "编号", "值"});
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

   private:
    QVector<QVector<QString>> table;

   signals:
    void fail(QString);
};

#endif  // REGISTERTABLEMODEL_H
