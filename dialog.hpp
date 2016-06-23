#ifndef DIALOG_HPP
#define DIALOG_HPP

#include <QDialog>
#include <QModelIndex>
#include <qmath.h>
#include "section.hpp"


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_getSectCmd_clicked();

    void on_pushButton_clicked();

    void on_listWidget_doubleClicked();

    void writefile();

private:
    Ui::Dialog *ui;
  };

#endif //DIALOG_HPP
