#include "qdynamicLayout.h"

QDynamicLayout::QDynamicLayout(QWidget *parent) :
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
    m_doubleValidator(-100,1000,2,this),
    m_intValidator(1, 1000, this)
{


    deleteButton.setText("Удалить");
    idNumber.setText("0");
	positionStart.setText("0");
    Q_in.setText("0");
    Q_out1.setText("0");
    Q_out2.setText("0");
    P_in.setText("0");
    P_out.setText("0");
    deltaP.setText("0");

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

