 #include "basemode.h"

#define DEBUG

// Просмотреть ВЕСЬ код на наличие ошибок и некрасоты!
// Сделать возможность создания нуменклатуры в Excel
// После нажатия на кнопку "Сохранить" массив QByteArray записывается в новый файл, который затем переименовывается

// Добавить обработку пользовательских ошибок и вывод их в QDialog
// Добавить обработку логических и программных ошибок
// Добавить QMenuBar
// Добавть QStatusBar
// Добавить подсказки


BaseMode::BaseMode(QWidget */*parent*/)
{
    m_mainWidget = new QWidget();
    m_mainWidget->setFixedSize(1280,768);

    m_choiceFileLbl = new QLabel("Выберите файл: ");
    m_searchLbl = new QLabel("Поиск: ");

    m_choiceFilePB = new QPushButton("Открыть");
    m_choiceFilePB->setObjectName("choiceFilePB");
    m_crossImgPB = new QPushButton();
    m_crossImgPB->setIcon(QIcon(":/img/cross.png"));
    m_crossImgPB->setObjectName("crossImgPB");

    model = new TableModel();
    table = new QTableView();
    table->setModel(model);


    QMenuBar *menuBar = new QMenuBar(m_mainWidget);
    QMenu *fileMenu = new QMenu("&Файл");
    fileMenu->addAction("&Открыть", this, SLOT(selectFile()),Qt::CTRL + Qt::Key_O);
    menuBar->addMenu(fileMenu);
    fileMenu->addAction("&Сохранить", this, SLOT(saveChanges()), Qt::CTRL + Qt::Key_S);
//    fileMenu->addAction("&Выход", this, SLOT(), Qt::CTRL + Qt::Key_Q);
    m_crossImgPB->setToolTip("Clear table");


    m_inputEANLE = new QLineEdit;
    m_inputEANLE->setValidator(new QRegExpValidator(QRegExp("[0-9]{13}")));

    m_choiceFileLayout = new QHBoxLayout();
    m_choiceFileLayout->addWidget(m_choiceFileLbl);
    m_choiceFileLayout->addWidget(m_choiceFilePB);
    m_choiceFileLayout->addWidget(m_crossImgPB);

    m_infoLayout = new QHBoxLayout();
    m_infoLayout->addWidget(m_searchLbl);
    m_infoLayout->addWidget(m_inputEANLE);

    m_mainLayout = new QVBoxLayout(m_mainWidget);
    m_mainLayout->addLayout(m_choiceFileLayout);
    m_mainLayout->addWidget(table);
    m_mainLayout->addLayout(m_infoLayout);

    connect(m_choiceFilePB, SIGNAL(clicked()), SLOT(selectFile()));
    connect(m_choiceFilePB, SIGNAL(pressed()), SLOT(selectFile()));

    connect(m_crossImgPB, SIGNAL(clicked()), SLOT(clearTable()));
    connect(m_crossImgPB, SIGNAL(pressed()), SLOT(clearTable()));

    connect(m_inputEANLE, SIGNAL(textEdited(const QString &)),
            SLOT(inputEAN(const QString &)));

    m_mainWidget->show();
}

BaseMode::~BaseMode()
{
    if(CSVFile.isOpen())
    CSVFile.close();
}

// Slots:
void BaseMode::selectFile()
{
    m_fileName = QFileDialog::getOpenFileName(m_mainWidget,
        tr("Open Image"), QDir::currentPath(), tr("CSV file (*.csv)")); // Изменить позже currentPath() на rootPath()

    CSVFile.setFileName(m_fileName);
    if (CSVFile.exists(m_fileName) && CSVFile.open(QIODevice::ReadOnly))
        m_fileContent = CSVFile.readAll(); // write all content in QByteArray

    if (!m_fileName.isEmpty()) {
        int count = 0; // counter for size QString name
        for (int i = m_fileName.size(); m_fileName[i] != '/'; --i)
            ++count;
        QString name(count);
        for (int i = m_fileName.size(); m_fileName[i] != '/'; --i, --count) // to only name of file view, not all directory
            name[count] = m_fileName[i];
        m_choiceFileLbl->setText(QString("Файл: %1").arg(name));
    }
}

