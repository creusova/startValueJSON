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
    textStatusBar->setText("Program start");
}



MainWindow::~MainWindow()
{
    delete ui;
}



/**************************************************/
/*   Метод для добавления динамических элементов  */
/*   Вызывается при нажатие на кнопку добавить    */
/**************************************************/
void MainWindow::on_addButton_clicked()
{
    QDynamicLayout *button = new QDynamicLayout(this);  // Создаем объект динамического лайаута

    m_verticalLayout->addWidget(button);
}


void MainWindow::on_closeLib_clicked()
{
    if (Qt::Checked==ui->checkLib->checkState())
    {
        textStatusBar->setText("Библиотека отключена");
        ui->checkLib->setCheckState(Qt::Unchecked);
        closedll();
    }
}

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



/****************************************************/
/* Создание файла JSON и отправка его в библиотеку  */
/****************************************************/
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
//		int positionStartInt = dynamicLayout->positionStart.text().toInt();

//        float QinFloat = dynamicLayout->Q_in.text().replace(QChar(','),QChar('.')).toFloat();
//        float Qout1Float = dynamicLayout->Q_out1.text().replace(QChar(','),QChar('.')).toFloat();
//        float Qout2Float =dynamicLayout->Q_out2.text().replace(QChar(','),QChar('.')).toFloat();
//        float PinFloat = dynamicLayout->P_in.text().replace(QChar(','),QChar('.')).toFloat();
//        float PoutFloat = dynamicLayout->P_out.text().replace(QChar(','),QChar('.')).toFloat();
//        float deltaPFloat = dynamicLayout->deltaP.text().replace(QChar(','),QChar('.')).toFloat();


        QString id = "id" + dynamicLayout->idEdit.text();
		idObject["id"] = idInt;//dynamicLayout->idNumber.text();
        idObject["name"] = name;
        idObject["type_element"] = type;
        idObject["status"] = status;

//		idObject["position_startup"] = positionStartInt;//dynamicLayout->positionStart.text();

//        while(!resultArray.isEmpty())
//        {
//            resultArray.removeLast();
//        }

//		Qin_outObject["Q_in"] = QinFloat;// dynamicLayout->Q_in.text();
//		Qin_outObject["Q_out1"] = Qout1Float;// dynamicLayout->Q_out1.text();
//		Qin_outObject["Q_out2"] = Qout2Float;//dynamicLayout->Q_out2.text();
//        resultArray.append(Qin_outObject);

//		Pin_outObject["P_in"] = PinFloat; //dynamicLayout->P_in.text();
//		Pin_outObject["P_out"] = PoutFloat; //dynamicLayout->P_out.text();
//		Pin_outObject["deltaP"] = deltaPFloat; //dynamicLayout->deltaP.text();
//        resultArray.append(Pin_outObject);

//        idObject["result_math_model"] = resultArray;

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


    textStatusBar->setText("JSON сохранен и отправлен");

}



void MainWindow::on_receiveDataBDButton_clicked()
{

	QString JsonString = QString::fromWCharArray(convertedDataRecieve);
	QJsonDocument receiveDataJSON = QJsonDocument::fromJson(JsonString.toUtf8());
	QJsonObject JSONReceive = receiveDataJSON.object();
	QStringList allKeys = JSONReceive.keys();
	QJsonObject Array;
    QString name, type,id, status, treningNumber;
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

        id = Array["id"].toString();
        status = Array["status"].toString();

        name = Array["name"].toString();
        type = Array["type_element"].toString();


//		arrayQ=Array["result_math_model"].toArray();

//		Q = arrayQ[0].toObject();
//		Qin = Q["Q_in"].toDouble();
//		Qout1 = Q["Q_out1"].toDouble();
//		Qout2 = Q["Q_out2"].toDouble();

//		P = arrayQ[1].toObject();
//		Pin = P["P_in"].toDouble();
//		Pout = P["P_out"].toDouble();
//		deltaP = P["deltaP"].toDouble();

      //  QDynamicLayout *button = new QDynamicLayout(this,id,name,type,status);  // Создаем объект динамической кнопки

//       m_verticalLayout->addWidget(button);
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
//    int treningNumber;
//    int id, status;
//    float Qin,Qout1,Qout2,Pin,Pout,deltaP;
    QString name, type,id, status,treningNumber;
    QJsonArray arrayQ;
    QJsonObject Q;
    QJsonObject P;

    for(int i = 0; i < m_verticalLayout->count(); i++)
    {
        QDynamicLayout *dynamicLayout = qobject_cast<QDynamicLayout*>(m_verticalLayout->itemAt(i)->widget());
        dynamicLayout->deleteAll();
    }

//    Array = JSONReceive[allKeys[0]].toObject();
    treningNumber = JSONReceive["TRENING_NUMBER"].toString();
    ui->numberTrening->setText(treningNumber);

    for(int i=1; i<allKeys.size();i++)
    {
        Array = JSONReceive[allKeys[i]].toObject();

        id = Array["id"].toString();
        status = Array["status"].toString();

        name = Array["name"].toString();
        type = Array["type_element"].toString();
//        position = Array["position_startup"].toInt();

//        arrayQ=Array["result_math_model"].toArray();

//        Q = arrayQ[0].toObject();
//        Qin = Q["Q_in"].toDouble();
//        Qout1 = Q["Q_out1"].toDouble();
//        Qout2 = Q["Q_out2"].toDouble();

//        P = arrayQ[1].toObject();
//        Pin = P["P_in"].toDouble();
//        Pout = P["P_out"].toDouble();
//        deltaP = P["deltaP"].toDouble();

        //QDynamicLayout *button = new QDynamicLayout(this,id,name,type,status);  // Создаем объект динамической кнопки

        //m_verticalLayout->addWidget(button);

    }


	textStatusBar->setText("Успех");



}
