#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QGridLayout>
#include <QLabel>
#include <QLibrary>

#include <qdynamicLayout.h>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QFileDialog>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_addButton_clicked();                // Обработчик нажатия кнопки добавления Layout "Добавить"
    void on_JSONButton_clicked();               // Обработчик нажатия кнопки отправки JSON файла "Сформировать JSON"
    void on_receiveDataBDButton_clicked();      // Обработчик нажатия кнопки получить JSON с базы данных "Получить данные с базы"
    void on_receiveDataFileButton_clicked();    // Обработчик нажатия кнопки прочитать JSON из файла "Получить данные из файла"
    void on_addLib_clicked();                   // Обработчик нажатия кнопки подлючить библиотеку
    void on_closeLib_clicked();                 // Обработчик нажатия кнопки отключить библиотеку

private:
    Ui::MainWindow *ui;

    QVBoxLayout * m_verticalLayout;             // Скролл для поля где добавляются Layout
    QLabel * textStatusBar;                     // Статус бар внизу


// Указатели на библиотечные функции
    typedef bool (*CloseFunction)();
    CloseFunction closedll;
    typedef bool (*initFunction)(int);
    initFunction initDll;
    typedef bool (*ConnectFunction)();
    ConnectFunction connectIni;
    typedef bool (*SetDataFunction)(int,wchar_t*);
    SetDataFunction setData;
    typedef wchar_t* (*GetDataFunction)(int);
    GetDataFunction getData;


    void parsingJSON(QString &);


 // Проверка, есть ли указатель на функции отключения от библиотеке. Если есть, то закрываем dll.
    bool closedllWithCheck()
    {
        if(closedll)
        {
            return closedll();
        }
        return false;
    }
};

#endif // MAINWINDOW_H


