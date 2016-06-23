#include "dialog.hpp"
#include "ui_dialog.h"
#include <QList>
#include <QMessageBox>
#include <memory>
#include "forcefactor.hpp"
#include <QString>
#include <QFile>
#include <QTextStream>

using namespace std;

// Обзываем строковое данное куда кидаем весь текст
QString MyStringToFile;

// Обзывает и создаем список точек трассы

typedef QList<Section *> iTrace;

QList<Section *> myTrace;


double SemiBeta = 0;
double SemiIks = 0;
double SemiLenght = 0;

// коофициент запаса прочности ленты
double Kd = 0;
double Kz = 0;

int POSITION = 0;




Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    MyStringToFile.push_back(QString::fromUtf8("Тяговый расчет ленточного конвейера \n"));
}

Dialog::~Dialog()
{
    delete ui;
}



//ЗДЕСЬ ПРОБУЕМ РЕАЛИЗОВАТЬ ФАЙЛОВЫЙ ВЫВОД

void Dialog::writefile()
{
    QString filename = "output.doc";
    QFile file(filename);
    file.open(QIODevice::WriteOnly|QIODevice::Text);
    QTextStream out(&file);
    out << MyStringToFile << endl;
    file.close();
}

//_________________________________________
//_________________________________________




void Dialog::on_getSectCmd_clicked()
{



// Ввожу нагрузку, длинну, и угол трассы.
// Сохраняю в список каждую точку

 QString SecQgr = ui->lineEditQgr->text();
 QString SecLenght = ui->lineEditLenght->text();
 QString SecAngle = ui->lineEditBeta->text();
 bool ok;
 double Qgr = SecQgr.toDouble(&ok);
 if (!ok)
 {
       QMessageBox::information(this, QString::fromUtf8("Ошибка"), QString::fromUtf8("Введите кооректные данные"));
       return;
 }

 MyStringToFile.push_back(QString::fromUtf8("Вносим значение поступающего груза ") + QString::number(Qgr) + QString::fromUtf8(" "));

 double lenght = SecLenght.toDouble(&ok);
 if (!ok)
 {
     QMessageBox::information(this, QString::fromUtf8("Ошибка"), QString::fromUtf8("Введите кооректные данные"));
     return;
 }

  MyStringToFile.push_back(QString::fromUtf8("Вносим значение длинны участка ") + QString::number(lenght) + QString::fromUtf8(" "));

 float angle = SecAngle.toDouble(&ok);
 if (!ok)
 {
     QMessageBox::information(this, QString::fromUtf8("Ошибка"), QString::fromUtf8("Введите кооректные данные"));
     return;
 }

  MyStringToFile.push_back(QString::fromUtf8("Вносим значение угла наклона участка ") + QString::number(angle) + QString::fromUtf8("\n"));

myTrace.append(new Section(lenght, Qgr, angle));

// Выводим на экране количество записей и значения погонной нагрузки
// которая прибавляется на данном участке. Если никто в этом месте не сыпет - значит НОЛЬ

ui->lineEditCountSect->setText(QString::number(myTrace.count()));

ui->listWidget->clear();

for(int i = 0; i < myTrace.count(); i++)
{
    double element = myTrace[i]->GetQgr();


ui->listWidget->addItem(QString::number(element));

}


// Расчет среднего угла трассы


for(int i = 0; i < myTrace.count(); i++)
{
    SemiIks += myTrace.at(i)->GetSinus() * myTrace.at(i)->GetLenght();
    SemiLenght += myTrace.at(i)->GetLenght();
}

SemiBeta = qAsin(SemiIks/SemiLenght);

 MyStringToFile.push_back(QString::fromUtf8("Определяем средний угол трассы конвейера ") + QString::number(SemiBeta) + QString::fromUtf8("\n"));

}



