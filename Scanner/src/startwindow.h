#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include "receivingmode.h"

#include <QWidget>
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
    QWidget *m_startWgt;
    QPushButton *m_receivingPB, *m_shipmentPB;

    QVBoxLayout *m_startLayout;

};

#endif // STARTWINDOW_H
