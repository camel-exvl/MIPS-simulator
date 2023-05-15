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
    explicit MemoryTableModel(QObject *parent = nullptr) : QAbstractTableModel(parent) {}
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

    void initTable(System &sys);

   private:
    QVector<QVector<QString>> table;
};

#endif  // MEMORYTABLEMODEL_H
