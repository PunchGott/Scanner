#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../tablemodel/tablemodel.h"

#include <QWidget>
#include <QLabel>
#include <QRegExpValidator>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QTableView>

class BaseMode : public QWidget
{
    Q_OBJECT

public:
    explicit BaseMode(QWidget *parent = nullptr);
    virtual ~BaseMode();

protected slots:
    void selectFile();
    void inputEAN(const QString &EAN);
    void clearTable();


    virtual bool makeVBA() = 0;

protected:
    bool readFile(const QString &EAN);
    bool convertPrise(QStringList &);

    virtual bool changeFile(QStringList& lineList);
    virtual bool changeModel(QStringList& lineList);

    virtual bool computeRest() = 0;
    virtual bool writeInFile(const QString &line) = 0;

    TableModel *model;
    QTableView *table;
    QFile CSVFile;
    QString m_fileName;

    QWidget *m_mainWidget;
    QLabel *m_choiceFileLbl, *m_searchLbl;
    QPushButton *m_choiceFilePB, *m_crossImgPB;
    QLineEdit *m_inputEANLE;
    QHBoxLayout *m_choiceFileLayout, *m_infoLayout; // For other PushButtons
    QVBoxLayout *m_mainLayout;
};

#endif // MAINWINDOW_H
