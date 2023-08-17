
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QApplication>
#include <QCamera>
#include <QCloseEvent>
#include <QComboBox>
#include <QDateTime>
#include <QGraphicsTextItem>
#include <QGraphicsVideoItem>
#include <QGraphicsView>
#include <QImageCapture>
#include <QMediaCaptureSession>
#include <QMediaDevices>
#include <QMediaRecorder>
#include <QMenu>
#include <QMessageBox>
#include <QMutex>
#include <QMutexLocker>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QUrl>
#include <QVideoFrame>
#include <QVideoSink>
#include <QVideoWidget>
#include <QWaitCondition>
#include <QException>
#include <QSerialPort>
#include <QSerialPortInfo>

#include "woThr.h"

const int N = 3;
extern QQueue<QImage> image_que;
extern QImage m_image;


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void test();
    void init();
    void imageCap();
    void record();
    void capturetoFile();
    void subwidget();
    void cameraImageCaptured(int id, QImage image);
    void imageSaved(int id, const QString &fileName);
    QImage GraizeImage();
    void setLabelPixmap();
    void closeEvent(QCloseEvent* ev) override;
    void keyPressEvent(QKeyEvent* ev) override;
    Ui::MainWindow *ui;


private slots:
    void on_phoBtn_clicked();

    void on_recordBtn_clicked();

    void on_recordBtn_2_clicked();

    void on_frame_changed(const QVideoFrame &frame); // 一帧视频到来的信号

    void on_action_triggered();

private:


    QList<QCameraDevice> m_cameraDevices;
    QScopedPointer<QCamera> m_camera; //动态分配 qcamera对象
    QScopedPointer<QCamera> m_Phocamera;
    QMediaCaptureSession m_cap;                     //媒体对话
    QImageCapture *m_imageCapture;                  //记录媒体内容
    QScopedPointer<QMediaRecorder> m_mediaRecorder; //QMediaRecorder  高级媒体录制类q1
    QVideoSink *m_videoSink;


    QSystemTrayIcon *icon; //托盘菜单

    QMutex *m_mutex; //定义一个线程锁变量

    QThread *m_thr;
    QWaitCondition m_condi;

    WoThr *q;
    QThread *thr;


    int m_width = 0;
    int m_height = 0;
signals:
    void starting();
    void setPic(QImage ima);
    void end();

};

#endif // MAINWINDOW_H
