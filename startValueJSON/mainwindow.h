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
};

#endif // MAINWINDOW_H


