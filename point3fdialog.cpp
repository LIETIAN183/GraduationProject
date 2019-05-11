#include "point3fdialog.h"
#include "ui_point3fdialog.h"
#include <QIntValidator>
#include <QtDebug>

Point3fDialog::Point3fDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Point3fDialog)
{
    ui->setupUi(this);
    this->setFixedSize( this->width (), this->height ());
    this->setFocus();
    connect(this->ui->Btn_OK, SIGNAL(clicked()), this, SLOT(OK()));
    connect(this->ui->Btn_Cancel, SIGNAL(clicked()), this, SLOT(Cancel()));
}

Point3fDialog::~Point3fDialog()
{
    delete ui;
}

void Point3fDialog::SetNumber(int i, int number)
{
    if(i == 1)
    {
        this->ui->Editx->setValidator(new QIntValidator(-10000, 10000));
        this->ui->Editx->setText(QString::number(number));
    }
    else if (i == 2)
    {
        this->ui->Edity->setValidator(new QIntValidator(-10000, 10000));
        this->ui->Edity->setText(QString::number(number));
    }
    else if (i == 3)
    {
        this->ui->Editz->setValidator(new QIntValidator(-10000, 10000));
        this->ui->Editz->setText(QString::number(number));
    }
    qDebug() << i << "\t" << number << "\n";
}

int Point3fDialog::getNumber(int i)
{
    if(i == 1)
    {
        return this->ui->Editx->text().toInt();
    }
    else if(i == 2)
    {
        return this->ui->Edity->text().toInt();
    }
    else if(i == 3)
    {
        return this->ui->Editz->text().toInt();
    }
    else
    {
        return 0;
    }
}

void Point3fDialog::OK()
{
    QDialog::accept();
}

void Point3fDialog::Cancel()
{
    this->done(QDialog::Rejected);
}
