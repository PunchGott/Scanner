#ifndef NAMESPACE_H
#define NAMESPACE_H

#include <QtGlobal>

namespace Scanner {
const quint8 g_headers_size = 8;
const quint8 g_EAN_size = 13; // typical size for EAN (EAN13)

enum Headers : quint8
{ // since 0 to 15
    CODE = 0,
    ARTICLE = 1,
    NAME = 2,
    PRISE,
    REST, // Остаток
    EAN,
    AMOUNT,
    SUM

};



enum Mode : quint8
{
    RECEIVING = 16, // Получение товара
    SHIPMENT = 17// Отгрузка товара
};

}

#endif // NAMESPACE_H