void BaseMode::inputEAN(const QString &EAN)
{
    if (EAN.size() < 13)
        qDebug() << __LINE__ << " " << __FILE__ << ":\n " << "Неправильный ввод! " << endl;
    else
        readFile(EAN);
}

void BaseMode::clearTable()
{
    m_choiceFileLbl->setText("");
    CSVFile.close();
    model->setData(model->index(model->rowCount() - 1, Scanner::SUM), QVariant(0), Qt::EditRole);
    model->removeRows(0,model->rowCount() - 2);
    // Поставить значение в "Итого: " = 0
}

// Just functions:
bool BaseMode::convertPrise(QStringList &lineList)
{
    if (lineList.at(Scanner::PRISE).contains(QRegExp("[0-9]{1,}\\.[0-9]{1,}"))) // !!!
        return true;
    else {
    for (int i = 0; i < lineList.at(Scanner::PRISE).size(); ++i) {
        if (lineList.at(Scanner::PRISE).at(i) == ',') {
            lineList[Scanner::PRISE][i] = '.';
            return true;
        }
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
                    qDebug() << "Не обнаружена цена! " << endl;
                    return false;
                }
                model->computeTotal();
                computeRest(); // the impelementation of this function is in inherits classes
                writeInFile(line);
                return true;
            }
            else if (readStream.atEnd())
                qDebug() << __FILE__ << " "  << __LINE__ << ":\n " << "Ничего не найдено!" << endl;
        } // while
    } // if
    else
        qDebug() << __FILE__ << " "  << __LINE__ << ":\n " << "File" << m_fileName << "isn't open!" << endl;

    return false;
}

bool BaseMode::changeFile(QStringList &/*lineList*/)
{
    return false;
}

bool BaseMode::changeModel(QStringList &/*lineList*/)
{
    return false;
}

void BaseMode::saveChanges()
{
    QVector<QVariant> vector = model->Row(0);

    QString line;
    QStringList lineList;

    QString lineContent = QTextCodec::codecForName("Windows-1251")->toUnicode(m_fileContent);
    QStringList lineListContent = lineContent.split(QRegExp(";|\\r\\n"));
#ifdef DEBUG
    qDebug() << lineListContent.at(0) << endl;
#endif
    for (int i = 0; i < vector.size(); ++i) {
        lineList.push_back(vector.at(i).toString());
    }
    for (int i = 0; i < lineListContent.size(); ++i)
#ifdef DEBUG
        qDebug() << lineListContent.at(i) << " ";
    qDebug() << endl << endl;
#endif

    lineListContent.indexOf(lineList.at(Scanner::EAN));
    lineListContent[Scanner::REST] = lineList.at(Scanner::AMOUNT);

    // Чтобы найти нужную строку, в которой необходимо заменить остаток lineListContent[Scanner::REST] на
    // остаток lineList[Scanner::REST], требуется найти начало строки в lineListContent по её коду и прибавить 4
    auto what = lineList.at(Scanner::CODE);
    auto index = lineListContent.indexOf(what);
    lineListContent[index + 4] = lineList.at(Scanner::REST); // + 4 because Scanner::REST = 4

     for (int i = 0; i < lineListContent.size(); ++i)
#ifdef DEBUG
         qDebug() << lineListContent.at(i) << " ";
#endif

     QString lineOut = lineListContent.join(';');
     int count = 0; // count of columns in file
     for (int i = 0; i < lineOut.size(); ++i) {
         if (lineOut.at(i) == ';')
             ++count;
         if (count == 6) { // 6 = кол-во столбцов
             lineOut.replace(i, 1, '\n');  // 13 = size of EAN
             count = 0;
         }
     }

     m_fileContent = lineOut.toLocal8Bit();

    QFile test("test123.csv");
    if (test.open(QIODevice::WriteOnly)) {
        test.write(m_fileContent);
        test.close();
        }
    else
#ifdef DEBUG
        qDebug() << "File isn't open!";
#endif

}

