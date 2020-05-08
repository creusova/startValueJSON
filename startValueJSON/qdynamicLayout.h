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
    explicit QDynamicLayout(QWidget *parent = 0);
    virtual ~QDynamicLayout();


    QPushButton deleteButton;
    QLineEdit idNumber;
	QLineEdit positionStart;
    QLineEdit Q_in;
    QLineEdit Q_out1;
    QLineEdit Q_out2;
    QLineEdit P_in;
    QLineEdit P_out;
    QLineEdit deltaP;

public slots:
    void deleteAll();
private:
    QHBoxLayout table;
    QDoubleValidator m_doubleValidator;
    QIntValidator m_intValidator;
};


#endif
