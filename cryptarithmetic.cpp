#include "cryptarithmetic.h"
#include "ui_cryptarithmetic.h"


#define word_width 22
#define word_height 30
#define line_spacing 1
#define cubic 4
#define exp_yspacing 0
#define exp_xspacing 25

Cryptarithmetic::Cryptarithmetic(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Cryptarithmetic)
{
    ui->setupUi(this);
    this->initStyle();
    myHelper::moveFormToCenter(this);
}

void Cryptarithmetic::paintEvent(QPaintEvent *)
{
    QImage image(":/image/board.png");
    QPainter painter(this);
    painter.drawImage(0,0,image);
}

Cryptarithmetic::~Cryptarithmetic()
{
    delete ui;
}
bool Cryptarithmetic::genSubQuestion(int num, int len, int types)
{
    bool flag = genAddQuestion(num,len,types);
    QString tempstr;
    tempstr = this->equ_lines.front();
    this->equ_lines.front() = this->equ_lines.back();
    this->equ_lines.back() = tempstr;
    return flag;
}
bool Cryptarithmetic::genAddQuestion(int num, int len, int types)
{
    int iter = 0;
    int res_num = 0;
    while(true)
    {
        //num包括被加数
        QVector<int> addlist;
        int res = 0;
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
        for(int i=0; i<num; i++)
        {
            addlist.append(1 + qrand()%(int)qPow(10,len));
            res += addlist[i];
        }
        if(res < qPow(10,len))
            continue;
        addlist.append(res);
        QVector<QString> lines = IntToEquation(addlist);
        eq.clear();
        eq.Initialize(lines,Equation::add);
        iter++;
        if(eq.getDigitNum() < types)
            continue;

        eq.search(eq.getSearchNum(),0);
        res_num = eq.result.size();
        if(res_num == 1)
        {
            this->equ_lines = lines;
            break;
        }
        if(iter > 100000)
            break;
    }
    return res_num == 1;
}
bool Cryptarithmetic::genMulQuestion(int len_up, int len_down, int types)
{
    int iter = 0;
    int res_num = 0;
    while(true)
    {
        QVector<int> mullist;
        int res = 1;
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

        mullist.append(1 + qrand()%(int)qPow(10,len_up));
        res *= mullist[0];
        mullist.append(1 + qrand()%(int)qPow(10,len_down));
        res *= mullist[1];
        int mul_b = mullist[1];
        while(mul_b > 0)
        {
            mullist.append((mul_b%10) * mullist[0]);
            mul_b/=10;
        }
        if(mullist.size() != 3)
            mullist.append(res);
        QVector<QString> lines = IntToEquation(mullist);
        eq.clear();
        eq.Initialize(lines,Equation::mul);
        iter++;
        if(eq.getDigitNum() < types)
            continue;

        eq.search(eq.getSearchNum(),0);
        res_num = eq.result.size();
        if(res_num == 1)
        {
            this->equ_lines = lines;
            break;
        }
        if(iter > 100000)
            break;
    }
    return res_num == 1;
}
bool Cryptarithmetic::genDivQuestion(bool residual, int len_up, int len_down, int types)
{
    //len_up 除数  len_down商
    int iter = 0;
    int res_num = 0;
    while(true)
    {
        QVector<int> mullist;
        int res = 1;
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

        mullist.append(1 + qrand()%(int)qPow(10,len_up));   //除数
        res *= mullist[0];
        mullist.append(1 + qrand()%(int)qPow(10,len_down)); //商
        res *= mullist[1];
        int resid = qrand()%mullist[0];
        if(!residual)   //不含余数
        {
            resid = 0;
        }
        int sum = resid;
        mullist.append(resid);  //余数
        int mul_b = mullist[1]; //商
        while(mul_b > 0)
        {
            int product = (mul_b%10) * mullist[0];
            mullist.append(product);
            mullist.append(sum + product);
            mul_b/=10;
            sum/=10;
        }
        mullist.pop_back(); //最后一行删去
        mullist.append(res);    //被除数
        QVector<QString> lines = IntToEquation(mullist);
        eq.clear();
        eq.Initialize(lines,Equation::div);
        iter++;
        if(eq.getDigitNum() < types)
            continue;

        eq.search(eq.getSearchNum(),0);
        res_num = eq.result.size();
        if(res_num == 1)
        {
            this->equ_lines = lines;
            break;
        }
        if(iter > 100000)
            break;
    }
    return res_num == 1;
}
QVector<QString> Cryptarithmetic::IntToEquation(QVector<int> vec)
{
    int num = 0;
    QString dict = QString("ABCDEFGHIJ");
    QVector<QString> lines;
    lines.resize(vec.size());
    QVector<int> stat;
    for(int i = 0; i<vec.size(); i++)
    {
        QString str = QString::number(vec[i]);
        lines[i] = str;
        for(int j = 0; j<str.size(); j++)
        {
            int curr = str[j].digitValue();
            if(stat.contains(curr))
            {
                for(int k = 0; k<stat.size(); k++)
                {
                    if(stat[k] == curr)
                    {
                        lines[i][j] = dict[k];
                        break;
                    }

                }
            }
            else
            {
                stat.append(curr);
                lines[i][j] = dict[num];
                num++;
            }
        }
    }
    return lines;

}

