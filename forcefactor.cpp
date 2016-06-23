#include "forcefactor.hpp"
#include "ui_forcefactor.h"
#include <cmath>
#include <QMessageBox>

ForceFactor::ForceFactor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ForceFactor)
{
    ui->setupUi(this);
    ui->groupBox->setVisible(false);
    ui->groupBox_2->setVisible(false);
}

ForceFactor::~ForceFactor()
{
    delete ui;
}

void ForceFactor::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

double ForceFactor::GetForceFact()
{
    if(ui->rbMono->isChecked())
    {
    QString myMono = ui->leMono->text();
    bool ok;
    double mono = myMono.toDouble(&ok);
    if(!ok)
    {
        QMessageBox::information(this, QString::fromUtf8("Ошибка"), QString::fromUtf8("Введите кооректные данные"));
        return -1;
    }
    ForceFact = exp(((mono * 3.14) / 180) * 0.25); //уточнить коэффициент сопротивления материалов
    }
    if(ui->rbDuo->isChecked())
    {
    QString myDuo1 = ui->leDuo1->text();
    QString myDuo2 = ui->leDuo2->text();
    bool ok;
    double duo1 = myDuo1.toDouble(&ok);
    if(!ok)
    {
        QMessageBox::information(this, QString::fromUtf8("Ошибка"), QString::fromUtf8("Введите кооректные данные"));
        return -1;
    }

    double duo2 = myDuo2.toDouble(&ok);
    if(!ok)
    {
        QMessageBox::information(this, QString::fromUtf8("Ошибка"), QString::fromUtf8("Введите кооректные данные"));
        return -1;
    }

    ForceFact = exp(((duo1 * 3.14) / 180) * 0.25) * exp(((duo2 * 3.14) / 180) * 0.25);
    }

    if(ui->rbMulti->isChecked())
    {
        QString myPower = ui->lePower->text();
        QString myObxvat = ui->leObxvat->text();
        QString myFullPower = ui->leFullPower->text();

        bool ok;

        double power = myPower.toDouble(&ok);
        if(!ok)
        {
            QMessageBox::information(this, QString::fromUtf8("Ошибка"), QString::fromUtf8("Введите кооректные данные"));
            return -1;
        }

        double obxvat = myObxvat.toDouble(&ok);
        if(!ok)
        {
            QMessageBox::information(this, QString::fromUtf8("Ошибка"), QString::fromUtf8("Введите кооректные данные"));
            return -1;
        }

        double fullpower = myFullPower.toDouble(&ok);
        if(!ok)
        {
            QMessageBox::information(this, QString::fromUtf8("Ошибка"), QString::fromUtf8("Введите кооректные данные"));
            return -1;
        }

        double koeff = (fullpower - power) / power;
        ForceFact = exp(((obxvat * 3.14) / 180) * 0.25) * (koeff + 1) - koeff;
    }

    return ForceFact;
}


void ForceFactor::on_rbMono_clicked(bool checked)
{
    if(checked)
    {
        ui->groupBox->setEnabled(false);
        ui->groupBox->setVisible(false);
        ui->groupBox_3->setEnabled(false);
        ui->groupBox_3->setVisible(false);
        ui->groupBox_2->setEnabled(true);
        ui->groupBox_2->setVisible(true);
    }
    else
    {
        ui->groupBox_2->setEnabled(false);
        ui->groupBox_2->setVisible(false);
    }
}



void ForceFactor::on_rbDuo_clicked(bool checked)
{
    if(checked)
    {
        ui->groupBox->setEnabled(false);
        ui->groupBox->setVisible(false);
        ui->groupBox_3->setEnabled(true);
        ui->groupBox_3->setVisible(true);
        ui->groupBox_2->setEnabled(false);
        ui->groupBox_2->setVisible(false);
    }
    else
    {
        ui->groupBox_3->setEnabled(false);
        ui->groupBox_3->setVisible(false);
    }
}



void ForceFactor::on_rbMulti_clicked(bool checked)
{
    if(checked)
    {
        ui->groupBox->setEnabled(true);
        ui->groupBox->setVisible(true);
        ui->groupBox_3->setEnabled(false);
        ui->groupBox_3->setVisible(false);
        ui->groupBox_2->setEnabled(false);
        ui->groupBox_2->setVisible(false);
    }
    else
    {
        ui->groupBox->setEnabled(false);
        ui->groupBox->setVisible(false);
    }
}
