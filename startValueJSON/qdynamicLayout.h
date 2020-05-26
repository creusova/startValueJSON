#ifndef QDYNAMICLAYOT_H
#define QDYNAMICLAYOT_H

#include <QPushButton>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QIntValidator>


class QDynamicLayout : public QWidget
{
    Q_OBJECT
public:
    QDynamicLayout(QWidget *parent = 0,
                   int idNumber = 0,
                   QString nameElement = "F500",
                   QString typeElement ="valve",
                   int statusElement = 0
                  /* double Q_in = 0,
                   double Q_out1 = 0,
                   double Q_out2 = 0,
                   double P_in = 0,
                   double P_out = 0,
                   double deltaP = 0*/);
    virtual ~QDynamicLayout();


    QPushButton deleteButton;
    QLineEdit idEdit;
    QLineEdit nameEdit;
    QLineEdit typeEdit;
    QLineEdit statusEdit;
//	QLineEdit positionStart;
//    QLineEdit Q_in;
//    QLineEdit Q_out1;
//    QLineEdit Q_out2;
//    QLineEdit P_in;
//    QLineEdit P_out;
//    QLineEdit deltaP;

public slots:
    void deleteAll();
private:
    QHBoxLayout table;
    QDoubleValidator m_doubleValidator;
    QIntValidator m_intValidator;
};


#endif
