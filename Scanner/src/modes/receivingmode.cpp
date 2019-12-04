#include "receivingmode.h"

ReceivingMode::ReceivingMode(QObject */*parent*/) : BaseMode()
{

}

ReceivingMode::~ReceivingMode()
{

}

bool ReceivingMode::computeRest()
{
    QModelIndex index = model->index(0,Scanner::REST);
    if (index.data().toInt() >= 0) {
        int temp = index.data().toInt();
    model->setData(index, ++temp, Qt::EditRole);
    return true;
    }
    else
        return false;
}

bool ReceivingMode::makeVBA()
{
    return false;
}

