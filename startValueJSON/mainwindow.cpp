#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "exch.h"
#include <stdio.h>

wchar_t convertedDataRecieve[1000];



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_verticalLayout = new QVBoxLayout(ui->scrollAreaWidgetContents);
    textStatusBar = new QLabel;
}



MainWindow::~MainWindow()
{
    delete ui;
}



/**************************************************/
/*   Метод для добавления динамических элементов  */
/**************************************************/
void MainWindow::on_addButton_clicked()
{
    QDynamicLayout *button = new QDynamicLayout(this);  // Создаем объект динамической кнопки

    m_verticalLayout->addWidget(button);
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


		int idInt = dynamicLayout->idNumber.text().toInt();
		int positionStartInt = dynamicLayout->positionStart.text().toInt();
		float QinFloat = dynamicLayout->Q_in.text().toFloat();
		float Qout1Float = dynamicLayout->Q_out1.text().toFloat();
		float Qout2Float =dynamicLayout->Q_out2.text().toFloat();
		float PinFloat = dynamicLayout->P_in.text().toFloat();
		float PoutFloat = dynamicLayout->P_out.text().toFloat();
		float deltaPFloat = dynamicLayout->deltaP.text().toFloat();


        QString id = "id" + dynamicLayout->idNumber.text();
		idObject["id"] = idInt;//dynamicLayout->idNumber.text();
		idObject["position_startup"] = positionStartInt;//dynamicLayout->positionStart.text();

        while(!resultArray.isEmpty())
        {
            resultArray.removeLast();
        }

		Qin_outObject["Q_in"] = QinFloat;// dynamicLayout->Q_in.text();
		Qin_outObject["Q_out1"] = Qout1Float;// dynamicLayout->Q_out1.text();
		Qin_outObject["Q_out2"] = Qout2Float;//dynamicLayout->Q_out2.text();
        resultArray.append(Qin_outObject);

		Pin_outObject["P_in"] = PinFloat; //dynamicLayout->P_in.text();
		Pin_outObject["P_out"] = PoutFloat; //dynamicLayout->P_out.text();
		Pin_outObject["deltaP"] = deltaPFloat; //dynamicLayout->deltaP.text();
        resultArray.append(Pin_outObject);

        idObject["result_math_model"] = resultArray;

        sopoObject["Trening_Number"]=ui->numberTrening->text();
        sopoObject[id] = idObject;


    }


    QFile saveFile(QStringLiteral("saveData.json"));
    if (!saveFile.open(QIODevice::WriteOnly))
    {
		//qWarning("Couldn't open save file.");
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

    ui->statusbar->addWidget(textStatusBar);
    textStatusBar->setText("Файл JSON готов");


	QLibrary lib("exch");

    if( !lib.load() )
    {
		 //qWarning("Loading failed!");
         textStatusBar->setText("Не удалось подключиться к библиотеке");
         return;
    }
    else
    {
		//qWarning("Ok");
    }

	typedef bool (*CloseFunction)();
	CloseFunction closedll = (CloseFunction) lib.resolve("CloseDLL");

	if (!closedll)
	{
		//qWarning("Error CloseDLL");
		textStatusBar->setText("Не найдена функция CloseDLL");
		return;
	}

    typedef bool (*initFunction)(int);
    bool answer = false;

    initFunction initDll = (initFunction) lib.resolve("InitDLL_Qt");
    if (initDll)
    {
        answer = initDll(8);
    }
    else
    {
		//qWarning("Error");
        textStatusBar->setText("Не найдена функция InitDLL_Qt");
        return;
    }

    if (answer==false)
    {
        textStatusBar->setText("Функция InitDLL_Qt вернула false");
		closedll();
        return;
    }

    typedef bool (*ConnectFunction)();
	typedef wchar_t* (*DebugFunction)();


    answer = false;
    ConnectFunction connectIni = (ConnectFunction) lib.resolve("dbConnectIni");
    DebugFunction debugDLL = (DebugFunction) lib.resolve("GetDebugInfo");
    if (connectIni)
    {
        answer = connectIni();
        if(debugDLL)
        {
            auto debugInfo=debugDLL();
            QString debugString = QString::fromWCharArray(debugInfo);
		   // qInfo(qUtf8Printable(debugString));
        }

    }
    else
    {
		//qWarning("Error");
        textStatusBar->setText("Не найдена функция dbConnectIni");
        closedll();
        return;
    }

    if (answer==false)
    {
        textStatusBar->setText("Функция dbConnectIni вернула false");
		closedll();
        return;
    }


    typedef bool (*SetDataFunction)(int,wchar_t*);
    SetDataFunction setData = (SetDataFunction) lib.resolve("SetDBDataString");

    if (setData)
    {
        answer = setData(0,convertedData);
		auto debugInfo=debugDLL();
		QString debugString = QString::fromWCharArray(debugInfo);
		//qInfo(qUtf8Printable(debugString));
    }
    else
    {
		//qWarning("Error SetDBDataString");
        textStatusBar->setText("Не найдена функция SetDBDataString");
		closedll();
        return;
    }


//    CloseDLL
    closedll();
    textStatusBar->setText("Успех");


}



