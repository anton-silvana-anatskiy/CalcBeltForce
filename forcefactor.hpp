#ifndef FORCEFACTOR_HPP
#define FORCEFACTOR_HPP

#include <QDialog>

namespace Ui {
class ForceFactor;
}

class ForceFactor : public QDialog
{
    Q_OBJECT

public:
    explicit ForceFactor(QWidget *parent = 0);
    ~ForceFactor();
     double GetForceFact();

protected:
    void changeEvent(QEvent *e);

private slots:

    void on_rbMono_clicked(bool checked);

    void on_rbDuo_clicked(bool checked);

    void on_rbMulti_clicked(bool checked);

private:
    Ui::ForceFactor *ui;
    double ForceFact;
};

#endif // FORCEFACTOR_HPP