void Cryptarithmetic::initStyle()
{

    myHelper::setStyle(QString(":/qss/black.css"));
}

void Cryptarithmetic::ShowEquation(bool readFlag)    //展示算式
{
    scene.clear();
    QVector<QVector<QGraphicsTextItem*>> vec_vecs;  //整理为矩阵
    vec_vecs.resize(equ_lines.size());
    for(int i = 0; i<this->equ_lines.size(); i++)
    {
        vec_vecs[i].resize(equ_lines[i].size());
        for(int j = 0; j<equ_lines[i].size(); j++)
        {
            QString str = QString(equ_lines[i][j]);
            vec_vecs[i][j] = new QGraphicsTextItem(str);
            vec_vecs[i][j]->setFont(QFont("Times",18,QFont::Bold));
            //lists[i][i]->setTextInteractionFlags(Qt::TextEditorInteraction);
        }
    }

    switch(this->exp)
    {
    case Equation::add:
    {
        int max_length = equ_lines.back().size();
        int num_lines = equ_lines.size();
        float base_x = word_width * max_length/2;
        float base_y = -word_height * num_lines/2;

        //scene.setSceneRect(-150,-150,300,300);
        for(int i = 0; i<this->equ_lines.size(); i++)
        {
            for(int j = 0; j<equ_lines[i].size(); j++)
            {
                vec_vecs[i][j]->setPos(base_x-(vec_vecs[i].size()-j)*word_width,base_y+word_height*i);
                scene.addItem(vec_vecs[i][j]);
            }
        }

        QPainterPath path;
        path.moveTo(base_x - vec_vecs.back().size() * word_width - word_width,base_y + (vec_vecs.size() - 1)*word_height - line_spacing);
        path.lineTo(base_x +  word_width,base_y + (vec_vecs.size() - 1)*word_height - line_spacing);

        QImage image;
        image.load( ":/image/add.png" );
        QPixmap pixmapToShow = QPixmap::fromImage(image.scaled(QSize(20,20)));
        QGraphicsPixmapItem* item = scene.addPixmap(pixmapToShow);
        item->setPos(base_x - vec_vecs.back().size() * word_width - exp_xspacing,base_y + (vec_vecs.size() - 2)*word_height - exp_yspacing);
        QPen pen = QPen(Qt::black);
        pen.setWidth(3);
        scene.addPath(path,pen,QBrush(Qt::green));
    }
        break;
    case Equation::sub:
    {
        int max_length = equ_lines.front().size();
        int num_lines = equ_lines.size();
        float base_x = word_width * max_length/2;
        float base_y = -word_height * num_lines/2;

        //scene.setSceneRect(-150,-150,300,300);
        for(int i = 0; i<this->equ_lines.size(); i++)
        {
            for(int j = 0; j<equ_lines[i].size(); j++)
            {
                vec_vecs[i][j]->setPos(base_x-(vec_vecs[i].size()-j)*word_width,base_y+word_height*i);
                scene.addItem(vec_vecs[i][j]);
            }
        }

        QPainterPath path;

        path.moveTo(base_x - vec_vecs.front().size() * word_width - exp_xspacing,base_y + (vec_vecs.size() - 1)*word_height - line_spacing);
        path.lineTo(base_x +  word_width,base_y + (vec_vecs.size() - 1)*word_height - line_spacing);

        QImage image;
        image.load( ":/image/sub.png" );
        QPixmap pixmapToShow = QPixmap::fromImage(image.scaled(QSize(20,20)));
        QGraphicsPixmapItem* item = scene.addPixmap(pixmapToShow);
        item->setPos(base_x - vec_vecs.front().size() * word_width - exp_xspacing,base_y + (vec_vecs.size() - 2)*word_height - exp_yspacing);

        QPen pen = QPen(Qt::black);
        pen.setWidth(3);
        scene.addPath(path,pen,QBrush(Qt::green));

    }
        break;
    case Equation::mul:
    {
        int max_length = equ_lines.back().size();
        int num_lines = equ_lines.size();
        float base_x = word_width * max_length/2;
        float base_y = -word_height * num_lines/2;

        //scene.setSceneRect(-150,-150,300,300);
        for(int i = 0; i<2; i++)    //乘法部分
        {
            for(int j = 0; j<equ_lines[i].size(); j++)
            {
                vec_vecs[i][j]->setPos(base_x-(vec_vecs[i].size()-j)*word_width,base_y+word_height*i);
                scene.addItem(vec_vecs[i][j]);
            }
        }

        QPainterPath path;
        path.moveTo(base_x - vec_vecs.back().size() * word_width - exp_xspacing,base_y + 2 * word_height - line_spacing);
        path.lineTo(base_x +  word_width,base_y + 2*word_height - line_spacing);

        QImage image;
        image.load( ":/image/mul.png" );
        QPixmap pixmapToShow = QPixmap::fromImage(image.scaled(QSize(20,20)));
        QGraphicsPixmapItem* item = scene.addPixmap(pixmapToShow);
        item->setPos(base_x - vec_vecs.back().size() * word_width - exp_xspacing,base_y + word_height - exp_yspacing);
        for(int i = 2; i<this->equ_lines.size() - 1; i++)   //中间结果部分
        {
            for(int j = 0; j<equ_lines[i].size(); j++)
            {
                vec_vecs[i][j]->setPos(base_x-(vec_vecs[i].size()-j + i - 2)*word_width,base_y+word_height*i);
                scene.addItem(vec_vecs[i][j]);
            }
        }

        for(int j = 0; j<equ_lines[equ_lines.size() - 1].size(); j++)   //最后结果
        {
            vec_vecs[equ_lines.size() - 1][j]->setPos(base_x-(vec_vecs[equ_lines.size() - 1].size()-j)*word_width,base_y+word_height*(equ_lines.size() - 1));
            scene.addItem(vec_vecs[equ_lines.size() - 1][j]);
        }
        path.moveTo(base_x - vec_vecs.back().size() * word_width - exp_xspacing,base_y + (vec_vecs.size() - 1) * word_height - line_spacing);
        path.lineTo(base_x +  word_width,base_y + (vec_vecs.size() - 1)*word_height - line_spacing);

        QPen pen = QPen(Qt::black);
        pen.setWidth(3);
        scene.addPath(path,pen,QBrush(Qt::green));
    }
        break;
    case Equation::div:
    {
        int max_length = equ_lines.back().size() + equ_lines.front().size();
        int num_lines = equ_lines.size() - 1;
        float base_x = word_width * max_length/2;
        float base_y = -word_height * num_lines/2;

        //scene.setSceneRect(-150,-150,300,300);
        for(int j = 0; j<equ_lines[1].size(); j++)  //商
        {
            vec_vecs[1][j]->setPos(base_x-(vec_vecs[1].size()-j)*word_width,base_y);
            scene.addItem(vec_vecs[1][j]);
        }

        QPainterPath path;  //绘制除法竖式
        path.moveTo(base_x - vec_vecs.back().size() * word_width,base_y + word_height - line_spacing);
        path.lineTo(base_x,base_y + word_height - line_spacing);
        path.moveTo(base_x - vec_vecs.back().size() * word_width,base_y + word_height - line_spacing);
        path.cubicTo(base_x - vec_vecs.back().size() * word_width,base_y + word_height + 2 * cubic\
                     ,base_x - vec_vecs.back().size() * word_width,base_y + word_height + 3 * cubic\
                     ,base_x - vec_vecs.back().size() * word_width- cubic,base_y + 2 * word_height-cubic);

        for(int j = 0; j<equ_lines[equ_lines.size() - 1].size(); j++)   //被除数
        {
            vec_vecs[vec_vecs.size() - 1][j]->setPos(base_x-(vec_vecs[vec_vecs.size() - 1].size()-j)*word_width,base_y + word_height);
            scene.addItem(vec_vecs[vec_vecs.size() - 1][j]);
        }
        for(int j = 0; j<equ_lines[0].size(); j++)   //除数
        {
            vec_vecs[0][j]->setPos(base_x - (vec_vecs.back().size()+0.5) * word_width -(vec_vecs[0].size()-j)*word_width,base_y + word_height);
            scene.addItem(vec_vecs[0][j]);
        }

        int base_rx = base_x-vec_vecs[1].size()*word_width;
        int base_ry = base_y + word_height*2;
        int tempn = 0;
        for(int i = equ_lines.size() - 2; i>1;i-=2)   //中间结果部分
        {
            for(int j = 0; j<equ_lines[i].size(); j++)
            {
                vec_vecs[i][j]->setPos(base_rx +(-vec_vecs[i].size() + 1 + j + tempn)*word_width,base_ry+word_height*2*tempn);
                scene.addItem(vec_vecs[i][j]);
            }
            tempn++;
            path.moveTo(base_x - vec_vecs.back().size() * word_width,base_y + (2*tempn + 1)*word_height - line_spacing);
            path.lineTo(base_x,base_y + (2*tempn+1)*word_height - line_spacing);
            for(int j = 0; j<equ_lines[i-1].size(); j++)
            {
                if(i==3)    //余数
                    vec_vecs[i-1][j]->setPos(base_rx +(-vec_vecs[i-1].size() + j + tempn)*word_width,base_ry+word_height*(2*tempn - 1));
                else
                    vec_vecs[i-1][j]->setPos(base_rx +(-vec_vecs[i-1].size() + 1 + j + tempn)*word_width,base_ry+word_height*(2*tempn - 1));
                scene.addItem(vec_vecs[i-1][j]);
            }
        }

        QPen pen = QPen(Qt::black);
        pen.setWidth(3);
        scene.addPath(path,pen,QBrush(Qt::green));
    }
        break;
    }

    ui->graphicsView->setScene(&scene);
    ui->graphicsView->show();
}

