#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QPixmap>
#include<QImage>
#include<QLabel>
#include<QGridLayout>
#include<QPushButton>
#include<QFileDialog>
#include<QVBoxLayout>
#include<QPainter>
#include<QPen>
#include<QFont>
#include<QDebug>
#include<QByteArray>
#include<QBuffer>
#include<QScrollArea>
#include<QCheckBox>
#include<QMessageBox>
#include<QtConcurrent/QtConcurrent>
#include<QProgressBar>
#include<QDesktopWidget>
#include<QSpinBox>
#include<QMovie>


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);

    ~Widget();
public slots:
    void zhuan();
    void openfile();
    void savefile();
    void gaibian_xiansi(bool a);
    void gaibian_xiansi_tupian_(bool a);
    void zhuan_run_pthread();

signals:
    void jingdutiao_signal(int x);


private:
    int zhuan_over;//0表示当已经完成转换 1表示未完成转换
    Ui::Widget *ui;
    QLabel *label;
    QImage *image_old;
    QGridLayout *layout;
    QPushButton *open;
    QString filename;
    QPushButton *zhuanhuai;
    QVBoxLayout *vbox;
    QPushButton *save;
    QScrollArea *label_area;
    QImage *image_new;
    QCheckBox *gaibian_xiansi_cicun;
    QCheckBox *gaibian_xiansi_tupian;
    QRect old_rect;
    QSize size_old;
    QProgressBar* jingdutiao;
    QDesktopWidget *top_window;

    QSpinBox *ziti_kuaidu;
    QSpinBox *ziti_hangju;
    QHBoxLayout *ziti_hangju_layout;
    QHBoxLayout *ziti_kuaidu_layout;
    QLabel *ziti_hangju_label;
    QLabel *ziti_kuaidu_label;
    int ziti_hangju_size;
    int ziti_kuaidu_size;
    int image_type;
    QMovie *gif_old;
    QMovie *gif_new;
};

#endif // WIDGET_H
