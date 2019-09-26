#include "tablemodel.h"

#define HEADERS_SIZE 6

TableModel::TableModel(QObject *parent)
    : QAbstractTableModel {parent},
      m_cells{1, QVector<QVariant>(m_headersSize)}
{
//    qDebug() << m_cells.at(0) << m_cells.size() << m_cells.first().size();

}

TableModel::~TableModel()
{

}

QModelIndex TableModel::index(int row, int column, const QModelIndex &/*parent*/) const
{
    QModelIndex index = this->createIndex(row, column);
    return index;
//    return QModelIndex(row,column,parent,this);
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
    return index.isValid() ? (flags | Qt::ItemIsEditable) : flags;
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
        switch (section) {
        case CODE: return QVariant("Code");
        case ARTICLE: return QVariant("Article");
        case NAME: return QVariant("Name");
        case PRISE: return QVariant("Prise");
        case EAN: return QVariant("EAN");
        case REST: return QVariant("Rest");
        }

    if (role == Qt::DisplayRole && orientation == Qt::Vertical)
        return QVariant(section); // For numbering since 1

    return QVariant();

}

int TableModel::rowCount(const QModelIndex &/*parent*/) const
{
    return m_cells.size();
}

int TableModel::columnCount(const QModelIndex &/*parent*/) const
{
    return HEADERS_SIZE;
//    return m_cells.empty() ? 0 : m_cells.first().size();
}

bool TableModel::insertRows(int row, int count, const QModelIndex &/*parent*/)
{
    if ( (row >= 0 && row <= m_cells.size()) && (count >= 0 && count <= m_cells.first().size()) ) {
    beginInsertRows(QModelIndex(), row, row);
    for (int i = 0; i < count; ++i)
        m_cells.push_back(QVector<QVariant>(HEADERS_SIZE));
    endInsertRows();
    return true;
    }
    return false;
}

//bool TableModel::insertColumns(int column, int count, const QModelIndex &parent)
//{

//}

//QVariant TableModel::computeTotal(int row, const QVariant &value, const QModelIndex &parent)
//{
//    return QVariant();
//}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if (index.row() >= m_cells.size() || index.row() < 0 ||
            index.column() >= m_cells.first().size() || index.column() < 0)
        return QVariant();

    if(role == Qt::DisplayRole || role == Qt::EditRole)
        return m_cells[index.row()][index.column()];
    else
        return QVariant();
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        m_cells[index.row()][index.column()] = value;
        return true;
    }
        return false;
}