void Cryptarithmetic::on_btn_GenQuestion_clicked()
{

    QString str = QString("该操作会擦掉白板内容。\n您确定继续吗？");
    if(myHelper::showMessageBoxQuesion(str) == QDialog::Accepted)
    {
        state = input;
        scene.clear();

        dlg_Gen = new GenDialog(this);
        dlg_Gen->setWindowFlags(dlg_Gen->windowFlags()& ~Qt::WindowMaximizeButtonHint); //固定大小
        dlg_Gen->setFixedSize(dlg_Gen->width(), dlg_Gen->height());
        dlg_Gen->setAttribute(Qt::WA_DeleteOnClose, true);
        dlg_Gen->setModal(true);
        if(dlg_Gen->exec() == QDialog::Accepted)
        {
//            WaitingDialog* dlg_Wait =  new WaitingDialog(this);
//            dlg_Wait->setWindowFlags(dlg_Wait->windowFlags()& ~Qt::WindowMaximizeButtonHint); //固定大小
//            dlg_Wait->setFixedSize(dlg_Wait->width(), dlg_Wait->height());
//            dlg_Wait->setAttribute(Qt::WA_DeleteOnClose, true);
//            dlg_Wait->show();
            //QTimer::singleShot(200,dlg_Wait,SLOT(qApp-processEvents()));
            state = end;
            bool flag = false;
            switch (exp) {
            case Equation::add:
            {
                switch (this->difficulty) {
                case EASY:
                    flag = genAddQuestion(2,5,6);
                    break;
                case MEDIUM:
                    flag = genAddQuestion(3,7,8);
                    break;
                case HARD:
                    flag = genAddQuestion(4,7,10);
                    break;
                default:
                    break;
                }

                if(flag == true)
                {
                    //dlg_Wait->close();
                    myHelper::showMessageBoxInfo("找到了匹配的题目。");
                    flag = false;
                    ShowEquation(true);    //展示算式
                    state = begin;

                }
                else
                {
                    //dlg_Wait->close();
                    myHelper::showMessageBoxError("对不起，没有找到合适的题目。");
                }
            }
                break;
            case Equation::sub:
            {
                switch (this->difficulty) {
                case EASY:
                    flag = genSubQuestion(2,5,1);
                    break;
                case MEDIUM:
                    flag = genSubQuestion(3,7,6);
                    break;
                case HARD:
                    flag = genSubQuestion(4,7,10);
                    break;
                default:
                    break;
                }
                if(flag == true)
                {
                    //dlg_Wait->close();
                    myHelper::showMessageBoxInfo("找到了匹配的题目。");
                    flag = false;
                    ShowEquation(true);    //展示算式
                    state = begin;
                }
                else
                {
                    //dlg_Wait->close();
                    myHelper::showMessageBoxError("对不起，没有找到合适的题目。");
                }
            }
                break;
            case Equation::mul:
            {
                switch (this->difficulty) {
                case EASY:
                    flag = genMulQuestion(4,2,1);
                    break;
                case MEDIUM:
                    flag = genMulQuestion(4,3,5);
                    break;
                case HARD:
                    flag = genMulQuestion(5,4,10);
                    break;
                default:
                    break;
                }

                if(flag == true)
                {
                    //dlg_Wait->close();
                    myHelper::showMessageBoxInfo("找到了匹配的题目。");
                    flag = false;
                    ShowEquation(true);    //展示算式
                    state = begin;
                }
                else
                {
                    //dlg_Wait->close();
                    myHelper::showMessageBoxError("对不起，没有找到合适的题目。");
                }
            }
                break;
            case Equation::div:
            {
                switch (this->difficulty) {
                case EASY:
                    flag = genDivQuestion(false,4,2,1);
                    break;
                case MEDIUM:
                    flag = genDivQuestion(false,4,3,8);
                    break;
                case HARD:
                    flag = genDivQuestion(true,5,4,10);
                    break;
                default:
                    break;
                }

                if(flag == true)
                {
                    //dlg_Wait->close();
                    myHelper::showMessageBoxInfo("找到了匹配的题目。");
                    flag = false;
                    ShowEquation(true);    //展示算式
                    state = begin;
                }
                else
                {
                    //dlg_Wait->close();
                    myHelper::showMessageBoxError("对不起，没有找到合适的题目。");
                }
            }
                break;
            default:
                break;
            }
        }
    }

}

