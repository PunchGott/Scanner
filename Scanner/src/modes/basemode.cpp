#include "basemode.h"

#define DEBUG

// Добавить обработку пользовательских ошибок и вывод их в QDialog
// Добавить обработку логических и программных ошибок
// Добавить QMenuBar
// Добавть QStatusBar
// Добавить подсказки


BaseMode::BaseMode(QWidget */*parent*/)
{
    this->setFixedSize(1024,768);

    m_statusBarLbl = new QLabel("StatusBar");
    statusBar = new QStatusBar;
    statusBar->addWidget(m_statusBarLbl);


    QMenuBar *menuBar = new QMenuBar(this);
    QMenu *fileMenu = new QMenu(tr("&Файл"));
    QMenu *ViewMenu = new QMenu(tr("Вид"));
    menuBar->addMenu(fileMenu);
    menuBar->addMenu(ViewMenu);

    fileMenu->addAction(tr("&Открыть"), this, SLOT(selectFile()),Qt::CTRL + Qt::Key_O);
    fileMenu->addAction(tr("&Сохранить"), this, SLOT(saveChanges()), Qt::CTRL + Qt::Key_S);
    fileMenu->addSeparator();
    fileMenu->addAction(tr("&Выход"), this, SLOT(closeProgram()), Qt::CTRL + Qt::Key_Q);


    m_choiceFileLbl = new QLabel(tr("Выберите файл: "));

    m_choiceFilePB = new QPushButton(tr("Открыть"));
    m_choiceFilePB->setObjectName("choiceFilePB");

    m_crossImgPB = new QPushButton;
    m_crossImgPB->setIcon(QIcon(":/img/cross.png"));
    m_crossImgPB->setObjectName("crossImgPB");
    m_crossImgPB->setToolTip(tr("Очистить таблицу"));

    m_choiceFileLayout = new QHBoxLayout;
    m_choiceFileLayout->addWidget(m_choiceFileLbl);
    m_choiceFileLayout->addWidget(m_choiceFilePB);
    m_choiceFileLayout->addWidget(m_crossImgPB);


    model = new TableModel;
    table = new QTableView;
    table->setModel(model);


    m_searchLbl = new QLabel(tr("Поиск: "));
    m_searchEANLE = new QLineEdit;
    m_searchEANLE->setValidator(new QRegExpValidator(QRegExp("[0-9]{13}")));

    m_infoLayout = new QHBoxLayout;
    m_infoLayout->addWidget(m_searchLbl);
    m_infoLayout->addWidget(m_searchEANLE);


    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->addLayout(m_choiceFileLayout);
    m_mainLayout->addWidget(table);
    m_mainLayout->addLayout(m_infoLayout);
    m_mainLayout->addWidget(statusBar);

    connect(m_choiceFilePB, SIGNAL(clicked()), SLOT(selectFile()));
    connect(m_choiceFilePB, SIGNAL(pressed()), SLOT(selectFile()));

    connect(m_crossImgPB, SIGNAL(clicked()), SLOT(clearTable()));
    connect(m_crossImgPB, SIGNAL(pressed()), SLOT(clearTable()));

    connect(m_searchEANLE, SIGNAL(textEdited(const QString &)),
            SLOT(inputEAN(const QString &)));

}

BaseMode::~BaseMode()
{
    if(CSVFile.isOpen())
    CSVFile.close();
}

// Slots:
void BaseMode::selectFile()
{
    m_fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), QDir::rootPath(), "CSV file (*.csv)");

    CSVFile.setFileName(m_fileName);
    if (CSVFile.exists(m_fileName) && CSVFile.open(QIODevice::ReadOnly))
        m_fileContent = CSVFile.readAll(); // write all content in QByteArray

    if (!m_fileName.isEmpty()) {
        int count = 0; // counter for size of QString name
        for (int i = m_fileName.size(); m_fileName[i] != '/'; --i)
            ++count;
        QString name(count);
        for (int i = m_fileName.size(); m_fileName[i] != '/'; --i, --count) // to only name of file view, not all directory
            name[count] = m_fileName[i];
        m_choiceFileLbl->setText(QString(tr("Файл: %1")).arg(name));
    }
}

void BaseMode::inputEAN(const QString &EAN)
{
    if (EAN.size() == Scanner::g_EAN_size) {
        m_statusBarLbl->setText("");
        readFile(EAN);
    }
    else {
        m_statusBarLbl->setText(tr("Ввод не соотвествует формату EAN13!"));
    }
}

void BaseMode::clearTable()
{
    m_choiceFileLbl->setText("");
    m_searchEANLE->setText("");
    CSVFile.close();
    model->setData(model->index(model->rowCount() - 1, Scanner::SUM), QVariant(0), Qt::EditRole);
    model->removeRows(0,model->rowCount() - 2);
    m_statusBarLbl->setText(tr("Таблица очищена. Файл закрыт"));
}


