#ifndef NAMESPACE_H
#define NAMESPACE_H

#define HEADERS_SIZE 9

namespace Scanner {

enum Headers // since 0 to 15
{
    CODE = 0,
    ARTICLE = 1,
    NAME = 2,
    PRISE = 3,
    AMOUNT = 4,
    SUM = 5,
    REST = 6, // Остаток
    EAN = 7
};

enum Mode
{
    RECEIVING = 16, // Получение товара
    SHIPMENT = 17// Отгрузка товара
};

}

#endif // NAMESPACE_H