void Cryptarithmetic::on_btn_Custom_clicked()
{

    QString str = QString("该操作会擦掉白板内容。\n您确定继续吗？");
    if(myHelper::showMessageBoxQuesion(str) == QDialog::Accepted)
    {
        state = input;
        scene.clear();

        dlg_Edit = new EditDialog(this);
        dlg_Edit->setWindowFlags(dlg_Edit->windowFlags()& ~Qt::WindowMaximizeButtonHint); //固定大小
        dlg_Edit->setFixedSize(dlg_Edit->width(), dlg_Edit->height());
        dlg_Edit->setAttribute(Qt::WA_DeleteOnClose, true);
        dlg_Edit->setModal(true);
        dlg_Edit->show();
    }

}

void Cryptarithmetic::on_btn_QuestionPool_clicked()
{
    dlg_Pool = new PoolDialog(this);
    dlg_Pool->setWindowFlags(dlg_Pool->windowFlags()& ~Qt::WindowMaximizeButtonHint); //固定大小
    dlg_Pool->setFixedSize(dlg_Pool->width(), dlg_Pool->height());
    dlg_Pool->setAttribute(Qt::WA_DeleteOnClose, true);
    if(dlg_Pool->exec() == QDialog::Accepted)
    {
        QString filepath = QString(":/questions/") + QString::number(this->qstNum + 1) + QString(".txt");
        QFile file(filepath);
         if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
             return;
         QTextStream in(&file);
         QString line = in.readLine();
         switch (line.toInt()) {
         case 1:
             this->exp = Equation::add;
             break;
         case 2:
             this->exp = Equation::sub;
             break;
         case 3:
             this->exp = Equation::mul;
             break;
         case 4:
             this->exp = Equation::div;
             break;
         default:
             break;
         }
         equ_lines.clear();
         line = in.readLine();
         while (!line.isNull()) {
             equ_lines.append(line);
             line = in.readLine();
         }
         ShowEquation(true);    //展示算式
         state = begin;
    }
}

