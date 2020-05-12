#include "qdynamicLayout.h"

QDynamicLayout::QDynamicLayout(QWidget *parent,
                               int id,
                               int position,
                               double Q_inValue,
                               double Q_out1Value,
                               double Q_out2Value,
                               double P_inValue,
                               double P_outValue,
                               double deltaPValue) :
    deleteButton(parent),
    idNumber(parent),
	positionStart(parent),
    Q_in(parent),
    Q_out1(parent),
    Q_out2(parent),
    P_in(parent),
    P_out(parent),
    deltaP(parent),
    table(parent),
    m_doubleValidator(-100,1000,6,this),
    m_intValidator(1, 1000, this)
{


    deleteButton.setText("Удалить");
    idNumber.setText(QString::number(id));
    positionStart.setText(QString::number(position));
    Q_in.setText(QString::number(Q_inValue));
    Q_out1.setText(QString::number(Q_out1Value));
    Q_out2.setText(QString::number(Q_out2Value));
    P_in.setText(QString::number(P_inValue));
    P_out.setText(QString::number(P_outValue));
    deltaP.setText(QString::number(deltaPValue));

    m_doubleValidator.setNotation(QDoubleValidator::StandardNotation);
    idNumber.setValidator(&m_intValidator);
	positionStart.setValidator(&m_intValidator);
    Q_in.setValidator(&m_doubleValidator);
    Q_out1.setValidator(&m_doubleValidator);
    Q_out2.setValidator(&m_doubleValidator);
    P_in.setValidator(&m_doubleValidator);
    P_out.setValidator(&m_doubleValidator);
    deltaP.setValidator(&m_doubleValidator);


    table.addWidget(&idNumber);
	table.addWidget(&positionStart);
    table.addWidget(&Q_in);
    table.addWidget(&Q_out1);
    table.addWidget(&Q_out2);
    table.addWidget(&P_in);
    table.addWidget(&P_out);
    table.addWidget(&deltaP);
    table.addWidget(&deleteButton);
    setLayout(&table);

    connect(&deleteButton, SIGNAL(clicked()), this, SLOT(deleteAll()));

}

void QDynamicLayout::deleteAll()
{
    this->hide();
    this->deleteLater();
}

QDynamicLayout::~QDynamicLayout()
{

}

