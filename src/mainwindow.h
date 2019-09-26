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

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();

private slots:
    void selectFile();
    void inputEAN(const QString &EAN);
private:
    void readFile(const QString &EAN);
    void addLine(const QString &line);

    TableModel *model;
    QTableView *table;
    QString m_fileName;
    QRegExp m_EANRegExp;

    QWidget *m_mainWidget;
    QComboBox *m_choiceModeComboBox;
    QLabel *m_choiceFileLbl;
    QPushButton *m_choiceFilePB, *m_crossImgPB;
    QLineEdit *m_inputEANLE;
    QHBoxLayout *m_choiceFileLayout; // For other PushButtons
    QVBoxLayout *m_mainLayout;
};

#endif // MAINWINDOW_H
