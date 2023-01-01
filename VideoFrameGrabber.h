#ifndef VIDEOFRAMEGRABBER_H
#define VIDEOFRAMEGRABBER_H

#include <QtWidgets>
#include <QAbstractVideoSurface>

class VideoFrameGrabber : public QAbstractVideoSurface
{
    Q_OBJECT

public:
    VideoFrameGrabber(QWidget *widget, QObject *parent = 0);

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const override;
    bool isFormatSupported(const QVideoSurfaceFormat &format) const override;

    bool start(const QVideoSurfaceFormat &format) override;
    void stop() override;

    bool present(const QVideoFrame &frame) override;

    QRect videoRect() const { return targetRect; }
    void updateVideoRect();

    void paint(QPainter *painter);

private:
    QWidget *widget;
    QImage::Format imageFormat;
    QRect targetRect;
    QSize imageSize;
    QRect sourceRect;
    QVideoFrame currentFrame;

signals:
    void frameAvailable(QImage frame);
};
#endif //VIDEOFRAMEGRABBER_H

