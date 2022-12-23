#ifndef MYVIDEOSURFACE_H
#define MYVIDEOSURFACE_H

#include <QAbstractVideoSurface>

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

    bool is_can_write = false;
};

#endif // MYVIDEOSURFACE_H