void Dialog::on_pushButton_clicked()
{
  // Вводятся погонные массы роликов и ленты
    QString WvQLenta = ui->lineEditQl->text();
    QString WvQr1 = ui->lineEditQr1->text();
    QString WvQr2 = ui->lineEditQr2->text();
    QString QWBelt = ui->lineEditWBelt->text();
    QString QSpeed = ui->lineEditSpeed->text();

  //Обрабатываются введенные строки в числовые значения
    bool ok;
    double QLenta = WvQLenta.toDouble(&ok);
    if (!ok)
    {
        QMessageBox::information(this, QString::fromUtf8("Ошибка"), QString::fromUtf8("Введите кооректные данные"));
        return;
    }

    double Qr1 = WvQr1.toDouble(&ok);
    if (!ok)
    {
        QMessageBox::information(this, QString::fromUtf8("Ошибка"), QString::fromUtf8("Введите кооректные данные"));
        return;
    }

    double Qr2 = WvQr2.toDouble(&ok);
    if (!ok)
    {
        QMessageBox::information(this, QString::fromUtf8("Ошибка"), QString::fromUtf8("Введите кооректные данные"));
        return;
    }

    double WBelt = QWBelt.toDouble(&ok);
    if (!ok)
    {
        QMessageBox::information(this, QString::fromUtf8("Ошибка"), QString::fromUtf8("Введите кооректные данные"));
        return;
    }

    double Speed = QSpeed.toDouble(&ok);
    if (!ok)
    {
        QMessageBox::information(this, QString::fromUtf8("Ошибка"), QString::fromUtf8("Введите кооректные данные"));
        return;
    }

    double KoophC = 0;
    if(SemiLenght > 0 && SemiLenght <= 50)
        KoophC = 3.5;

    if(SemiLenght > 50 && SemiLenght <= 100)
        KoophC = 2.5;

    if(SemiLenght > 100 && SemiLenght <= 250)
        KoophC = 1.4;

    if(SemiLenght > 250 && SemiLenght <= 500)
        KoophC = 1.2;

    if (SemiLenght > 500 && SemiLenght < 1000)
        KoophC = 1.1;

    if (SemiLenght >= 1000)
        KoophC = 1;



    // массивы умных указателей ????????

    int Counter = myTrace.count();



    std::unique_ptr<double[]> WvResist(new double[Counter]);

    std::unique_ptr<double[]> WnResist(new double[Counter]);
    std::unique_ptr<double[]> ThisQgr(new double[Counter]);

  //В массивы заносим поочередно расчитываемые сопротивления движению верхней и нижней ветвей

    double WVObsh = 0;
    double WNObsh = 0;

    double MakeThisQgr(int x, iTrace & y);
    double MakeMaxQgr(iTrace & y);
     double FindMax(std::unique_ptr<double[]> & mass, int count);

    for(int i = 0; i < myTrace.count(); i++)
    {

         ThisQgr[i] = MakeThisQgr(i, myTrace);

  // Формула Wв = (Qгр + Qл + Qр1) * W * C * L + sin(B) * (Qгр + Qл) * L

         WvResist[i] = (ThisQgr[i] + QLenta + Qr1) * 0.032 * KoophC * myTrace[i]->GetLenght() + myTrace[i]->GetSinus() * (ThisQgr[i] + QLenta) * myTrace[i]->GetLenght();

  // Формула Wн = [(Qл + Qр2) * W - sin(B) * Qл] * L

         WnResist[i] = ((QLenta + Qr2) * 0.032 - myTrace[i]->GetSinus() * QLenta) * myTrace[i]->GetLenght();

    }


  // Суммируем поочередно сопротивления верхней, а затем и нижней ветвей

    for(int y = 0; y < myTrace.count(); y++)
    {
        WVObsh += WvResist[y];
    }


    for(int z = 0; z < myTrace.count(); z++)
    {
        WNObsh += WnResist[z];
    }

  // Получаем необходимое тяговое усилие привода

    double WZero = WVObsh + WNObsh;
// !!! Разделить формулы !!!
    double NPotr = (WZero * Speed) / (102 * 0.85);
    double NUst = NPotr * 1.5; // Здесь необходимо рассчитать коэффициент запаса мощности


    // пробуем вызвать тяговый фактор

    ForceFactor AFack;

    double myForceFactor;


    if(AFack.exec())
    {
        myForceFactor = AFack.GetForceFact();

// вычисляем тяговый фактор
    }

    // получаем минимальное натяжение по отсутствию пробуксовки

    double S1min = (WZero * 1.4) / myForceFactor - 1; //Здесь внедрен Тяговый фактор!


    double MaxQgr = MakeMaxQgr(myTrace);
    double S2min = 8 * 1.5 * (MaxQgr + QLenta);





    if(SemiBeta >= -3 && SemiBeta < 3)
        Kd = 0.83;

    if(SemiBeta >= 3 && SemiBeta < 10)
        Kd = 0.8;

    if(SemiBeta >= 10 && SemiBeta < 18)
        Kd = 0.78;


    int TYPE;
    if(ui->rbHard->isChecked())
    {
        TYPE = 1;
    }

    if(ui->rbMenHard->isChecked())
    {
        TYPE = 2;
    }

    switch(TYPE)   //ЗДЕСЬ ВЫБОР ИЗ ФОРМЫ (ПЕРЕКЛЮЧАТЕЛЬ) ТИПА КОНВЕЙЕРА
    {
    case 1:
    {
        if(SemiBeta <= 10)
            Kz = 8.5;
        if(SemiBeta > 10)
            Kz = 9;
        break;
    }
    case 2:
    {
        if(SemiBeta <= 10)
            Kz = 9.5;
        if(SemiBeta > 10)
            Kz = 10;
        break;
    }
    default:
        break;
    }



    if(NPotr >= 0)
    {

// _____________________________________________________________________________________________
// ЗДЕСЬ ПРОИЗВОДИТСЯ РАСЧЕТ ГОРИЗОНТАЛЬНОГО КОНВЕЙЕРА
// _____________________________________________________________________________________________

  // Начинаю считать по характерным точкам трассы

  int DoubleCounter = (Counter * 2) + 2;

  std::unique_ptr<double[]> Stochka(new double[DoubleCounter]);
  double Natjag = S1min;

  Stochka[0] = Natjag;

  int alfa = 1; // Некий счетчик массива расчетов точек трассы

  std::unique_ptr<double[]> RaznSmin(new double[DoubleCounter]);

  for(int i = 0; i < Counter; i++, alfa++)
  {
      Natjag += WnResist[i];
      Stochka[alfa] = Natjag;
      if(Natjag < S2min)
          RaznSmin[alfa] = S2min - Natjag;
      else
          RaznSmin[alfa] = 0;
  }
  for (int i = Counter; i >= 0; i--, alfa++)
  {
      Natjag += WvResist[i];
      Stochka[alfa] = Natjag;

      if(Natjag < S2min)
          RaznSmin[alfa] = S2min - Natjag;
      else
          RaznSmin[alfa] = 0;
  }

  double MaxRaznSmin = FindMax(RaznSmin, DoubleCounter);

  for(int i = 0; i < DoubleCounter; i++)
  {
      Stochka[i] += MaxRaznSmin;
  }
// __________________________________________________________________________________

// __________________________________________________________________________________

  // Теперь тот же просчет но по минимальному натяжению по провисанию

  std::unique_ptr<double[]> Stochka2(new double[Counter]);
  std::unique_ptr<double[]> Stochka3(new double[Counter]);
  double Natjag2 = S2min;
  double Natjag3 = S2min;

  Stochka2[0] = Natjag2;


  for(int i = 1; i < Counter; i++)
  {
      Natjag2 += WvResist[i];
      Stochka2[i] = Natjag2;
  }

  Stochka3[0] = Natjag3;

  for(int i = 0; i < Counter; i++)
  {
      Natjag3 -= WnResist[i];
      Stochka3[i] = Natjag2;
  }


// ___________________________________________________________________________

// ___________________________________________________________________________

  ui->lstNatjag->clear();
  for(int i = 0; i < DoubleCounter; i++)
  {
    double snatjag = Stochka[i];
    ui->lstNatjag->addItem(QString::number(snatjag));
  }



  // Теперь выбираем наибольшее значение натяжения и принимаем его как максимальное

  double SMaxS1min = 0;

  SMaxS1min = FindMax(Stochka, DoubleCounter);

  double SMaxS21min = 0;

  SMaxS21min = FindMax(Stochka2, Counter);

  double SMaxS22min = 0;

  SMaxS22min = FindMax(Stochka3, Counter);

  double SMaxS2min = 0;
  if(SMaxS21min > SMaxS22min)
      SMaxS2min = SMaxS21min;
  else
      SMaxS2min = SMaxS22min;

  double SMax = 0;

  if(SMaxS1min > SMaxS2min)
      SMax = SMaxS1min;
  else
      SMax = SMaxS2min;



// Начинаем просчитывать разрывное

  double SRazryv = SMax * (Kz * Kd); // Здесь необходимо рассчитать коэффициент запаса прочности ленты

  double PROCHNOST = SRazryv / WBelt; // Здесь необходимо ввести ширину ленты в сантиметрах


    double result = PROCHNOST;
    ui->lineEditPROCHNOST->setText(QString::number(result));
    ui->lineEditMOSHNOST->setText(QString::number(NUst));



    }

    else
    {
        // ___________________________________________________________________
        // ЗДЕСЬ ПРОИЗВОДИТСЯ РАСЧЕТ БРЕМСБЕРГОВОГО КОНВЕЙЕРА

        if(ui->rbUp->isChecked())
        {
            POSITION = 1;
        }

        if(ui->rbDown->isChecked())
        {
            POSITION = 2;
        }



        switch(POSITION)
        {
        case 1:
        {
            //ПРИВОД В ВЕРХНЕЙ ЧАСТИ
            int DCounter = Counter + 1;
            std::unique_ptr<double[]> SBtochka1(new double[DCounter]);
            double BNatjag1 = S2min;

            SBtochka1[0] = BNatjag1;
                int zero = 1;
            for(int i = DCounter-2; i >= 0; i--, zero++)
            {
                BNatjag1 -= WvResist[i];
                SBtochka1[zero] = BNatjag1;
            }

            std::unique_ptr<double[]> SBtochka2(new double[DCounter]);
            double BNatjag2 = S2min;

            SBtochka2[0] = BNatjag2;
            zero = 1;
            for(int i = DCounter-2; i >= 0; i--, zero++)
            {
                BNatjag2 += WnResist[i];
                SBtochka2[zero] = BNatjag2;
            }


            ui->lstNatjag2->clear();
            for(int i = 0; i < DCounter; i++)
            {
              double snatjag = SBtochka1[i];
              ui->lstNatjag2->addItem(QString::number(snatjag));
            }
            for(int i = 0; i < DCounter; i++)
            {
              double snatjag = SBtochka2[i];
              ui->lstNatjag2->addItem(QString::number(snatjag));
            }

            double SMax1 = FindMax(SBtochka1, DCounter);
            double SMax2 = FindMax(SBtochka2, DCounter);
            double SMax = 0;
            if (SMax1 >= SMax2)
                SMax = SMax1;
            else
                SMax = SMax2;

            double SRazryv = SMax * (Kz * Kd); // Здесь необходимо рассчитать коэффициент запаса прочности ленты

            double PROCHNOST = SRazryv / WBelt; // Здесь необходимо ввести ширину ленты в сантиметрах


              double result = PROCHNOST;
              ui->lineEditPROCHNOST->setText(QString::number(result));
              ui->lineEditMOSHNOST->setText(QString::number(NUst));



              break;

        }
        case 2:
        {
            // ПРИВОД В НИЖНЕЙ ЧАСТИ
            int DoubleCounter2 = (Counter * 2) + 1;
            std::unique_ptr<double[]> SBtochkaN2(new double[DoubleCounter2]);
            double BNatjagN2 = 0;
            if(S2min > S1min)
                BNatjagN2 = S2min;
            else
                BNatjagN2 = S1min;

            SBtochkaN2[0] = BNatjagN2;

            int alfa = 1;
            for(int i = 0; i < Counter; i++, alfa++)
            {
                BNatjagN2 -= WvResist[i];
                SBtochkaN2[alfa] = BNatjagN2;
            }
            for(int i = myTrace.count(); i > 0; i--, alfa++)
            {
                BNatjagN2 -= WnResist[i];
                SBtochkaN2[alfa] = BNatjagN2;
            }

            double SMax = FindMax(SBtochkaN2, DoubleCounter2);

            double SRazryv = SMax * (Kz * Kd); // Здесь необходимо рассчитать коэффициент запаса прочности ленты

            double PROCHNOST = SRazryv / WBelt; // Здесь необходимо ввести ширину ленты в сантиметрах


              double result = PROCHNOST;
              ui->lineEditPROCHNOST->setText(QString::number(result));
              ui->lineEditMOSHNOST->setText(QString::number(NUst));

              break;


        }
        }



        // ____________________________________________________________________
    }


ui->lineEditPROCHNOST->update();
ui->lineEditMOSHNOST->update();

QString PuToFile = ui->lineEditPROCHNOST->text();
QString MoToFile = ui->lineEditMOSHNOST->text();

MyStringToFile.push_back(QString::fromUtf8("Необходимая прочность конвейерной ленты при данных условиях эксплуатации равна ") + PuToFile + QString::fromUtf8("Н/мм \n"));
MyStringToFile.push_back(QString::fromUtf8("Необходимая мощность привода при этом равна ") + MoToFile + QString::fromUtf8("КВт \n"));

writefile();

}

double MakeThisQgr(int x, iTrace & y)
{
    int z = y[x]->GetNumber();
    double MyThisQgr = 0;
    for(int i = 0; i < z; i++)
    {
        MyThisQgr += y[i]->GetQgr();
    }
    return MyThisQgr;
}

double MakeMaxQgr(iTrace & y)
{
    double myQ = 0;
    for(int i = 0; i < y.count(); i++)
    {
        myQ += y[i]->GetQgr();
    }
    return myQ;
}

double FindMax(std::unique_ptr<double[]> & mass, int count)
{
    if (count < 1)
        return 0;
    double max = mass[0];

    for(int i = 1; i < count; i++)
    {
        if (mass[i] > max)
        {
            max = mass[i];
        }
    }
    return max;
}



