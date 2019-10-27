#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../namespace.h"
#include "../tablemodel/tablemodel.h"

#include <QWidget>
#include <QMenuBar>
#include <QMenu>
#include <QLabel>
#include <QRegExpValidator>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QTableView>

class BaseMode : public QWidget
{
    Q_OBJECT

public:
    explicit BaseMode(QWidget *parent = nullptr);
    virtual ~BaseMode();

protected slots:
    virtual bool makeVBA() = 0;

protected:
    virtual bool changeFile(QStringList& lineList);
    virtual bool changeModel(QStringList& lineList);

    virtual bool computeRest() = 0;

    TableModel *model;
    QTableView *table;
    QFile CSVFile;
    QByteArray m_fileContent;
    QString m_fileName;

private slots:
    void saveChanges();
    void selectFile();
    void inputEAN(const QString &EAN);
    void clearTable();
    bool closeProgram();


private:
    bool readFile(const QString &EAN);
    bool convertPrise(QStringList &);

    QWidget *m_mainWidget;
    QLabel *m_choiceFileLbl, *m_searchLbl;
    QPushButton *m_choiceFilePB, *m_crossImgPB;
    QLineEdit *m_inputEANLE;
    QHBoxLayout *m_choiceFileLayout, *m_infoLayout; // For other PushButtons
    QVBoxLayout *m_mainLayout;
};

#endif // MAINWINDOW_H
