#ifndef RESULTDIALOG_H
#define RESULTDIALOG_H
#include "equation.h"
#include <QDialog>
#include <QVector>
#include <QString>
namespace Ui {
class ResultDialog;
}

class ResultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResultDialog(QWidget *parent = 0);
    ~ResultDialog();
    void showResult(QVector<QVector<DigitMap>> res);

private:
    void paintEvent(QPaintEvent *);
    Ui::ResultDialog *ui;
};

#endif // RESULTDIALOG_H
