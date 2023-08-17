#include "mainwindow.h"
#include "./ui_mainwindow.h"

QQueue<QImage> image_que;
QImage m_image;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_mutex(new QMutex)
{
    ui->setupUi(this);

    qDebug() << "main" << QThread::currentThread();
    //    int ret = QMessageBox::information(this,"提示","文本文件",QMessageBox::Save|QMessageBox::Discard,QMessageBox::Save);
    //    switch (ret) {
    //    case QMessageBox::Save:
    //        qDebug()<<"yes";
    //    break;
    //    case QMessageBox::Discard:
    //        this->close();
    //    break;
    //    }
    //
    //test();
    init();
//    m_thr = QThread::create(&MainWindow::setLabelPixmap, this);
//    m_thr->start();

//    QTimer *timer = new QTimer;
//    connect(timer, &QTimer::timeout, this, [=]() {
//        QString timeStr = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
//        QString str = timeStr + " " + QString("高度 %1，宽度 %2").arg(m_height).arg(m_width);
//        ui->widheiLabel->setText(str);
//    });
//    timer->start(1000);

    thr = new QThread(this);
    q = new WoThr;
    q->label = ui->label;
    q->m_mutex = m_mutex;
    q->moveToThread(thr);
    connect(this, &MainWindow::starting, q, &WoThr::doWork);
    emit starting();
    //connect(this, &MainWindow::setPic, q, &WoThr::setPic);
    thr->start();
}

void MainWindow::subwidget()
{

}

void MainWindow::closeEvent(QCloseEvent* ev)
{
    Q_UNUSED(ev);
}

void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    if (ev->key() == Qt::Key_Return) {
        if (q->flag == true) {
            q->flag = false;
        } else
            q->flag = true;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    m_camera->stop();   //关闭程序之间停止相机 不然程序会崩溃
    if (thr->isRunning()) {
        q->endflag = false;
        q->flag = false;
        thr->quit();
        thr->wait();
        delete thr;
        delete q;
    }
}

void MainWindow::test()
{
    icon = new QSystemTrayIcon(QIcon(":/SuDio.ico"), this);
    QMenu *menu = new QMenu(this);
    menu->addAction("打开", this, &QMainWindow::showNormal);
    menu->addAction("最小化到托盘", this, &QMainWindow::showMinimized);
    menu->addSeparator();
    menu->addAction("退出程序", qApp, &QApplication::quit);

    icon->setContextMenu(menu);
    icon->show();

    connect(icon, &QSystemTrayIcon::activated, this, [=](QSystemTrayIcon::ActivationReason rea) {
        if (rea == QSystemTrayIcon::ActivationReason::DoubleClick) {
            if (this->windowState() & Qt::WindowState::WindowMinimized) {
                this->showNormal();
            } else {
                this->showMinimized();
            }
        }
    });
}

void MainWindow::imageCap()
{
    qDebug() << "imageCap" << QThread::currentThreadId();
    qDebug() << QThread::currentThreadId();
    connect(m_imageCapture,
            &QImageCapture::imageCaptured,
            this,
            &MainWindow::cameraImageCaptured,
            Qt::UniqueConnection);
    connect(m_imageCapture,
            &QImageCapture::imageSaved,
            this,
            &MainWindow::imageSaved,
            Qt::UniqueConnection);

    m_cap.setImageCapture(m_imageCapture);
    capturetoFile();
}

void MainWindow::record()
{
    qDebug() << "record" << QThread::currentThreadId();
    m_cap.setVideoOutput(ui->widget);
    if (ui->recordBtn->text() == "录像") {
        m_mediaRecorder->setOutputLocation(QUrl::fromLocalFile("D:/QT/Project/pic"));
        m_mediaRecorder->record();
        ui->recordBtn->setText("停止");

    } else {
        ui->recordBtn->setText("录像");
        m_mediaRecorder->stop();
        m_cap.setVideoOutput(ui->capWidget);
    }
}

void MainWindow::capturetoFile()
{
    m_imageCapture->captureToFile("D:/QT/Project/pic");
}



void MainWindow::on_phoBtn_clicked()
{
    QThread *thr = QThread::create(&MainWindow::imageCap, this);
    thr->start();
    //    imageCap();
}

void MainWindow::on_recordBtn_clicked()
{
    //    QThread* thr = QThread::create(&MainWindow::record,this);
    //    thr->start();
    record();
}

void MainWindow::on_recordBtn_2_clicked() {}

