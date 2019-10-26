#include "shipmentmode.h"

ShipmentMode::ShipmentMode(QObject */*parent*/) : BaseMode()
{

}

ShipmentMode::~ShipmentMode()
{

}

bool ShipmentMode::computeRest()
{
    QModelIndex index = model->index(0,Scanner::REST);
    if (index.data().toInt() > 0) {
        int temp = index.data().toInt();
    model->setData(index, --temp, Qt::EditRole);
    return true;
    }
    else
        return false;
}

bool ShipmentMode::writeInFile(const QString &/*line*/)
{
    return false;
}

bool ShipmentMode::makeVBA()
{
    return false;
}
