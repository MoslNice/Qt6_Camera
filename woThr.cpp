
#include "woThr.h"
#include "mainwindow.h"
#include <QApplication>


WoThr::WoThr(QObject *parent)
    : QObject{parent}
{
    label = new QLabel;
    m_mutex = new QMutex;
}

void WoThr::doWork()
{
    qDebug() << "sub " << QThread::currentThread();;
    static int i = 1;
    while (endflag) {
        while (flag) {
            int a = image_que.length();
            if (a) {
                m_mutex->lock();
                m_image = image_que.first();
                image_que.removeFirst();
                 m_mutex->unlock();
                //qDebug() << __FUNCTION__ << QThread::currentThread() << image_que.size()<<i++;
                m_image = m_image.convertToFormat(QImage::Format_Grayscale8, Qt::AutoColor);
                label->setPixmap(QPixmap::fromImage(
                    m_image.scaled(label->width(), label->height(), Qt::KeepAspectRatio)));
                QThread::msleep(10);
            }
        }
    }
}

void WoThr::setPic(QImage ima)
{
    qDebug() << __FUNCTION__ << QThread::currentThread();
    ima = ima.convertToFormat(QImage::Format_Grayscale8, Qt::AutoColor);
    label->setPixmap(QPixmap::fromImage(
        ima.scaled(label->width(), label->height(), Qt::KeepAspectRatio)));
}
