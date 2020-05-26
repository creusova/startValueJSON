#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "srcDLL/exch.h"
#include <stdio.h>

wchar_t convertedDataRecieve[60000];


typedef bool (*CloseFunction)();
CloseFunction closedll;
typedef bool (*initFunction)(int);
initFunction initDll;
typedef bool (*ConnectFunction)();
ConnectFunction connectIni;
typedef bool (*SetDataFunction)(int,wchar_t*);
SetDataFunction setData;
typedef wchar_t (*GetDataFunction)(int);
GetDataFunction getData;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{


    ui->setupUi(this);

    m_verticalLayout = new QVBoxLayout(ui->scrollAreaWidgetContents);
    textStatusBar = new QLabel;
    ui->statusbar->addWidget(textStatusBar);
    textStatusBar->setText("Программа запущена");
}



MainWindow::~MainWindow()
{
    delete ui;
}



/**************************************************/
/*   Метод для добавления динамических элементов  */
/*   Вызывается при нажатие на кнопку Добавить    */
/**************************************************/
void MainWindow::on_addButton_clicked()
{

    QDynamicLayout *button = new QDynamicLayout(this);  // Создаем объект динамического Layout
    m_verticalLayout->addWidget(button);

}




/*********************************************************/
/*             Метод для отключения библиотеки           */
/*                Вызывает функцию closedll              */
/* Вызывается при нажатие на кнопку Отключить библиотеку */
/*********************************************************/
void MainWindow::on_closeLib_clicked()
{
    if (Qt::Checked==ui->checkLib->checkState())
    {
        textStatusBar->setText("Библиотека отключена");
        ui->checkLib->setCheckState(Qt::Unchecked);
        closedll();
    }
}




/**********************************************************/
/*            Метод для подключения библиотеки            */
/*  Подключается к библиотеке и ищет все функции в ней    */
/* Вызывается при нажатие на кнопку Подключить библиотеку */
/**********************************************************/
void MainWindow::on_addLib_clicked()
{
    QString lib_path=qApp->applicationDirPath() +"/exch.dll";
    QLibrary lib(lib_path);

    if( !lib.load() )
    {
         qDebug("Loading failed!");
         textStatusBar->setText(lib_path+"  Не удалось подключиться к библиотеке");
         ui->checkLib->setCheckState(Qt::Unchecked);
         return;
    }
    else
    {
        textStatusBar->setText(lib_path+"  Подключились к библиотеке");
        ui->checkLib->setCheckState(Qt::Checked);
        qWarning("Load lib");
    }



    closedll = (CloseFunction) lib.resolve("CloseDLL");
    if (!closedll)
    {
        qWarning("Error CloseDLL");
        textStatusBar->setText("Не найдена функция CloseDLL");
        ui->checkLib->setCheckState(Qt::Unchecked);
        return;
    }



    initDll = (initFunction) lib.resolve("InitDLL_Qt");
    if (!initDll)
    {
        qWarning("Error InitDLL_Qt");
        textStatusBar->setText("Не найдена функция InitDLL_Qt");
        ui->checkLib->setCheckState(Qt::Unchecked);
        closedll();
        return;
    }


    connectIni = (ConnectFunction) lib.resolve("dbConnectIni");
    if (!connectIni)
    {
        qWarning("Error dbConnectIni");
        textStatusBar->setText("Не найдена функция dbConnectIni");
        ui->checkLib->setCheckState(Qt::Unchecked);
        closedll();
        return;
    }


    setData = (SetDataFunction) lib.resolve("SetDBDataString");
    if (!setData)
    {
        qWarning("Error SetDBDataString");
        textStatusBar->setText("Не найдена функция SetDBDataString");
        ui->checkLib->setCheckState(Qt::Unchecked);
        closedll();
        return;
    }


    getData = (GetDataFunction) lib.resolve("GetDBDataString");
    if (!getData)
    {
        qWarning("Error GetDBDataString");
        textStatusBar->setText("Не найдена функция GetDBDataString");
        ui->checkLib->setCheckState(Qt::Unchecked);
        closedll();
        return;
    }

    textStatusBar->setText("Все функции в библиотеке найдены");
}



