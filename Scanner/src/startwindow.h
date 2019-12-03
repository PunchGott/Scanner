#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include "modes/receivingmode.h"
#include "modes/shipmentmode.h"

#include <QWidget>
#include <QPushButton>
#include <QPointer>
#include <QVBoxLayout>
#include <QPushButton>


class StartWindow : public QWidget
{
    Q_OBJECT
public:
    explicit StartWindow(QWidget *parent = nullptr);

signals:

public slots:

private slots:
    void click_on_receivingPB();
    void click_on_shipmentPB();

private:
    void closeEvent(QCloseEvent *event) override;

    QWidget *m_startWgt;
    QPushButton *m_receivingPB, *m_shipmentPB;

    QVBoxLayout *m_startLayout;

};

#endif // STARTWINDOW_H
