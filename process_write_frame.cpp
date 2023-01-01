#include "process_write_frame.h"

#include <QtWidgets>
#include <qabstractvideosurface.h>
#include <qvideosurfaceformat.h>
#include "private/qvideoframe_p.h"

Process_write_frame::Process_write_frame(QObject *parent) : QObject(parent)
{

}

void Process_write_frame::run()
{
    mutex.lock();
    condition.wait(&mutex);
    mutex.unlock();
    emit finished();
}


Process_write_frame::~Process_write_frame()
{

}

void Process_write_frame::destroy()
{
    mutex.lock();
    condition.wakeOne();
    mutex.unlock();
}

QString Process_write_frame::getTime(qint64 ms)
{
    uint32_t ret_ms = ms % 1000u;
    ms /= 1000u;
    uint32_t sec = ms % 60u;
    ms /= 60u;
    uint32_t mn = ms % 60u;
    ms /= 60u;
    uint32_t hr = ms;
    QString str = "%1:%2:%3::%4";
    QString out = str.arg(hr).arg(mn).arg(sec).arg(ret_ms);
    return out;
};

QImage Process_write_frame::QVideoFrameToQImage( const QVideoFrame& videoFrame )
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

void Process_write_frame::image_process_invalide(QVideoFrame videoFrame, qint64 pos)
{
    QImage im = QVideoFrameToQImage(videoFrame);
    auto tm = getTime(pos);
    tm += ".png";
    im.save(tm);
    int h = 6;
}

void Process_write_frame::image_process(QVideoFrame videoFrame, qint64 pos)
{
    QVideoFrame cloneFrame(videoFrame);
    cloneFrame.map(QAbstractVideoBuffer::ReadOnly);
    const QImage im(cloneFrame.bits(),
                       cloneFrame.width(),
                       cloneFrame.height(),
                       QVideoFrame::imageFormatFromPixelFormat(cloneFrame .pixelFormat()));
    auto tm = getTime(pos);
    tm += ".png";
    im.save(tm);
    cloneFrame.unmap();
}
