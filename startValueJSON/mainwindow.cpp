#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "srcDLL/exch.h"
#include <stdio.h>

wchar_t convertedDataRecieve[60000];


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
/*   Вызывается при нажатии на кнопку Добавить    */
/**************************************************/
void MainWindow::on_addButton_clicked()
{

    QDynamicLayout * layout = new QDynamicLayout(this);  // Создаем объект динамического Layout
    m_verticalLayout->addWidget(layout);

}




/*********************************************************/
/*             Метод для отключения библиотеки           */
/*                Вызывает функцию closedll              */
/* Вызывается при нажатии на кнопку Отключить библиотеку */
/*********************************************************/
void MainWindow::on_closeLib_clicked()
{
    if (Qt::Checked==ui->checkLib->checkState())
    {
        textStatusBar->setText("Отключение библиотеки");
        ui->checkLib->setCheckState(Qt::Unchecked);
        
        bool answer=closedllWithCheck();
        if(!answer)
        {
            textStatusBar->setText("Функция CloseDll не сработала");
        }
    }
}




/**********************************************************/
/*            Метод для подключения библиотеки            */
/*  Подключается к библиотеке и ищет все функции в ней    */
/* Вызывается при нажатии на кнопку Подключить библиотеку */
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
        bool answer=closedllWithCheck();
        if(!answer)
        {
            textStatusBar->setText("Функция CloseDll не сработала");
        }
        return;
    }


    connectIni = (ConnectFunction) lib.resolve("dbConnectIni");
    if (!connectIni)
    {
        qWarning("Error dbConnectIni");
        textStatusBar->setText("Не найдена функция dbConnectIni");
        ui->checkLib->setCheckState(Qt::Unchecked);
        bool answer=closedllWithCheck();
        if(!answer)
        {
            textStatusBar->setText("Функция CloseDll не сработала");
        }
        return;
    }


    setData = (SetDataFunction) lib.resolve("SetDBDataString");
    if (!setData)
    {
        qWarning("Error SetDBDataString");
        textStatusBar->setText("Не найдена функция SetDBDataString");
        ui->checkLib->setCheckState(Qt::Unchecked);
        bool answer=closedllWithCheck();
        if(!answer)
        {
            textStatusBar->setText("Функция CloseDll не сработала");
        }
        return;
    }


    getData = (GetDataFunction) lib.resolve("GetDBDataString");
    if (!getData)
    {
        qWarning("Error GetDBDataString");
        textStatusBar->setText("Не найдена функция GetDBDataString");
        ui->checkLib->setCheckState(Qt::Unchecked);
        bool answer=closedllWithCheck();
        if(!answer)
        {
            textStatusBar->setText("Функция CloseDll не сработала");
        }
        return;
    }


    bool answerFromLib = initDll(8);
    if (answerFromLib==false)
    {
        textStatusBar->setText("Функция InitDLL_Qt вернула false");
        ui->checkLib->setCheckState(Qt::Unchecked);
        closedll();
        return;
    }


    textStatusBar->setText("Все функции в библиотеке найдены. InitDLL подключена");
}



/******************************************************/
/*  Создание файла JSON и отправка его в библиотеку   */
/* Вызывается при нажатии на кнопку Сформировать JSON */
/******************************************************/
void MainWindow::on_JSONButton_clicked()
{

    QJsonObject sopoObject;
    QJsonObject idObject;


    sopoObject["TRENING_NUMBER"]=ui->numberTrening->text();

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
    
    textStatusBar->setText("JSON сохранен (saveData.json)");


    if(Qt::Unchecked==ui->checkLib->checkState())
    {
        textStatusBar->setText("Библиотека не подключена, JSON файл не отправился, но сохранен(saveData.json)");
        return;
    }


    QVector<wchar_t> dataVector;
    dataVector.resize(toJsonData.size()+1);
    wchar_t * convertedData = dataVector.data();
    QString( toJsonData ).toWCharArray(convertedData);
    convertedData[toJsonData.size()] = '\0';

    for(int i=0; i<toJsonData.size() + 1;i++)
    {
        convertedDataRecieve[i] = convertedData[i];
    }


    textStatusBar->setText("Файл JSON готов");


    bool answerFromLib = false;


    answerFromLib = connectIni();
    if (answerFromLib==false)
    {
        textStatusBar->setText("Функция dbConnectIni вернула false");
        ui->checkLib->setCheckState(Qt::Unchecked);
        closedll();
        return;
    }


    if(ui->sessionNumber->text().isEmpty())
    {
       textStatusBar->setText("Не задан номер сессии. Нельзя отправить сообщение в базу");
       return;
    }

    int session = ui->sessionNumber->text().toInt();
    if(session<0)
    {
        textStatusBar->setText("Номер сессии отрицательный. Нельзя отправить сообщение в базу");
        return;
    }

    answerFromLib = false;
    answerFromLib = setData(session,convertedData);
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

    QString JsonString = QString::fromWCharArray(getData(ui->sessionNumber->text().toInt()));
    if(JsonString.isEmpty())
    {
        textStatusBar->setText("Данных из базы нет");
        return;
    }
    parsingJSON(JsonString);


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

    parsingJSON(JsonString);
}


void MainWindow::parsingJSON(QString &JsonString)
{
    QJsonDocument receiveDataJSON = QJsonDocument::fromJson(JsonString.toUtf8());
    QJsonObject JSONReceive = receiveDataJSON.object();
    QStringList allKeys = JSONReceive.keys();

    QString treningNumber;


    for(int i = 0; i < m_verticalLayout->count(); i++)
    {
        QDynamicLayout *dynamicLayout = qobject_cast<QDynamicLayout*>(m_verticalLayout->itemAt(i)->widget());
        dynamicLayout->deleteAll();
    }

    treningNumber = JSONReceive["TRENING_NUMBER"].toString();
    ui->numberTrening->setText(treningNumber);

    for(int i=1; i<allKeys.size();i++)
    {
        QJsonObject Array = JSONReceive[allKeys[i]].toObject();

        int id = Array["id"].toInt();
        int status = Array["status"].toInt();

        QString name = Array["name"].toString();
        QString type = Array["type_element"].toString();


        QDynamicLayout *layout = new QDynamicLayout(this,id,name,type,status);

        m_verticalLayout->addWidget(layout);

    }


    textStatusBar->setText("JSON выведен на экран");

}
