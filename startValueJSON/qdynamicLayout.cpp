#include "qdynamicLayout.h"

QDynamicLayout::QDynamicLayout(QWidget *parent,
                               int id,
                               QString name,
                               QString type,
                               int status) :
    deleteButton(parent),
    idEdit(parent),
    nameEdit(parent),
    typeEdit(parent),
    statusEdit(parent),
    table(parent),
    m_intValidator(1, 1000, this)
{


    deleteButton.setText("Удалить");
    idEdit.setText(QString::number(id));
    nameEdit.setText(name);
    typeEdit.setText(type);
    statusEdit.setText(QString::number(status));


    //Проверка, что вводит целочисленные значения
    idEdit.setValidator(&m_intValidator);
    statusEdit.setValidator(&m_intValidator);


    table.addWidget(&idEdit);
    table.addWidget(&nameEdit);
    table.addWidget(&typeEdit);
    table.addWidget(&statusEdit);
    table.addWidget(&deleteButton);
    setLayout(&table);

    connect(&deleteButton, SIGNAL(clicked()), this, SLOT(deleteAll()));

}


// Удаление Layout
void QDynamicLayout::deleteAll()
{
    this->hide();
    this->deleteLater();
}

QDynamicLayout::~QDynamicLayout()
{

}

