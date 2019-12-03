#ifndef RECEIVINGMODE_H
#define RECEIVINGMODE_H

#include "basemode.h"

class ReceivingMode : public BaseMode
{
    Q_OBJECT

public:
    explicit ReceivingMode(QObject *parent = nullptr);
    virtual ~ReceivingMode() override;

private:
    virtual bool computeRest() override;
    virtual void closeEvent(QCloseEvent *event) override;

    virtual bool makeVBA() override;
};

#endif // RECEIVINGMODE_H
