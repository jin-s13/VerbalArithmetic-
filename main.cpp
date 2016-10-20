#include "cryptarithmetic.h"
#include <QApplication>
#include "api/appinit.h"
#include <QTextCodec>
#include "equation.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AppInit *initApp = new AppInit;
    initApp->loadInfo();

    Cryptarithmetic w;
    w.setWindowFlags(w.windowFlags()& ~Qt::WindowMaximizeButtonHint); //固定大小
    w.setFixedSize(w.width(), w.height());
    w.show();

    return a.exec();
}
