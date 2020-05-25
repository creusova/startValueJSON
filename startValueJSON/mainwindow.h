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
    void on_addButton_clicked();    // СЛОТ-обработчик нажатия кнопки добавления
    void on_JSONButton_clicked(); // СЛОТ-обработчик нажатия кнопки удаления
	void on_receiveDataBDButton_clicked();           // СЛОТ для получения номера нажатой динамической кнопки
	void on_receiveDataFileButton_clicked();
    void on_addLib_clicked();

private:
    Ui::MainWindow *ui;

    QVBoxLayout * m_verticalLayout;
    QLabel * textStatusBar;
};

#endif // MAINWINDOW_H


