#ifndef MYVIDEOSURFACE_H
#define MYVIDEOSURFACE_H

#include <QAbstractVideoSurface>
#include <QMediaPlayer>
#include <QObject>

class MyVideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    explicit MyVideoSurface(QAbstractVideoSurface *parent = nullptr);
    ~MyVideoSurface();


    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const override;

    bool present(const QVideoFrame &frame) override;

    bool isFormatSupported(const QVideoSurfaceFormat &format) const override;
    bool start(const QVideoSurfaceFormat &format) override;
    void stop() override;

    QImage QVideoFrameToQImage( const QVideoFrame& videoFrame );

    bool is_can_write = false;

    qint64 pos_play;
    QString file_video;

public slots:
    void destroy_all(bool def = true);
signals:
    void process_image_valid(QVideoFrame, qint64);
    void process_image_invalid(QVideoFrame, qint64);
    void destroy();
    void destroy_main();

private:
    QVideoSurfaceFormat * m_format;
};

#endif // MYVIDEOSURFACE_H
