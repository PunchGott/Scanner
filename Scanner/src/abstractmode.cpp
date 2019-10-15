#include "abstractmode.h"

#define DEBUG

// Добавить функцию подсчета оставшегося товара на складе REST
// Сделать возможность создания нуменклатуры в Excel
// Добавить возможность чтения XML формата

// Добавить обработку пользовательских ошибок и вывод их в QDialog
// Добавить обработку логических и программных ошибок
// Добавить верхнюю панель
// Добавить бар-меню
// Добавить подсказки
// Перевести контекстное меню на русский
// Добавить настройки
// Добавить хоть какие-то стили

AbstractMode::AbstractMode(QWidget *parent)
{
    m_mainWidget = new QWidget();
    m_mainWidget->setFixedSize(1280,768);

    m_choiceFileLbl = new QLabel("Выберите файл: ");
    m_searchLbl = new QLabel("Поиск: ");

    m_choiceFilePB = new QPushButton("Выберите файл");
    m_choiceFilePB->setObjectName("choiceFilePB");
    m_crossImgPB = new QPushButton();
    m_crossImgPB->setIcon(QIcon(":/img/cross.png"));
    m_crossImgPB->setObjectName("crossImgPB");

    model = new TableModel();
    table = new QTableView();
    table->setModel(model);

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

    qDebug() << connect(m_choiceFilePB, SIGNAL(clicked()), SLOT(selectFile()));
    connect(m_choiceFilePB, SIGNAL(pressed()), this, SLOT(selectFile()));

    connect(m_crossImgPB, SIGNAL(clicked()), this, SLOT(clearTable()));
    connect(m_crossImgPB, SIGNAL(pressed()), this, SLOT(clearTable()));

    connect(m_inputEANLE, SIGNAL(textEdited(const QString &)), this, SLOT(inputEAN(const QString &)));

    m_mainWidget->show();
}

AbstractMode::~AbstractMode()
{
    if(CSVFile.isOpen())
    CSVFile.close();
}

// Slots:
void AbstractMode::selectFile()
{
    m_fileName = QFileDialog::getOpenFileName(m_mainWidget,
        tr("Open Image"), QDir::currentPath(), tr("CSV file (*.csv)")); // Изменить позже currentPath() на rootPath()

    CSVFile.setFileName(m_fileName);

    if (!m_fileName.isEmpty()) {
        QString name;
        for (int i = m_fileName.size(); m_fileName[i] != '/'; --i) // to only name of file view, not all directory
            name[i] = m_fileName[i];
        m_choiceFileLbl->setText(QString("Файл: %1").arg(name));
    }
}

void AbstractMode::inputEAN(const QString &EAN)
{
    if (EAN.size() < 13)
        qDebug() << __LINE__ << " " << __FILE__ << ":\n " << "Неправильный ввод! " << endl;
    else
        readFile(EAN);
}

void AbstractMode::clearTable()
{
    m_choiceFileLbl->setText("");
    CSVFile.close();
    model->removeRows(0,model->rowCount() - 2);
}

// Just functions:
bool AbstractMode::convertPrise(QStringList &lineList)
{
    for (int i = 0; i < lineList.at(Scanner::PRISE).size(); ++i) {
        if (lineList.at(Scanner::PRISE).at(i) == ',') {
            lineList[Scanner::PRISE][i] = '.';
            return true;
        }
    }
    return false;
}

bool AbstractMode::readFile(const QString &EAN)
{
    if (CSVFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&CSVFile);
        QString line;
        QStringList lineList;
        while (!in.atEnd()) {
            line = in.readLine();
            if (line.contains(EAN))
            {
                lineList.append(line.split(";"));
#ifdef DEBUG
                qDebug() << __LINE__ << " "  << __FILE__ << ":\n " << line << endl;
#endif
                if (changeFile(lineList, Scanner::SHIPMENT) &&
                        changeModel(lineList, Scanner::SHIPMENT))
                    return true;
            }
            else if (in.atEnd())
                qDebug() << __LINE__ << " "  << __FILE__ << ":\n " << "Ничего не найдено!" << endl;
        } // while
    } // if
    else
        qDebug() << __LINE__ << " "  << __FILE__ << ":\n " << "File" << m_fileName << "isn't open!" << endl;

    return false;
}

bool AbstractMode::changeFile(QStringList &lineList, int role)
{

}

bool AbstractMode::changeModel(QStringList &lineList, int role)
{
    if (convertPrise(lineList) && role == Scanner::SHIPMENT) {
    model->addLine(lineList, Scanner::SHIPMENT);
    model->computeTotal();
    }
    else qDebug() << "Неправильная цена!" << endl;
}