void Cryptarithmetic::on_btn_ShowResult_clicked()
{
    if(state == begin)  //游戏已经开始
    {
        state = end;
        eq.clear();
        eq.Initialize(this->equ_lines,this->exp);
        eq.search(eq.getSearchNum(),0);
        int res_num = eq.result.size();
        if(res_num == 0)
        {
            QString str("很抱歉，没有找到答案。");
            myHelper::showMessageBoxInfo(str);
        }
        else
        {
            QString str = QString("哈哈，我找到了")+ QString::number(res_num) + QString("个答案。\n");
            str+=QString("你想要看所有的答案吗？");
            if(myHelper::showMessageBoxQuesion(str) == QDialog::Accepted)
            {
                ResultDialog res_dlg;
                res_dlg.showResult(eq.result);
                res_dlg.exec();
            }
        }
    }
    else if(state == end)
    {
        int res_num = eq.result.size();
        if(res_num == 0)
        {
            QString str("很抱歉，没有找到答案。");
            myHelper::showMessageBoxInfo(str);
        }
        else
        {
            QString str = QString("哈哈，我找到了")+ QString::number(res_num) + QString("个答案。\n");
            str+=QString("你想要看所有的答案吗？");
            if(myHelper::showMessageBoxQuesion(str) == QDialog::Accepted)
            {
                ResultDialog res_dlg;
                res_dlg.showResult(eq.result);
                res_dlg.exec();
            }

        }
    }
    else if(state == ready)
    {

    }

}