/******************************************************/
/*  Создание файла JSON и отправка его в библиотеку   */
/* Вызывается при нажатие на кнопку Сформировать JSON */
/******************************************************/
void MainWindow::on_JSONButton_clicked()
{

    QJsonObject sopoObject;
    QJsonObject idObject;
    QJsonArray resultArray;
    QJsonObject Qin_outObject;
    QJsonObject Pin_outObject;


    for(int i = 0; i < m_verticalLayout->count(); i++)
    {
        QDynamicLayout *dynamicLayout = qobject_cast<QDynamicLayout*>(m_verticalLayout->itemAt(i)->widget());

        int idInt = dynamicLayout->idEdit.text().toInt();
        QString name = dynamicLayout->nameEdit.text();
        QString type = dynamicLayout->typeEdit.text();
        int status = dynamicLayout->statusEdit.text().toInt();


        QString id = "id" + dynamicLayout->idEdit.text();
        idObject["id"] = idInt;
        idObject["name"] = name;
        idObject["type_element"] = type;
        idObject["status"] = status;


        sopoObject["TRENING_NUMBER"]=ui->numberTrening->text();
        sopoObject[id] = idObject;


    }


    QFile saveFile(QStringLiteral("saveData.json"));
    if (!saveFile.open(QIODevice::WriteOnly))
    {
        qWarning("Couldn't open save file.");
        textStatusBar->setText("Не открыть файл для записи JSON");
    }


    QJsonDocument saveDoc(sopoObject);
    auto toJsonData = saveDoc.toJson();
    saveFile.write(toJsonData);
    wchar_t convertedData[ toJsonData.size() + 1 ];
    QString( toJsonData ).toWCharArray(convertedData);
    convertedData[toJsonData.size()] = '\0';

    for(int i=0; i<toJsonData.size() + 1;i++)
    {
        convertedDataRecieve[i] = convertedData[i];
    }


    textStatusBar->setText("Файл JSON готов");


    bool answerFromLib = false;

    answerFromLib = initDll(8);
    if (answerFromLib==false)
    {
        textStatusBar->setText("Функция InitDLL_Qt вернула false");
        ui->checkLib->setCheckState(Qt::Unchecked);
        closedll();
        return;
    }


    answerFromLib = false;
    answerFromLib = connectIni();
    if (answerFromLib==false)
    {
        textStatusBar->setText("Функция dbConnectIni вернула false");
        ui->checkLib->setCheckState(Qt::Unchecked);
        closedll();
        return;
    }

    answerFromLib = false;
    answerFromLib = setData(0,convertedData);
    if (answerFromLib==false)
    {
        textStatusBar->setText("Функция SetDBDataString вернула false");
        ui->checkLib->setCheckState(Qt::Unchecked);
        closedll();
        return;
    }


    textStatusBar->setText("JSON сохранен (saveData.json) и отправлен в базу данных");

}



void MainWindow::on_receiveDataBDButton_clicked()
{

	QString JsonString = QString::fromWCharArray(convertedDataRecieve);
	QJsonDocument receiveDataJSON = QJsonDocument::fromJson(JsonString.toUtf8());
	QJsonObject JSONReceive = receiveDataJSON.object();
	QStringList allKeys = JSONReceive.keys();
	QJsonObject Array;
    int id, status;
    QString name, type, treningNumber;
	QJsonArray arrayQ;


    for(int i = 0; i < m_verticalLayout->count(); i++)
    {
        QDynamicLayout *dynamicLayout = qobject_cast<QDynamicLayout*>(m_verticalLayout->itemAt(i)->widget());
        dynamicLayout->deleteAll();
    }

    treningNumber = JSONReceive["TRENING_NUMBER"].toString();
    ui->numberTrening->setText(treningNumber);

    for(int i=1; i<allKeys.size();i++)
	{
		Array = JSONReceive[allKeys[i]].toObject();

        id = Array["id"].toInt();
        status = Array["status"].toInt();
        name = Array["name"].toString();
        type = Array["type_element"].toString();

        QDynamicLayout *layout = new QDynamicLayout(this,id,name,type,status);  // Создаем объект динамической кнопки

       m_verticalLayout->addWidget(layout);
    }


	textStatusBar->setText("Успех");

}

void MainWindow::on_receiveDataFileButton_clicked()
{
	QString str = QFileDialog::getOpenFileName(0, "Open Dialog", "", "*.ini *.json *.txt");


	QFile jsonFile(str);
	if (!jsonFile.open(QIODevice::ReadOnly))
	{
		return;
	}

    QString JsonString = jsonFile.readAll();
    QJsonDocument receiveDataJSON = QJsonDocument::fromJson(JsonString.toUtf8());
    QJsonObject JSONReceive = receiveDataJSON.object();
    QStringList allKeys = JSONReceive.keys();

    QJsonObject Array;
    int id, status;
    QString name, type,treningNumber;
    QJsonArray arrayQ;
    QJsonObject Q;
    QJsonObject P;

    for(int i = 0; i < m_verticalLayout->count(); i++)
    {
        QDynamicLayout *dynamicLayout = qobject_cast<QDynamicLayout*>(m_verticalLayout->itemAt(i)->widget());
        dynamicLayout->deleteAll();
    }

    treningNumber = JSONReceive["TRENING_NUMBER"].toString();
    ui->numberTrening->setText(treningNumber);

    for(int i=1; i<allKeys.size();i++)
    {
        Array = JSONReceive[allKeys[i]].toObject();

        id = Array["id"].toInt();
        status = Array["status"].toInt();

        name = Array["name"].toString();
        type = Array["type_element"].toString();


        QDynamicLayout *layout = new QDynamicLayout(this,id,name,type,status);  // Создаем объект динамической кнопки

        m_verticalLayout->addWidget(layout);

    }


	textStatusBar->setText("Успех");



}
