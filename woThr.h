
#ifndef WOTHR_H
#define WOTHR_H


#include <QMainWindow>
#include <QObject>
#include <QThread>
#include <QImage>
#include <QDebug>
#include <QLabel>
#include <QQueue>

#include <QMutex>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class WoThr : public QObject
{
    Q_OBJECT
public:
    explicit WoThr(QObject *parent = nullptr);
    QLabel *label;
    QMutex *m_mutex;
    bool flag = true;
    bool endflag = true;


signals:

public slots:
    void doWork();
    void setPic(QImage ima);


private:

};

#endif // WOTHR_H
