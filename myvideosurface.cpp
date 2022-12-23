#include "myvideosurface.h"

#include <QtWidgets>
#include <qabstractvideosurface.h>
#include <qvideosurfaceformat.h>
#include "private/qvideoframe_p.h"

MyVideoSurface::MyVideoSurface(QAbstractVideoSurface *parent):
    QAbstractVideoSurface(parent)
{

}

MyVideoSurface::~MyVideoSurface()
{

}

QList<QVideoFrame::PixelFormat> MyVideoSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
    Q_UNUSED(handleType);
    Q_UNUSED(handleType);
    return QList<QVideoFrame::PixelFormat>()
            << QVideoFrame::Format_ARGB32
            << QVideoFrame::Format_ARGB32_Premultiplied
            << QVideoFrame::Format_RGB32
            << QVideoFrame::Format_RGB24
            << QVideoFrame::Format_RGB565
            << QVideoFrame::Format_RGB555
            << QVideoFrame::Format_ARGB8565_Premultiplied
            << QVideoFrame::Format_BGRA32
            << QVideoFrame::Format_BGRA32_Premultiplied
            << QVideoFrame::Format_BGR32
            << QVideoFrame::Format_BGR24
            << QVideoFrame::Format_BGR565
            << QVideoFrame::Format_BGR555
            << QVideoFrame::Format_BGRA5658_Premultiplied
            << QVideoFrame::Format_AYUV444
            << QVideoFrame::Format_AYUV444_Premultiplied
            << QVideoFrame::Format_YUV444
            << QVideoFrame::Format_YUV420P
            << QVideoFrame::Format_YV12
            << QVideoFrame::Format_UYVY
            << QVideoFrame::Format_YUYV
            << QVideoFrame::Format_NV12
            << QVideoFrame::Format_NV21
            << QVideoFrame::Format_IMC1
            << QVideoFrame::Format_IMC2
            << QVideoFrame::Format_IMC3
            << QVideoFrame::Format_IMC4
            << QVideoFrame::Format_Y8
            << QVideoFrame::Format_Y16
            << QVideoFrame::Format_Jpeg
            << QVideoFrame::Format_CameraRaw
            << QVideoFrame::Format_AdobeDng;

    // Return the formats you will support
}

bool MyVideoSurface::isFormatSupported(const QVideoSurfaceFormat &format) const
{
    const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
    const QSize size = format.frameSize();

    return imageFormat != QImage::Format_Invalid
            && !size.isEmpty()
            && format.handleType() == QAbstractVideoBuffer::NoHandle;
}

//bool MyVideoSurface::start(const QVideoSurfaceFormat &format)
//{
//    const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
//    const QSize size = format.frameSize();

//    if (imageFormat != QImage::Format_Invalid && !size.isEmpty()) {
////        this->imageFormat = imageFormat;
////        imageSize = size;
////        sourceRect = format.viewport();

//        QAbstractVideoSurface::start(format);

////        widget->updateGeometry();
////        updateVideoRect();

//        return true;
//    }
//    else if(imageFormat == QImage::Format_Invalid && size.isEmpty())
//    {
//        QAbstractVideoSurface::start(format);

//    }
//    else {
//        return false;
//    }
//}

void MyVideoSurface::stop()
{
    //    currentFrame = QVideoFrame();
    //    targetRect = QRect();

    QAbstractVideoSurface::stop();

    //    widget->update();
}

bool MyVideoSurface::present(const QVideoFrame &frame)
{
    if(is_can_write)
    {
        if (frame.isValid())
        {
            QVideoFrame::PixelFormat pf = frame.pixelFormat();
            const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat());
            //            const QSize size = frame.size();
            QVideoFrame cloneFrame(frame);
            if (imageFormat != QImage::Format::Format_Invalid){
                cloneFrame.map(QAbstractVideoBuffer::ReadOnly);
                const QImage image(cloneFrame.bits(),
                                   cloneFrame.width(),
                                   cloneFrame.height(),
                                   QVideoFrame::imageFormatFromPixelFormat(cloneFrame .pixelFormat()));
                //        emit frameAvailable(image); // this is very important
                image.save("image_cor.png");
                cloneFrame.unmap();
            }
            else{
                QImage im = MyVideoSurface::QVideoFrameToQImage(frame);
                QString as = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
                as += ".png";
                im.save(as);
                is_can_write = false;
            }
        }
    }

    if (surfaceFormat().pixelFormat() != frame.pixelFormat()
            || surfaceFormat().frameSize() != frame.size()) {
        setError(IncorrectFormatError);
        stop();

        return false;
    } else {
        //        currentFrame = frame;

        //        widget->repaint(targetRect);

        return true;
    }
}

QImage MyVideoSurface::QVideoFrameToQImage( const QVideoFrame& videoFrame )
{
    if ( videoFrame.handleType() == QAbstractVideoBuffer::NoHandle )
    {
        QImage image = qt_imageFromVideoFrame( videoFrame );
        if ( image.isNull() )
        {
            return QImage();
        }

        if ( image.format() != QImage::Format_ARGB32 )
        {
            image = image.convertToFormat( QImage::Format_ARGB32 );
        }

        return image;
    }

    if ( videoFrame.handleType() == QAbstractVideoBuffer::GLTextureHandle )
    {
        QImage image( videoFrame.width(), videoFrame.height(), QImage::Format_ARGB32 );
        GLuint textureId = static_cast<GLuint>( videoFrame.handle().toInt() );
        QOpenGLContext* ctx = QOpenGLContext::currentContext();
        QOpenGLFunctions* f = ctx->functions();
        GLuint fbo;
        f->glGenFramebuffers( 1, &fbo );
        GLint prevFbo;
        f->glGetIntegerv( GL_FRAMEBUFFER_BINDING, &prevFbo );
        f->glBindFramebuffer( GL_FRAMEBUFFER, fbo );
        f->glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0 );
        f->glReadPixels( 0, 0,  videoFrame.width(),  videoFrame.height(), GL_RGBA, GL_UNSIGNED_BYTE, image.bits() );
        f->glBindFramebuffer( GL_FRAMEBUFFER, static_cast<GLuint>( prevFbo ) );
        return image.rgbSwapped();
    }

    return QImage();
}
