#include "tablemodel.h"

#define DEBUG
// (Заполнить эту колонку в самом .csv файле)

TableModel::TableModel(QObject *parent)
    : QAbstractTableModel {parent},
      m_cells{1, QVector<QVariant>(HEADERS_SIZE)} // почему создается две строки?
{
//    qDebug() << m_cells.size();
    m_cells.push_back(QVector<QVariant>(HEADERS_SIZE));
    m_cells[rowCount() - 1][0] = "Итого: ";

}

TableModel::~TableModel()
{

}

QModelIndex TableModel::index(int row, int column, const QModelIndex &/*parent*/) const
{
    if (row >= 0 && row < rowCount() && column >= 0 && column < columnCount()) {
    QModelIndex index = createIndex(row, column);
    return index;
    }
    else
        return QModelIndex();
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
        case Scanner::CODE: return QVariant("Код");
        case Scanner::ARTICLE: return QVariant("Артикул");
        case Scanner::NAME: return QVariant("Наименование");
        case Scanner::PRISE: return QVariant("Цена");
        case Scanner::AMOUNT: return QVariant("Кол-во");
        case Scanner::SUM: return QVariant("Сумма");
        case Scanner::REST: return QVariant("Остаток");
        case Scanner::EAN: return QVariant("EAN");
        }

    if (role == Qt::DisplayRole && orientation == Qt::Vertical)
        return QVariant(section + 1); // For numbering since 1

    return QVariant();

}

int TableModel::rowCount(const QModelIndex &/*parent*/) const
{
    return m_cells.size();
}

int TableModel::columnCount(const QModelIndex &/*parent*/) const
{
    return m_cells.empty() ? 0 : m_cells.first().size();
}

bool TableModel::insertRows(int row, int count, const QModelIndex &/*parent*/)
{
    if ( (row >= 0 && row < m_cells.size()) && (count > 0 && count < m_cells.first().size()) ) {
    beginInsertRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i)
        m_cells.insert(row, QVector<QVariant>(HEADERS_SIZE));
    endInsertRows();
    return true;
    }
    else
        return false;
}

bool TableModel:: removeRows(int row, int count, const QModelIndex &/*parent*/) // !!!
{
    if ( (row >= 0 && row < m_cells.size()) && (count > 0 && count < m_cells.first().size()) ) {
        beginRemoveRows(QModelIndex(), row, row + count - 1);
        for (int i = 0; i < count; ++i)
            m_cells.remove(row);
        endRemoveRows();
        return true;
        }
    else
        return false;
}

bool TableModel::computeRest(const QStringList &lineList)
{
    if (lineList.at(Scanner::REST) == '0' || lineList.at(Scanner::REST).isNull()) {
        qDebug() << "Товара нет в наличии";
        return false;
    }
    else {

        return true;
    }
}

// checks if there is already such a row in the table
bool TableModel::repeatLine(const QStringList &lineList)
{
    QVariant EAN = QVariant(lineList.at(Scanner::EAN));

#ifdef DEBUG
    qDebug() << __LINE__ << " "  << __FILE__ << ": " << lineList.at(Scanner::EAN);
#endif

    for (int i = 0; i < m_cells.size(); ++i) {
        if (m_cells.at(i).at(Scanner::EAN) == EAN) {
            m_cells[i][Scanner::AMOUNT] = QVariant(m_cells.at(i).at(Scanner::AMOUNT).toInt() + 1);
            emit dataChanged(index(i,0), index(i,HEADERS_SIZE - 1));
            computePrise(index(i,0), index(i,6));
            return true;
        }
    }
    return false;
}

//this function add a line in begin of m_cells's table if no such line in current table
bool TableModel::addLine(const QStringList &lineList)
{
    if (!repeatLine(lineList) && insertRows(0,1)) {
    for (int i = 0; i < lineList.size(); ++i) {
        QModelIndex modelIndex = index(0,i);
        setData(modelIndex, QVariant(lineList.at(i)), Qt::EditRole);
    }
    m_cells[0][Scanner::AMOUNT] = QVariant(1);
    m_cells[0][Scanner::SUM] = QVariant(m_cells.at(0).at(Scanner::PRISE));
    emit dataChanged(QModelIndex(), QModelIndex());
    return true;
    }
    else
        return false;
}

void TableModel::computePrise(const QModelIndex &left, const QModelIndex &right)
{
    m_cells[left.row()][Scanner::SUM] =
            QVariant (m_cells.at(left.row()).at(Scanner::PRISE).toDouble() *
            m_cells.at(right.row()).at(Scanner::AMOUNT).toInt());
}

void TableModel::computeTotal()
{
    double sum = 0;
    for (int i = 0; i < m_cells.size() && !m_cells[i][Scanner::SUM].isNull(); ++i)
        sum += m_cells.at(i).at(Scanner::SUM).toDouble();
    m_cells[rowCount() - 1][Scanner::SUM] = QVariant(sum);
    emit dataChanged(QModelIndex(), QModelIndex());
}

QVariant TableModel::data(const QModelIndex &index, int role) const // Переписать!
{
    if(!index.isValid())
        return QVariant();

    if (index.row() >= m_cells.size() || index.row() < 0 ||
            index.column() >= m_cells.first().size() || index.column() < 0)
        return QVariant();

    if(role == Qt::DisplayRole || role == Qt::EditRole)
        return m_cells.at(index.row()).at(index.column());
    else
        return QVariant();
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        m_cells[index.row()][index.column()] = value;
        dataChanged(QModelIndex(),QModelIndex());
        return true;
    }
        return false;
}
