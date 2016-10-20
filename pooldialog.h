#ifndef POOLDIALOG_H
#define POOLDIALOG_H


#include <QImage>
#include <iostream>
#include <QMouseEvent>
#include <QMessageBox>
#include <QString>
#include <QDialog>
#include <QPainter>

namespace Ui {
class PoolDialog;
}

class PoolDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PoolDialog(QWidget *parent = 0);
    ~PoolDialog();
protected:
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent* e);
private:
    Ui::PoolDialog *ui;
};

#endif // POOLDIALOG_H
