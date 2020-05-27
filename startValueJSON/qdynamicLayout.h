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
                   int statusElement = 0);
    virtual ~QDynamicLayout();


    QPushButton deleteButton;
    QLineEdit idEdit;
    QLineEdit nameEdit;
    QLineEdit typeEdit;
    QLineEdit statusEdit;

public slots:
    void deleteAll();
private:
    QHBoxLayout table;
    QIntValidator m_intValidator;




};


#endif
