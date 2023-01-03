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

signals:
    void finished();

public slots:
    void image_process(QVideoFrame videoFrame, qint64 pos);
    void destroy();
    void image_process_invalide(QVideoFrame videoFrame, qint64 pos);
    void run();
};

#endif // PROCESS_WRITE_FRAME_H
