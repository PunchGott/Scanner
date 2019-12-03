#pragma once

#include "../namespace.h"
#include "../tablemodel/tablemodel.h"

#include <QWidget>
#include <QMessageBox>
#include <QCloseEvent>
#include <QMenuBar>
#include <QMenu>
#include <QLabel>
#include <QStatusBar>
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
    virtual ~BaseMode() override;

protected slots:
    virtual bool makeVBA() = 0;

protected:
    virtual bool computeRest() = 0;
    virtual void closeEvent(QCloseEvent *event) override;

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
//    void closeProgram();


private:
    BaseMode(const BaseMode&);
    BaseMode& operator=(const BaseMode&);
    bool readFile(const QString &EAN);
    bool convertPrise(QStringList &);

    QWidget *m_mainWidget;
    QLabel *m_choiceFileLbl, *m_searchLbl, *m_statusBarLbl;
    QPushButton *m_choiceFilePB, *m_crossImgPB;
    QLineEdit *m_searchEANLE;
    QStatusBar *statusBar;
    QHBoxLayout *m_choiceFileLayout, *m_infoLayout; // For other PushButtons
    QVBoxLayout *m_mainLayout;
};