void BaseMode::closeEvent(QCloseEvent *event)
{
    QMessageBox closeMsg;
    closeMsg.setIcon(QMessageBox::Question);
    closeMsg.setText("Вы точно хотите закрыть приложение?");
    closeMsg.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    closeMsg.setDefaultButton(QMessageBox::Yes);
    int ret = closeMsg.exec();
    switch (ret) {
    case QMessageBox::No:
        event->ignore();
        break;
     case QMessageBox::Yes:
        event->accept();
        break;
    }
}

void BaseMode::closeProgram()
{
    this->close();
}

// Just functions:
bool BaseMode::convertPrise(QStringList &lineList)
{
    if (lineList.at(Scanner::PRISE).contains(QRegExp("[0-9]{1,}\\.[0-9]{1,}"))) // !!!
        return true;
    else {
    for (int i = 0; i < lineList.at(Scanner::PRISE).size(); ++i)
        if (lineList.at(Scanner::PRISE).at(i) == ',') {
            lineList[Scanner::PRISE][i] = '.';
            return true;
        }
        }
    return false;
}

bool BaseMode::readFile(const QString &EAN)
{
    if (CSVFile.isOpen()) {
        QTextStream readStream(&m_fileContent);
        QString line;
        QStringList lineList;
        while (!readStream.atEnd()) {
            line = readStream.readLine();
            if (line.contains(EAN))
            {
                lineList.append(line.split(";"));
#ifdef DEBUG
                qDebug() << __FILE__ << " "  << __LINE__ << ":\n " << line << endl;
#endif
                if (convertPrise(lineList)) {
                model->addLine(lineList);
                }
                else
                {
                    int ret = QMessageBox::warning(this, tr("Что-то с ценой"), tr("В файле не обнаружена цена,\n"
                                                                            "либо она в неверном формате\n"
                                                                            "Проверьте наличие цены, либо ее формат\n"
                                                                            "Допустимый формат: 12.34"), QMessageBox::Ok);
                    qDebug() << "Нет цены! " << endl;
                    return false;
                }
                model->computeTotal();
                computeRest(); // the impelementation of this function is in inherits classes
                return true;
            }
            else if (readStream.atEnd())
                qDebug() << __FILE__ << " "  << __LINE__ << ":\n " << "Ничего не найдено!" << endl;
        } // while
    } // if
    else
        qDebug() << __FILE__ << " "  << __LINE__ << ":\n " << "File: " << m_fileName << " isn't open!" << endl;

    return false;
}

void BaseMode::saveChanges()
{
    QVector< QVector <QVariant> > tableContent;
    for (int i = 0; i < model->rowCount(); ++i) // add all rows from model
        tableContent.push_back(model->Row(i));

    QStringList lineList;

    QString lineContent = QTextCodec::codecForName("Windows-1251")->toUnicode(m_fileContent);
    QStringList lineListContent = lineContent.split(QRegExp(";|\\r\\n"));

    for (int i = 0; i < tableContent.size() - 2; ++i) // - 2 because it's without "Итого" row and previous row of end of table
        for (int j = 0; j < tableContent.first().size(); ++j)
            lineList.push_back(tableContent.at(i).at(j).toString());
#ifdef DEBUG
    for (int i = 0; i < lineList.size(); ++i)
        qDebug() << __FILE__ << " "  << __LINE__ << " " << lineList.at(i) << " ";
    qDebug() << endl << endl;
#endif

#ifdef DEBUG
    for (int i = 0; i < lineListContent.size(); ++i)
        qDebug() << __FILE__ << " "  << __LINE__ << " " << lineListContent.at(i) << " ";
    qDebug() << endl << endl;
#endif

    // Чтобы найти нужную строку, в которой необходимо заменить остаток lineListContent[Scanner::REST] на
    // остаток lineList[Scanner::REST], требуется найти начало строки в lineListContent по её коду и прибавить 4
    for (int i = Scanner::CODE; i < lineList.size(); i += Scanner::g_headers_size) {
        auto what = lineList.at(i);
        auto index = lineListContent.indexOf(what);
        lineListContent[index + 4] = lineList.at(i + Scanner::REST); // + 4 because Scanner::REST = 4
    }



#ifdef DEBUG
     for (int i = 0; i < lineListContent.size(); ++i)
         qDebug() << lineListContent.at(i) << " ";
#endif

     QString lineOut = lineListContent.join(';');
     int count = 0; // count of columns in file
     for (int i = 0; i < lineOut.size(); ++i) {
         if (lineOut.at(i) == ';')
             ++count;
         if (count == 6) { // 6 = кол-во точек с запятой, разделяющих столбцы
             lineOut.replace(i, 1, '\n');  // 13 = size of EAN
             count = 0;
         }
     }
     m_fileContent = lineOut.toLocal8Bit();

    QFile test("result.csv");
    if (test.open(QIODevice::WriteOnly)) {
        test.write(m_fileContent);
        test.close();
        }
    else

#ifdef DEBUG
        qDebug() << "File isn't open!";
#endif

}

