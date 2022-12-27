#ifndef MYVIDEOSURFACE_H
#define MYVIDEOSURFACE_H

#include <QAbstractVideoSurface>
#include <QMediaPlayer>

class MyVideoSurface : public QAbstractVideoSurface
{

public:
    explicit MyVideoSurface(QAbstractVideoSurface *parent = nullptr);
    ~MyVideoSurface();


    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const override;

    bool present(const QVideoFrame &frame) override;

    bool isFormatSupported(const QVideoSurfaceFormat &format) const override;
//    bool start(const QVideoSurfaceFormat &format) override;
    void stop() override;

    QImage QVideoFrameToQImage( const QVideoFrame& videoFrame );

    void setPlayer(QMediaPlayer * ptr);

    bool is_can_write = false;

    qint64 pos_play;
    QString file_video;

private:
    QMediaPlayer * m_player;
};

#endif // MYVIDEOSURFACE_H