void MainWindow::on_receiveDataBDButton_clicked()
{

//	QString str = "{\"bookHeavyInfo\":{\"Qty\":100},\"bookLightInfo\":{\"Qty\":2}}";
//	QJsonDocument jsonResponse = QJsonDocument::fromJson(str.toUtf8());

//	QJsonObject jsonObject = jsonResponse.object();

//	QJsonObject bookHeavyInfo = jsonObject["bookHeavyInfo"].toObject();

//	int qty = bookHeavyInfo["Qty"].toInt();

	QString JsonString = QString::fromWCharArray(convertedDataRecieve);

	QJsonDocument receiveDataJSON = QJsonDocument::fromJson(JsonString.toUtf8());

	QJsonObject JSONReceive = receiveDataJSON.object();

	QStringList allKeys = JSONReceive.keys();

	QJsonObject Array;
	int id, position;
	float Qin,Qout1,Qout2,Pin,Pout,deltaP;
	QJsonArray arrayQ;
	QJsonObject Q;
	QJsonObject P;

	for(int i=1; i<allKeys.size();i++)
	{
		Array = JSONReceive[allKeys[i]].toObject();

		id = Array["id"].toInt();
		position = Array["position_startup"].toInt();

		arrayQ=Array["result_math_model"].toArray();

		Q = arrayQ[0].toObject();
		Qin = Q["Q_in"].toDouble();
		Qout1 = Q["Q_out1"].toDouble();
		Qout2 = Q["Q_out2"].toDouble();

		P = arrayQ[1].toObject();
		Pin = P["P_in"].toDouble();
		Pout = P["P_out"].toDouble();
		deltaP = P["deltaP"].toDouble();
	}


//	QJsonValue Array = JSONReceive.value(allKeys[1]);
//	QJsonArray id = Array.toArray();
//	QJsonObject sub = Array["id"].toObject();


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

	QByteArray saveData = jsonFile.readAll();

	textStatusBar->setText("Успех");

//	// Выбираем файл
//		QString openFileName = QFileDialog::getOpenFileName(this,
//															tr("Open Json File"),
//															QString(),
//															tr("JSON (*.json)"));
//		QFileInfo fileInfo(openFileName);   // С помощью QFileInfo
//		QDir::setCurrent(fileInfo.path());  // установим текущую рабочую директорию, где будет файл
//		// Создаём объект файла и открываем его на чтение
//		QFile jsonFile(openFileName);
//		if (!jsonFile.open(QIODevice::ReadOnly))
//		{
//			return;
//		}

//		// Считываем весь файл
//		QByteArray saveData = jsonFile.readAll();
//		// Создаём QJsonDocument
//		QJsonDocument jsonDocument(QJsonDocument::fromJson(saveData));
//		// Из которого выделяем объект в текущий рабочий QJsonObject
//		m_currentJsonObject = jsonDocument.object();
//		// Очищаем текстовое поле
//		ui->jsonDocumentTextEdit->clear();
//		// И устанавливаем в проверочное текстовое поле содержимое Json объекта
//		ui->jsonDocumentTextEdit->setText(QJsonDocument(m_currentJsonObject).toJson(QJsonDocument::Indented));
}
