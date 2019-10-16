#include "startwindow.h"

StartWindow::StartWindow(QWidget *parent) : QWidget(parent)
{
    m_startWgt = new QWidget();
    m_startWgt->setFixedSize(600,160);

    m_receivingPB = new QPushButton("Получение товара");
    m_receivingPB->setObjectName("start_PB");
    m_shipmentPB = new QPushButton("Отгрузка товара");
    m_shipmentPB->setObjectName("start_PB");

    m_startLayout = new QVBoxLayout(m_startWgt);
    m_startLayout->addWidget(m_receivingPB);
    m_startLayout->addWidget(m_shipmentPB);

    connect(m_receivingPB, SIGNAL(clicked()), SLOT(click_on_receivingPB()));
    connect(m_shipmentPB, SIGNAL(clicked()), SLOT(click_on_shipmentPB()));

    m_startWgt->show();
}

void StartWindow::click_on_receivingPB()
{
     QPointer<ReceivingMode> receiving = new ReceivingMode();
     m_startWgt->close();
}

void StartWindow::click_on_shipmentPB()
{
    QPointer<ShipmentMode> shipmentmode = new ShipmentMode();
    m_startWgt->close();
}
