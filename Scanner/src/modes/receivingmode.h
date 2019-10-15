#ifndef RECEIVINGMODE_H
#define RECEIVINGMODE_H

#include "basemode.h"

class ReceivingMode : public BaseMode
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
