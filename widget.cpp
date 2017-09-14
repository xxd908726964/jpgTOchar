#include "widget.h"
#include "ui_widget.h"


char chs[]={'M','N','Q','$','H','6','E','C','A','7','?','>',' ',' '};


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{

    setMinimumSize( 600,500);

    label_area=new QScrollArea(this);
    label=new QLabel(this);


    open=new QPushButton(this);
    open->setMaximumSize(110,30);
    open->setText("选择文件");

    layout=new QGridLayout(this);
    vbox=new QVBoxLayout(this);

    layout->addWidget(label_area,0,0);
    layout->addLayout(vbox,0,1);
    vbox->addWidget(open);

    label_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    label_area->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    label_area->setWidget(label);
    //--------------------------------------字符行距
    ziti_hangju_layout=new QHBoxLayout(this);
    ziti_hangju_label=new QLabel(this);
    ziti_hangju=new QSpinBox(this);
    ziti_hangju->setValue(10);
    ziti_hangju->setMaximum(99);
    ziti_hangju->setMinimum(1);
    ziti_hangju_label->setText("字符行距");
    vbox->addLayout(ziti_hangju_layout);
    ziti_hangju_layout->addWidget(ziti_hangju);
    ziti_hangju_layout->addWidget(ziti_hangju_label);
    ziti_hangju_size=10;

    //--------------------------------------字符宽度

    ziti_kuaidu_layout=new QHBoxLayout(this);
    ziti_kuaidu_label=new QLabel(this);
    ziti_kuaidu=new QSpinBox(this);
    ziti_kuaidu->setValue(10);
    ziti_kuaidu->setMaximum(99);
    ziti_kuaidu->setMinimum(1);
    ziti_kuaidu_label->setText("字符宽度");
    vbox->addLayout(ziti_kuaidu_layout);
    ziti_kuaidu_layout->addWidget(ziti_kuaidu);
    ziti_kuaidu_layout->addWidget(ziti_kuaidu_label);
    ziti_kuaidu_size=10;

    //------------------------------------------------------

    zhuanhuai=new QPushButton(this);
    zhuanhuai->setText("开始转换");
    zhuanhuai->setMaximumSize(110,30);
    vbox->addWidget(zhuanhuai);

    save=new QPushButton(this);
    save->setText("保存图片");
    save->setMaximumSize(110,30);
    vbox->addWidget(save);

    gaibian_xiansi_cicun=new QCheckBox(this);
    vbox->addWidget(gaibian_xiansi_cicun);
    gaibian_xiansi_cicun->setText("图片适应窗口");

    gaibian_xiansi_tupian=new QCheckBox(this);
    vbox->addWidget(gaibian_xiansi_tupian);
    gaibian_xiansi_tupian->setText("显示原始图片");

    image_new=NULL;
    image_old=NULL;

    jingdutiao=new QProgressBar(this);
    jingdutiao->setMaximumSize(110,30);
    jingdutiao->setValue(0);
    vbox->addWidget(jingdutiao);


    vbox->addStretch();
    gaibian_xiansi_tupian->setChecked(true);
    gaibian_xiansi_cicun->setChecked(true);

    connect(open,SIGNAL(clicked(bool)),this,SLOT(openfile()),Qt::AutoConnection);
    connect(zhuanhuai,SIGNAL(clicked(bool)),this,SLOT(zhuan_run_pthread()),Qt::AutoConnection);
    connect(save,SIGNAL(clicked(bool)),this,SLOT(savefile()),Qt::AutoConnection);
    connect(gaibian_xiansi_cicun,SIGNAL(clicked(bool)),
            this,SLOT(gaibian_xiansi(bool)),Qt::AutoConnection);
    connect(gaibian_xiansi_tupian,SIGNAL(clicked(bool)),
            this,SLOT(gaibian_xiansi_tupian_(bool)),
            Qt::AutoConnection);
    connect(this,SIGNAL(jingdutiao_signal(int)),jingdutiao,SLOT(setValue(int)),Qt::AutoConnection);



    ui->setupUi(this);

}



Widget::~Widget()
{
    delete ui;
}

