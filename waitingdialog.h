#ifndef WAITINGDIALOG_H
#define WAITINGDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QTime>

namespace Ui {
class WaitingDialog;
}

class WaitingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WaitingDialog(QWidget *parent = 0);
    ~WaitingDialog();

public slots:
    void timeUpDate();

private:
    void paintEvent(QPaintEvent *);
    Ui::WaitingDialog *ui;
    int waitTime = 0;
    QTimer* timer;
};

#endif // WAITINGDIALOG_H
