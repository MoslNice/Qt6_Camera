
#include "subWidget.h"
#include "mainwindow.h"

SubWidget::SubWidget(QWidget *parent)
    : QWidget{parent},pic_label(new QLabel(this))
{
    pic_label->resize(1060,595);
    pic_label->setText("hello world");
}

SubWidget::~SubWidget()
{
    qDebug() << QThread::currentThread();
}

void SubWidget::putImage(QImage ima)
{
    pic_label->setPixmap(QPixmap::fromImage(
        ima.scaled(pic_label->width(), pic_label->height(), Qt::KeepAspectRatio)));
}