void Widget::zhuan()
{

    zhuan_over=0;
    size_old=image_old->size();
    image_new=new QImage(size_old,QImage::Format_ARGB32);

    QPainter *painter=new QPainter(image_new);




    for(int y=ziti_hangju_size;y<=image_old->height();y+=ziti_hangju_size)
    {
        int x=0;

        for(;x<=image_old->width();x+=ziti_kuaidu_size)
        {
            QPointF pos;
            pos.setX(x);
            pos.setY(y);
            QColor color=image_old->pixelColor(x,y);
            QPen pen= painter->pen();
            pen.setColor(Qt::black);
            QFont font=painter->font();
            font.setPixelSize(ziti_kuaidu_size);
            painter->setPen(pen);
            painter->setFont(font);

            int ch=(color.red()+color.green()+color.blue())/42;
            painter->drawText(pos,QString(chs[ch]));

        }
        emit jingdutiao_signal(qRound(((float)(x*y)/(image_new->height()*image_new->width())*100)));
    }

    if(gaibian_xiansi_cicun->isChecked())
    {
        QSize scal(label_area->geometry().width(),label_area->geometry().height());
        QImage *temp=new QImage;
        *temp=image_new->scaled(scal,Qt::KeepAspectRatio);
        label->setGeometry(0,0,label_area->geometry().width(),label_area->geometry().height());
        label->setPixmap(QPixmap::fromImage(*temp));
        gaibian_xiansi_tupian->setChecked(false);

        return;
    }
    label->setGeometry(0,0,image_new->width(),image_old->height());
    label->setPixmap(QPixmap::fromImage(*image_new));
    gaibian_xiansi_tupian->setChecked(false);
    return;

}

void Widget::openfile()
{

    filename=QFileDialog::getOpenFileName(this,QString("选择文件"),
                                          QString(QCoreApplication::applicationDirPath()),
                                          QString("*.jpg *.gif"));
    if(filename.isEmpty())return;
    //gif--------gif--------gif--------gif--------gif--------gif--------gif--------gif--------gif
    if((filename.contains(".gif")));
    {

        gif_old=new QMovie(filename);
        gif_old->jumpToFrame(2);
qDebug()<<gif_old->scaledSize();
        if(gaibian_xiansi_cicun->isChecked())
        {

//            QSize scal;
//            if(qAbs(gif_old->frameRect().width()-label_area->geometry().width())
//                    >qAbs(gif_old->frameRect().height()-label_area->geometry().height()))
//            scal=QSize(gif_old->frameRect().width()*(label_area->geometry().height()/gif_old->frameRect().height()),
//                       gif_old->frameRect().height()*(label_area->geometry().height()/gif_old->frameRect().height()));

//            scal=QSize(gif_old->frameRect().width()*(label_area->geometry().width()/gif_old->frameRect().width()),
//                       gif_old->frameRect().height()*(label_area->geometry().width()/gif_old->frameRect().width()));

//        qDebug()<<scal;
//             gif_old->setScaledSize(scal);

            label->setGeometry(0,0,label_area->geometry().width(),label_area->geometry().height());
            label->setMovie(gif_old);
            gif_old->start();

            zhuan_over=1;
            gaibian_xiansi_tupian->setChecked(true);
            if(gif_new!=NULL)
            {
                delete image_new;
                gif_new=NULL;
            }
            return;
        }
        label->setGeometry(0,0,image_old->width(), image_old->height());
        label->setPixmap(QPixmap::fromImage(*image_old));
        zhuan_over=1;
        gaibian_xiansi_tupian->setChecked(true);
        if(image_new!=NULL)
        {
            delete image_new;
            image_new=NULL;
        }
        return;
    }
    //gif--------gif--------gif--------gif--------gif--------gif--------gif--------gif--------gif
    image_old=new QImage;
    image_old->load(filename);
    if(gaibian_xiansi_cicun->isChecked())
    {

        QSize scal(label_area->geometry().width(),label_area->geometry().height());
        QImage *temp=new QImage;
        *temp=image_old->scaled(scal,Qt::KeepAspectRatio);
        label->setGeometry(0,0,label_area->geometry().width(),label_area->geometry().height());
        label->setPixmap(QPixmap::fromImage(*temp));
        zhuan_over=1;
        gaibian_xiansi_tupian->setChecked(true);
        if(image_new!=NULL)
        {
            delete image_new;
            image_new=NULL;
        }
        return;
    }
    label->setGeometry(0,0,image_old->width(), image_old->height());
    label->setPixmap(QPixmap::fromImage(*image_old));
    zhuan_over=1;
    gaibian_xiansi_tupian->setChecked(true);
    if(image_new!=NULL)
    {
        delete image_new;
        image_new=NULL;
    }
    return;

}

void Widget::savefile()
{
    filename=QFileDialog::getSaveFileName(this,QString("保存文件"),filename,QString("*.jpg"));
    if(filename.isEmpty())return;
    image_new->save(filename);


}

