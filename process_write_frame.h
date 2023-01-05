#ifndef PROCESS_WRITE_FRAME_H
#define PROCESS_WRITE_FRAME_H

#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include <QVideoFrame>

class Process_write_frame : public QObject
{
    Q_OBJECT
    QString getTime(qint64 ms);
    QImage QVideoFrameToQImage(const QVideoFrame &videoFrame);
public:
    explicit Process_write_frame(QObject *parent = nullptr);



    QMutex mutex;
    QWaitCondition condition;
    ~Process_write_frame();
    void image_process(QVideoFrame videoFrame, qint64 pos);
    void image_process_invalide(QVideoFrame videoFrame, qint64 pos);
    void run();


signals:
    void finished();

public slots:

    void destroy();
};

#endif // PROCESS_WRITE_FRAME_H
