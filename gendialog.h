#ifndef GENDIALOG_H
#define GENDIALOG_H

#include <QDialog>

namespace Ui {
class GenDialog;
}

class GenDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GenDialog(QWidget *parent = 0);
    ~GenDialog();

private slots:
    void on_btnOk_clicked();

private:
    void paintEvent(QPaintEvent *);
    Ui::GenDialog *ui;
};

#endif // GENDIALOG_H
