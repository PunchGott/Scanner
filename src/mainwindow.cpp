 #include "mainwindow.h"

MainWindow::MainWindow()
    : m_EANRegExp{"[0-9]{13}"}
{
    m_mainWidget = new QWidget();
    m_mainWidget->setFixedSize(800,600);

    m_choiceFileLbl = new QLabel("Выберите файл: ");

    m_choiceFilePB = new QPushButton("Выберите файл");
    m_crossImgPB = new QPushButton();
    m_crossImgPB->setIcon(QIcon("../img/cross.png"));
    m_crossImgPB->setStyleSheet("background: blue;");

    model = new TableModel();
    table = new QTableView();
    table->setModel(model);

    m_inputEANLE = new QLineEdit;
    m_inputEANLE->setValidator(new QRegExpValidator(QRegExp("[0-9]{13}")));

    m_choiceFileLayout = new QHBoxLayout();
    m_choiceFileLayout->addWidget(m_choiceFileLbl);
    m_choiceFileLayout->addWidget(m_choiceFilePB);
    m_choiceFileLayout->addWidget(m_crossImgPB);

    m_mainLayout = new QVBoxLayout(m_mainWidget);
    m_mainLayout->addLayout(m_choiceFileLayout);
    m_mainLayout->addWidget(table);
    m_mainLayout->addWidget(m_inputEANLE);

    connect(m_choiceFilePB, SIGNAL(clicked()), SLOT(selectFile()));
    connect(m_choiceFilePB, SIGNAL(pressed()), SLOT(selectFile()));
    connect(m_inputEANLE, SIGNAL(textEdited(const QString &)), SLOT(inputEAN(const QString &)));

    m_mainWidget->show();
}

MainWindow::~MainWindow()
{

}

void MainWindow::inputEAN(const QString &EAN)
{
    if (EAN.size() < 13)
        qDebug() << "Неправильный ввод!\n";
    else
        readFile(EAN);
}

void MainWindow::selectFile()
{
    m_fileName = QFileDialog::getOpenFileName(m_mainWidget,
        tr("Open Image"), QDir::currentPath(), tr("CSV file (*.csv)")); // Изменить позже currentPath() на rootPath()
    QString name;

    for (int i = m_fileName.size(); m_fileName[i] != '/' ; --i)
        name[i] = m_fileName[i];

    m_choiceFileLbl->setText(QString("Файл: %1").arg(name));
    qDebug() << m_fileName;
}

void MainWindow::readFile(const QString &EAN)
{
    QFile CSVFile(m_fileName);
    if (!CSVFile.open(QIODevice::ReadOnly)) // Проверяю, открываю файл и проверяю его на открытость одновременно
            qDebug() << "File" << m_fileName << "is't open!";
    else {
    QTextStream in(&CSVFile);
    QString line;
    while (!in.atEnd()) {
        line = in.readLine();
        if (line.contains(EAN))
        {
            addLine(line);
            qDebug() << line << endl;
            break;
        }
        else if (in.atEnd())
            qDebug() << "Ничего не найдено!\n";
    }
    }
}

void MainWindow::addLine(const QString &line)
{
    QStringList lineList;
    lineList.append(line.split(";"));
    for (int i = 0; i < 6; ++i) {
//        index = table->currentIndex();
        QModelIndex index = model->index(model->rowCount() - 1,i);
        qDebug() << index.row() << " " << index.column() << lineList[i] << endl;
        model->setData(index, QVariant(lineList[i]), Qt::EditRole); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }
    model->insertRows(model->rowCount(),1);
}
