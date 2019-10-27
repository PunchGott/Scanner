#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include "../namespace.h"

#include <QAbstractTableModel>
#include <QVector>

#include <QDebug>

class TableModel : public QAbstractTableModel
{
public:
    explicit TableModel(QObject *parent = nullptr);
    virtual ~TableModel() override;

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QVector<QVariant> Row(const int row) const;

    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    bool addLine(const QStringList &);

    bool computeRest(const QStringList &);
    void computeTotal();

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;


private:
    bool repeatLine(const QStringList &);
    void computePrise(const QModelIndex &left, const QModelIndex &right);
    explicit TableModel(const TableModel&);
    TableModel& operator=(const TableModel&);

    QVector<QVector <QVariant> > m_cells;
    QMap <int, QVariant> m_total;
};

#endif // TABLEMODEL_H
