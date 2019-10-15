#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <tablemodel.h>

#include <QWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QLabel>
#include <QRegExpValidator>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QKeyEvent>

#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QTableView>
#include <QItemDelegate>

class AbstractMode : public QWidget
{
    Q_OBJECT

public:
    explicit AbstractMode(QWidget *parent = nullptr);
    ~AbstractMode();

public slots:
    void selectFile();
    void inputEAN(const QString &EAN);
    void clearTable();
public:
    virtual bool readFile(const QString &EAN);
    virtual bool convertPrise(QStringList &);

    virtual bool changeFile(QStringList& lineList, int role);
    virtual bool changeModel(QStringList& lineList, int role);

//    virtual bool makeVBA();

    TableModel *model;
    QTableView *table;
    QFile CSVFile;
    QString m_fileName;

    QWidget *m_mainWidget;
    QComboBox *m_choiceModeComboBox;
    QLabel *m_choiceFileLbl, *m_searchLbl;
    QPushButton *m_choiceFilePB, *m_crossImgPB;
    QLineEdit *m_inputEANLE;
    QHBoxLayout *m_choiceFileLayout, *m_infoLayout; // For other PushButtons
    QVBoxLayout *m_mainLayout;
};

#endif // MAINWINDOW_H
