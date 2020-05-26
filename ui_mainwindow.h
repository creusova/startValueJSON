/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *verticalLayoutWidget_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *addButton;
    QPushButton *addLib;
    QPushButton *JSONButton;
    QScrollArea *Scroll;
    QWidget *scrollAreaWidgetContents;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QLabel *label_2;
    QLabel *label;
    QLabel *label_6;
    QLabel *label_5;
    QLabel *label_4;
    QLabel *label_8;
    QLabel *label_9;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_7;
    QLineEdit *numberTrening;
    QPushButton *receiveDataBDButton;
    QPushButton *receiveDataFileButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(730, 650);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayoutWidget_2 = new QWidget(centralwidget);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(10, 20, 343, 25));
        horizontalLayout = new QHBoxLayout(verticalLayoutWidget_2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        addButton = new QPushButton(verticalLayoutWidget_2);
        addButton->setObjectName(QString::fromUtf8("addButton"));

        horizontalLayout->addWidget(addButton);

        addLib = new QPushButton(verticalLayoutWidget_2);
        addLib->setObjectName(QString::fromUtf8("addLib"));

        horizontalLayout->addWidget(addLib);

        JSONButton = new QPushButton(verticalLayoutWidget_2);
        JSONButton->setObjectName(QString::fromUtf8("JSONButton"));

        horizontalLayout->addWidget(JSONButton);

        Scroll = new QScrollArea(centralwidget);
        Scroll->setObjectName(QString::fromUtf8("Scroll"));
        Scroll->setGeometry(QRect(30, 89, 671, 500));
        Scroll->setLayoutDirection(Qt::LeftToRight);
        Scroll->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContentsOnFirstShow);
        Scroll->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 669, 498));
        Scroll->setWidget(scrollAreaWidgetContents);
        horizontalLayoutWidget = new QWidget(centralwidget);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(60, 60, 541, 21));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(horizontalLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_2->addWidget(label_3);

        label_2 = new QLabel(horizontalLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        label = new QLabel(horizontalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        label_6 = new QLabel(horizontalLayoutWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_2->addWidget(label_6);

        label_5 = new QLabel(horizontalLayoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_2->addWidget(label_5);

        label_4 = new QLabel(horizontalLayoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_2->addWidget(label_4);

        label_8 = new QLabel(horizontalLayoutWidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        horizontalLayout_2->addWidget(label_8);

        label_9 = new QLabel(horizontalLayoutWidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        horizontalLayout_2->addWidget(label_9);

        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(360, 20, 141, 31));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_7 = new QLabel(layoutWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_3->addWidget(label_7);

        numberTrening = new QLineEdit(layoutWidget);
        numberTrening->setObjectName(QString::fromUtf8("numberTrening"));

        horizontalLayout_3->addWidget(numberTrening);

        receiveDataBDButton = new QPushButton(centralwidget);
        receiveDataBDButton->setObjectName(QString::fromUtf8("receiveDataBDButton"));
        receiveDataBDButton->setGeometry(QRect(510, 10, 101, 41));
        receiveDataFileButton = new QPushButton(centralwidget);
        receiveDataFileButton->setObjectName(QString::fromUtf8("receiveDataFileButton"));
        receiveDataFileButton->setGeometry(QRect(620, 10, 101, 41));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 730, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        addButton->setText(QCoreApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214", nullptr));
        addLib->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\264\320\272\320\273\321\216\321\207\320\270\321\202\321\214 \320\261\320\270\320\261\320\273\320\270\320\276\321\202\320\265\320\272\321\203", nullptr));
        JSONButton->setText(QCoreApplication::translate("MainWindow", "\320\241\321\204\320\276\321\200\320\274\320\270\321\202\321\200\320\276\320\262\320\260\321\202\321\214 JSON", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Id", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Position Start", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "   Q_in", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Q_out1", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Q_out2", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "P_in", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "P_out", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "deltaP", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Trening Number", nullptr));
        receiveDataBDButton->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\273\321\203\321\207\320\270\321\202\321\214 \320\264\320\260\320\275\320\275\321\213\320\265\n"
" \321\201 \320\261\320\260\320\267\321\213", nullptr));
        receiveDataFileButton->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\273\321\203\321\207\320\270\321\202\321\214 \320\264\320\260\320\275\320\275\321\213\320\265\n"
" \321\201 \321\204\320\260\320\271\320\273\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