void MainWindow::cameraImageCaptured(int requestId, QImage img)
{
    Q_UNUSED(requestId);
    qDebug() << "cameraImageCaptured" << QThread::currentThreadId();

    ui->widheiLabel_2->setPixmap(QPixmap::fromImage(
        img.scaled(ui->widheiLabel_2->width(), ui->widheiLabel_2->height(), Qt::KeepAspectRatio)));
}

void MainWindow::imageSaved(int id, const QString &fileName)
{
    Q_UNUSED(id);
    qDebug() << fileName;
}

QImage MainWindow::GraizeImage()
{
    //    int w =m_image.width();
    //    int h = m_image.height();
    //    QImage iGray(w,h, QImage::Format_ARGB32);

    //    for(int i=0; i<w;i++)
    //    {
    //        for(int j=0; j<h;j++)
    //        {
    //            QRgb pixel = m_image.pixel(i,j);
    //            int gray = qGray(pixel);
    //            QRgb grayPixel = qRgb(gray,gray,gray);
    //            QColor color(gray,gray,gray,qAlpha(pixel));
    //            iGray.setPixel(i,j,color.rgba());
    //        }
    //    }
    //    return iGray;

    return m_image;
}

void MainWindow::init()
{
    //初始化下拉框
    QList<QCameraDevice> list = QMediaDevices::videoInputs(); //获得所有设备信息
    ui->comboBox->blockSignals(true);                         //不接受信号
    for (const auto &l : list) {
        ui->comboBox->addItem(l.description(), l.id());
        m_cameraDevices.append(l);
    }
    ui->comboBox->blockSignals(false);

    //初始化摄像头
    m_camera.reset(new QCamera(QMediaDevices::defaultVideoInput())); //新建 设置摄像头使用默认驱动
    m_cap.setCamera(m_camera.data());                                //捕获摄像头画面

    m_videoSink = ui->capWidget->videoSink();
    connect(m_videoSink,
            &QVideoSink::videoFrameChanged,
            this,
            &MainWindow::on_frame_changed); //帧改变信号

    m_camera->start();
    m_cap.setVideoOutput(ui->capWidget);

    //新建获取图像采集对象
    m_imageCapture = new QImageCapture;

    //录制
    m_mediaRecorder.reset(new QMediaRecorder);
    m_cap.setRecorder(m_mediaRecorder.data());

    //连接qcombobox信号  实现摄像头的切换
    connect(ui->comboBox, &QComboBox::currentIndexChanged, this, [=](int index) {
        //qDebug()<<m_cameraDevices.at(index);
        m_camera.reset(new QCamera(m_cameraDevices.at(index)));
        m_cap.setCamera(m_camera.data());
        m_cap.setVideoOutput(ui->capWidget);
        m_camera->start();
    });
}

void MainWindow::setLabelPixmap()
{
    static int a = 1;
    //    m_mutex->lock();
    //    m_image = image_que.first();
    //    image_que.removeFirst();
    //    qDebug()<<QThread::currentThread()<<image_que.size();
    //    m_image = m_image.convertToFormat(QImage::Format_Grayscale8, Qt::AutoColor);
    //    ui->label->setPixmap(QPixmap::fromImage(
    //        m_image.scaled(ui->label->width(), ui->label->height(), Qt::KeepAspectRatio)));
    //    m_mutex->unlock();
    qDebug() << __FUNCTION__ << QThread::currentThread();
    while (1) {
        try {
            if (!image_que.isEmpty()) {
                m_mutex->lock();
                m_image = image_que.first();
                image_que.removeFirst();
                //qDebug() << __FUNCTION__ << QThread::currentThread() << image_que.size()<<a++;

                m_image = m_image.convertToFormat(QImage::Format_Grayscale8, Qt::AutoColor);
                ui->label->setPixmap(QPixmap::fromImage(
                    m_image.scaled(ui->label->width(), ui->label->height(), Qt::KeepAspectRatio)));
                m_mutex->unlock();
                QThread::msleep(10);
            }
            else continue;
        }
        catch(QException ex){
            QMessageBox::information(this,"提示","isEmtpy");
            qDebug() << ex.what();
        }
    }
}

void MainWindow::on_frame_changed(const QVideoFrame &frame)
{
    m_mutex->lock();
    image_que.append(frame.toImage());
    if (image_que.size() > N)
        image_que.clear();
    m_mutex->unlock();

    //emit setPic(frame.toImage());

    m_width = frame.toImage().width();
    m_height = frame.toImage().height();

}



void MainWindow::on_action_triggered()
{
    qDebug()<<"yes";
}

