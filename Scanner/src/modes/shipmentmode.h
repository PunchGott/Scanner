#ifndef SHIPMENTMODE_H
#define SHIPMENTMODE_H

#include "basemode.h"

class ShipmentMode : public BaseMode
{
    Q_OBJECT

public:
    explicit ShipmentMode(QObject *parent = nullptr);
    virtual ~ShipmentMode() override;

private:
    virtual bool computeRest() override;

    virtual bool makeVBA() override;
};

#endif // SHIPMENTMODE_H
