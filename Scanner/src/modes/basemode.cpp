#include "basemode.h"

#define DEBUG
//#define SCANNER_MODE

BaseMode::BaseMode(QWidget */*parent*/)
{
    this->setFixedSize(1200, 800);

    m_statusBarLbl = new QLabel("StatusBar");
    statusBar = new QStatusBar;
    statusBar->addWidget(m_statusBarLbl);


    QMenuBar *menuBar = new QMenuBar(this);
    menuBar->resize(this->width(),menuBar->height());
    QMenu *fileMenu = new QMenu(tr("&Файл"));
    QMenu *viewMenu = new QMenu(tr("&Вид"));
    QMenu *helpMenu = new QMenu(tr("&Справка"));
    menuBar->addMenu(fileMenu);
    menuBar->addMenu(viewMenu);
    menuBar->addMenu(helpMenu);

    qDebug() << menuBar->width() << " " << menuBar->height();

    fileMenu->addAction(tr("&Открыть"), this, SLOT(selectFile()),Qt::CTRL + Qt::Key_O);
    fileMenu->addAction(tr("&Сохранить"), this, SLOT(saveChanges()), Qt::CTRL + Qt::Key_S);
    fileMenu->addSeparator();
    fileMenu->addAction(tr("&Выход"), this, SLOT(closeProgram()), Qt::CTRL + Qt::Key_Q);

    viewMenu->addAction(tr("&Выход"), this, SLOT(closeProgram()), Qt::CTRL + Qt::Key_Q);
    helpMenu->addAction(tr("&Выход"), this, SLOT(closeProgram()), Qt::CTRL + Qt::Key_Q);

    m_choiceFileLbl = new QLabel(tr("Выберите файл: "));

    m_choiceFilePB = new QPushButton(tr("Открыть"));
    m_choiceFilePB->setObjectName("choiceFilePB");

    m_crossImgPB = new QPushButton;
    m_crossImgPB->setIcon(QIcon(":/img/cross.png"));
    m_crossImgPB->setObjectName("crossImgPB");
    m_crossImgPB->setToolTip(tr("Очистить таблицу"));

    m_choiceFileLayout = new QHBoxLayout;
    m_choiceFileLayout->setContentsMargins(0,20,0,0);
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
    m_mainLayout->setMargin(20);
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
#ifdef SCANNER_MODE
        m_searchEANLE->setText("");
#endif
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
    QMessageBox::warning(this, tr("Что-то с ценой"), tr("В файле не обнаружена цена,\n"
                                                            "либо она в неверном формате\n"
                                                            "Проверьте наличие цены, либо ее формат\n"
                                                            "Допустимый формат: 12.34"), QMessageBox::Ok);
    return false;
}

bool BaseMode::readFile(const QString &EAN)
// Изменить функцию так, что она считывает файл не каждый раз, а только тогда,
// когда введенный EAN-код не равен предыдущему
{
    if (CSVFile.isOpen()) {
        QTextStream readStream(&m_fileContent);
        QString line;
        QStringList lineList;
        while (!readStream.atEnd()) {
            line = readStream.readLine();
            if (line.contains(EAN)) {
                lineList.append(line.split(";"));
#ifdef DEBUG
                qDebug() << __FILE__ << " "  << __LINE__ << ":\n " << line << endl;
#endif

                // Добавить здесь функцию isHeadRight(), проверяющую на корректность всю строку.
                // Эта функция должна возвращать true если строка содержит всё, что необходимо.
                // Сама проверка происходит в отдельных функциях на каждое необходимое свойство в таблице
                // по типу того, как это делает функция convertPrise()
                if (convertPrise(lineList)) {
                model->addLine(lineList);
                model->computeTotal();
                computeRest(); // the impelementation of this function is in inherits classes
                return true;
                }
            }
        }
        QMessageBox::critical(this, tr("Товар не найден"), tr("Такого EAN-кода не было найдено!"), QMessageBox::Ok);
    } // if
    else
        QMessageBox::critical(this, tr("Файл не открыт"), tr("Не получается открыть файл!"), QMessageBox::Ok);

    return false;
}

void BaseMode::saveChanges()
{
    QVector< QVector <QVariant> > modelContent;
    for (int i = 0; i < model->rowCount(); ++i) // add all rows from model in modelContent
        modelContent.push_back(model->Row(i));

    QStringList lineList;

    QString lineFileContent = QTextCodec::codecForName("Windows-1251")->toUnicode(m_fileContent); // add all file contents in lineFileContent
    QStringList lineListContentFile = lineFileContent.split(QRegExp(";|\\r\\n"));

    for (int i = 0; i < modelContent.size() - 2; ++i) // - 2 because it's without "Итого" row and previous row of end of table
        for (int j = 0; j < modelContent.first().size(); ++j)
            lineList.push_back(modelContent.at(i).at(j).toString());
#ifdef DEBUG
    for (int i = 0; i < lineList.size(); ++i)
        qDebug() << __FILE__ << " "  << __LINE__ << " " << lineList.at(i) << " ";
    qDebug() << endl << endl;
#endif

#ifdef DEBUG
    for (int i = 0; i < lineListContentFile.size(); ++i)
        qDebug() << __FILE__ << " "  << __LINE__ << " " << lineListContentFile.at(i) << " ";
    qDebug() << endl << endl;
#endif

    // Чтобы найти нужную строку, в которой необходимо заменить остаток lineListContentFile[Scanner::REST] на
    // остаток lineList[Scanner::REST], требуется найти начало строки в lineListContentFile по её коду и прибавить 4
    for (int i = Scanner::CODE; i < lineList.size(); i += Scanner::g_headers_size) {
        auto what = lineList.at(i);
        auto index = lineListContentFile.indexOf(what);
        lineListContentFile[index + 4] = lineList.at(i + Scanner::REST); // + 4 because Scanner::REST = 4
    }

#ifdef DEBUG
     for (int i = 0; i < lineListContentFile.size(); ++i)
         qDebug() << __FILE__ << " " << __LINE__ << " " << lineListContentFile.at(i) << " ";
#endif

     QString lineOut = lineListContentFile.join(';');
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
        QMessageBox::critical(this,tr("Не получается сохранить изменения"), tr("При сохранении изменений произошла ошибка."
                                                                               "Проверьте формат исходного файла"), QMessageBox::Ok);
}

