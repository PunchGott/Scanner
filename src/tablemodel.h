#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QDebug>
#include <QAbstractTableModel>
#include <QVector>

class TableModel : public QAbstractTableModel
{
public:
    TableModel(QObject *parent = nullptr);
    virtual ~TableModel() override;

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
//    virtual bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    QVariant addCount();
    QVariant removeCount();

    QVariant computeTotal(int row, const QVariant &value, const QModelIndex &parent = QModelIndex());

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;


private:
//    int m_rows, m_cols;

    enum Headers
    {
        CODE = 0,
        ARTICLE,
        NAME,
        PRISE,
        EAN,
        REST // Остаток
    };
    int m_headersSize = 6;
    QVector<QVector <QVariant> > m_cells;
    QMap <int, QVariant> m_total;
};

#endif // TABLEMODEL_H
