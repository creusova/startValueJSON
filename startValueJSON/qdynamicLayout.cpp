#include "qdynamicLayout.h"

QDynamicLayout::QDynamicLayout(QWidget *parent,
                               QString id,
                               QString name,
                               QString type,
                               QString status) :
    deleteButton(parent),
    idEdit(parent),
    nameEdit(parent),
    typeEdit(parent),
    statusEdit(parent),
    table(parent),
    m_doubleValidator(-100,1000,6,this),
    m_intValidator(1, 1000, this)
{


    deleteButton.setText("Удалить");
    idEdit.setText(id);
    nameEdit.setText(name);
    typeEdit.setText(type);
    statusEdit.setText(status);


//    m_doubleValidator.setNotation(QDoubleValidator::StandardNotation);
    idEdit.setValidator(&m_intValidator);
    statusEdit.setValidator(&m_intValidator);
//	positionStart.setValidator(&m_intValidator);
//    Q_in.setValidator(&m_doubleValidator);
//    Q_out1.setValidator(&m_doubleValidator);
//    Q_out2.setValidator(&m_doubleValidator);
//    P_in.setValidator(&m_doubleValidator);
//    P_out.setValidator(&m_doubleValidator);
//    deltaP.setValidator(&m_doubleValidator);


    table.addWidget(&idEdit);
    table.addWidget(&nameEdit);
    table.addWidget(&typeEdit);
    table.addWidget(&statusEdit);
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