void Widget::gaibian_xiansi(bool a)
{

    if(image_old==NULL||image_old->isNull())
    {
        QMessageBox::information(this,"信息","请先加载图像。        ",QMessageBox::Ok);
        gaibian_xiansi_cicun->setChecked(false);
        return;
    }
    if(a)
    {
        if(image_new==NULL&&!gaibian_xiansi_tupian->isChecked())
        {
            QMessageBox::information(this,"信息","请先\"转换图像\"或者选择\"显示原始图像\"。        ",QMessageBox::Ok);
            gaibian_xiansi_cicun->setChecked(false);
            return;
        }
        if(gaibian_xiansi_tupian->isChecked())
        {

            QSize scal(label_area->geometry().width(),label_area->geometry().height());
            QImage *temp=new QImage;
            *temp=image_old->scaled(scal,Qt::KeepAspectRatio);
            label->setGeometry(0,0,label_area->geometry().width(),label_area->geometry().height());
            label->setPixmap(QPixmap::fromImage(*temp));



            return;
        }
        QSize scal(label_area->geometry().width(),label_area->geometry().height());
        QImage *temp=new QImage;
        *temp=image_new->scaled(scal,Qt::KeepAspectRatio);
        label->setGeometry(0,0,label_area->geometry().width(),label_area->geometry().height());
        label->setPixmap(QPixmap::fromImage(*temp));


        return;

    }
    else
    {
        if(image_new==NULL&&!gaibian_xiansi_tupian->isChecked())
        {
            QMessageBox::information(this,"信息","请先\"转换图像\"或者选择\"显示原始图像\"。        ",QMessageBox::Ok);
            gaibian_xiansi_cicun->setChecked(false);
            return;
        }
        if(gaibian_xiansi_tupian->isChecked())
        {

            label->setGeometry(0,0,image_old->width(),image_old->height());
            label->setPixmap(QPixmap::fromImage(*image_old));

            return;
        }

        label->setGeometry(0,0,image_old->width(),image_old->height());
        label->setPixmap(QPixmap::fromImage(*image_new));

        return;

    }

}

void Widget::gaibian_xiansi_tupian_(bool a)
{

    if(a==1)//显示原始图
    {
        if(image_old==NULL)
        {
            QMessageBox::information(this,"信息","请先加载图像。              ",QMessageBox::Ok);
            gaibian_xiansi_tupian->setChecked(false);
            return;
        }
        if(gaibian_xiansi_cicun->isChecked())
        {
            QSize scal(label_area->geometry().width(),label_area->geometry().height());
            QImage *temp=new QImage;
            *temp=image_old->scaled(scal,Qt::KeepAspectRatio);
            label->setGeometry(0,0,label_area->geometry().width(),label_area->geometry().height());
            label->setPixmap(QPixmap::fromImage(*temp));
            return;
        }
        label->setGeometry(0,0,image_old->width(),image_old->height());
        label->setPixmap(QPixmap::fromImage(*image_old));
    }
    if(a==0)//显示转换图
    {
        if(image_new==NULL)
        {
            QMessageBox::information(this,"信息","请先\"转换图像\"或者选择\"显示原始图像\"。        ",QMessageBox::Ok);
            gaibian_xiansi_tupian->setChecked(true);
            return;
        }
        if(gaibian_xiansi_cicun->isChecked())
        {
            QSize scal(label_area->geometry().width(),label_area->geometry().height());
            QImage *temp=new QImage;
            *temp=image_new->scaled(scal,Qt::KeepAspectRatio);
            label->setGeometry(0,0,label_area->geometry().width(),label_area->geometry().height());
            label->setPixmap(QPixmap::fromImage(*temp));
            return;
        }
        label->setGeometry(0,0,image_new->width(),image_new->height());
        label->setPixmap(QPixmap::fromImage(*image_new));
    }

}

void Widget::zhuan_run_pthread()
{
    if(image_old==NULL
            &&ziti_hangju_size==ziti_hangju->text().toInt()
            &&ziti_kuaidu_size==ziti_kuaidu->text().toInt() )
    {
        QMessageBox::information(this,"信息","请先加载图像。        ",QMessageBox::Ok);
        return;
    }
    if(image_new!=NULL&&zhuan_over!=1
            &&ziti_hangju_size==ziti_hangju->text().toInt()
            &&ziti_kuaidu_size==ziti_kuaidu->text().toInt())
    {
        QMessageBox::information(this,"信息","图像已经转换完成。        ",QMessageBox::Ok);
        return;
    }

    ziti_hangju_size=ziti_hangju->text().toInt();
    ziti_kuaidu_size=ziti_kuaidu->text().toInt();


    jingdutiao->setValue(0);
    QtConcurrent::run(this,&zhuan);

}

