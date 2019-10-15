#ifndef RECEIVINGMODE_H
#define RECEIVINGMODE_H

#include "abstractmode.h"

class ReceivingMode : public AbstractMode
{
    Q_OBJECT
public:
    explicit ReceivingMode(QObject *parent = nullptr);
//    virtual bool makeVBA() override;

signals:

private:

public slots:
};

#endif // RECEIVINGMODE_H
