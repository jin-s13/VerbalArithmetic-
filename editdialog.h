#ifndef EDITDIALOG_H
#define EDITDIALOG_H
#include <QMovie>
#include <QDialog>
#include <iostream>
#include <QLabel>
#include <QLineEdit>
namespace Ui {
class EditDialog;
}

class EditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditDialog(QWidget *parent = 0);
    ~EditDialog();

private slots:
    void on_comboBox_currentIndexChanged(int index);
    void on_btnOk_clicked();
    void on_btnCancel_clicked();

private:
    void paintEvent(QPaintEvent *);
    Ui::EditDialog *ui;
    QLabel* lbl[4];
    QLineEdit* edit[4];
    int exp;
};

#endif // EDITDIALOG_H
