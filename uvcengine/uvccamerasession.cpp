#include "uvccamerasession.h"
#include <QThread>
#include <QtConcurrent/QtConcurrent>

QMutex UVCCameraSession::sessionMutex;

UVCCameraSession::UVCCameraSession(QObject *parent)
    : QObject(parent),
      m_surface(Q_NULLPTR),
      streaming(false),
      bandwidthFactor(1.3f),
      interval(0),
      dev(NULL),
      devh(NULL)
{
}

UVCCameraSession::~UVCCameraSession()
{
    unload();
}

void UVCCameraSession::setDevice(uvc_device_t *device)
{
    dev = device;
}

void UVCCameraSession::setSurface(QAbstractVideoSurface* surface)
{
    QMutexLocker locker(&surfaceMutex);
    m_surface = surface;
}

// If the following is defined, frame grabbing is performed by the callback thread
//#define BLOCKING_FRAME_GRABBING
void cb(uvc_frame_t *frame, void *ptr) { if (ptr && frame) static_cast<UVCCameraSession*>(ptr)->callback(frame); }
void UVCCameraSession::callback(uvc_frame_t *frame)
{
    qreal t = frameReference.elapsed();

    if (!streaming)
        return;

    QMutexLocker locker(&surfaceMutex);
    if (m_surface) {
        QVideoFrame qFrame;
        switch(frame->frame_format) {
            case UVC_FRAME_FORMAT_MJPEG:
                qFrame = QVideoFrame( (int) frame->data_bytes, QSize(frame->width, frame->height), 0, QVideoFrame::Format_Jpeg);
                qFrame.map(QAbstractVideoBuffer::WriteOnly);
                memcpy( qFrame.bits(), frame->data, frame->data_bytes); // copied; safe to retun from callback now
                qFrame.unmap();
                break;
            default:
                return;
        }
#ifdef BLOCKING_FRAME_GRABBING
        m_surface->present( qFrame );
#else
        QMetaObject::invokeMethod(this, "presentFrame", Qt::QueuedConnection, Q_ARG(const QVideoFrame&, qFrame), Q_ARG(const qreal, t) );
#endif
    }
}

void UVCCameraSession::presentFrame(QVideoFrame frame, const qreal t)
{
    qreal latency = frameReference.elapsed() - t;
    frame.setMetaData("timestamp", t);
    if (latency <= MAX_LATENCY_MS && m_surface)
        m_surface->present(frame);
    //else
    //    qWarning() << "Dropping frame (" << latency << "ms old )";
}

bool UVCCameraSession::load()
{
    QMutexLocker sessionLocker(&sessionMutex);
    uvc_error_t res;

    if (!dev)
        return false;

    res = uvc_open(dev, &devh);
    if (res != UVC_SUCCESS) {
        qWarning() << "uvc_open" << uvc_strerror(res);
        devh = NULL;
        return false;
    }

    // TODO: parametrize this
    uvc_set_focus_auto(devh, 0);
    if (isPupilFieldCamera()) {
        bandwidthFactor = 2.0f;
        uvc_set_ae_priority(devh, 1);
    } else if (isPupilEyeCamera()) {
        uvc_set_ae_priority(devh, 0);
        uvc_set_ae_mode(devh, 1);
        uvc_set_saturation(devh, 127); // do not set saturation to zero as to not lose color information
        uvc_set_exposure_abs(devh, 63);
        uvc_set_backlight_compensation(devh, 2);
        uvc_set_gamma(devh, 100);
    }

    updateSourceCapabilities();

    return true;
}

bool UVCCameraSession::unload()
{
    QMutexLocker sessionLocker(&sessionMutex);

    if (streaming)
        stopPreview();

    // TODO: closing here seems to mess up the device
    //if (devh)
    //    uvc_close(devh);
    //devh = NULL;

    m_supportedViewfinderSettings.clear();
    return true;
}

void concurrentCustomCallback(UVCCameraSession *session)
{
    QMutexLocker getFrameLocker( &session->getFrameMutex );
    while (session->streaming) {
        if (session->strmh == NULL)
            continue;
        uvc_frame_t *frame = NULL;
        uvc_error_t res;
        res = uvc_stream_get_frame(session->strmh, &frame, 0);
        if ( res == UVC_SUCCESS && frame != NULL)
            session->callback(frame);
        else
            if (res != UVC_ERROR_TIMEOUT)
                qWarning() << session->streaming << uvc_strerror(res) << frame;
    }
}

// Custom callback is experimental; avoid it for the moment.
// Just here for debugging a libusb bug
//#define USE_CUSTOM_CALLBACK
bool UVCCameraSession::startPreview()
{
    uvc_error_t res;

    if (streaming)
        stopPreview();

    QMutexLocker getFrameLocker( &getFrameMutex );
    uvc_frame_format format;

    if (!qPixelFormat2UVCFrameFormat(settings.pixelFormat(), format)) {
        qWarning() << "Unknown pixel format";
        return false;
    }

    res = uvc_get_stream_ctrl_format_size(devh, &ctrl, format, settings.resolution().width(), settings.resolution().height(), settings.maximumFrameRate());

    if (res != UVC_SUCCESS) {
        qWarning() << "uvc_get_stream_ctrl" << uvc_strerror(res);
        return false;
    }

    res = uvc_stream_open_ctrl(devh, &strmh, &ctrl);
    if (res != UVC_SUCCESS) {
        qWarning() << "uvc_stream_open_ctrl" << uvc_strerror(res);
        return false;
    }

#ifdef USE_CUSTOM_CALLBACK
    res = uvc_stream_start(strmh, NULL, (void*) this, bandwidthFactor, 0);
#else
    res = uvc_stream_start(strmh, cb, (void*) this, bandwidthFactor, 0);
#endif
    if (res != UVC_SUCCESS) {
        qWarning() << "uvc_stream_start" << uvc_strerror(res);
        return false;
    }

    streaming = true;
    frameReference.restart();

#ifdef USE_CUSTOM_CALLBACK
    QtConcurrent::run(concurrentCustomCallback, this);
#endif
    return true;
}

bool UVCCameraSession::qPixelFormat2UVCFrameFormat(const QVideoFrame::PixelFormat &qFormat, uvc_frame_format &uvcFormat)
{
    switch(qFormat) {
        case QVideoFrame::Format_Jpeg:
            uvcFormat = UVC_FRAME_FORMAT_MJPEG;
            return true;
        // TODO: support for YUYV. See:
        // 		bDescriptorSubtype (UVC_VS_FORMAT_UNCOMPRESSED)
        //		guidFormat
        default:
            return false;
    }
}

void UVCCameraSession::updateSourceCapabilities()
{
    m_supportedViewfinderSettings.clear();

    if (!devh)
        return;

    const uvc_format_desc_t* formatDesc = uvc_get_format_descs(devh);
    while (formatDesc != NULL) {

        QVideoFrame::PixelFormat qFormat;
        switch (formatDesc->bDescriptorSubtype) {
            case UVC_VS_FORMAT_MJPEG:
                qFormat = QVideoFrame::Format_Jpeg;
                break;
            default: // format not supported, next!
                formatDesc = formatDesc->next;
                continue;
        }

        const uvc_frame_desc *frameDesc = formatDesc->frame_descs;
        while (frameDesc!=NULL) {
            QCameraViewfinderSettings settings;
            settings.setResolution(frameDesc->wWidth, frameDesc->wHeight);
            settings.setPixelAspectRatio(frameDesc->wWidth, frameDesc->wHeight);
            settings.setPixelAspectRatio(formatDesc->bAspectRatioX,formatDesc->bAspectRatioY);
            settings.setPixelFormat(qFormat);

            uint32_t *intervals = frameDesc->intervals;
            while (*intervals) {
                // libuvc uses 100ns units
                double fps = 1.0e7 / (*intervals);
                settings.setMinimumFrameRate( fps );
                settings.setMaximumFrameRate( fps );
                m_supportedViewfinderSettings.push_back(settings);
                intervals++;
            }

            frameDesc = frameDesc->next;
        }

        formatDesc = formatDesc->next;
    }
}

bool UVCCameraSession::stopPreview()
{
    uvc_error_t res;
    streaming = false;

    // for some reason libuvc seems to timeout on uvc_stream_stop
    for (unsigned int i=0; i<100;i++)
        if ( (res = uvc_stream_stop(strmh)) != UVC_ERROR_TIMEOUT)
            break;
    if (res != UVC_SUCCESS && res != UVC_ERROR_INVALID_PARAM)
        qWarning() << "uvc_stream_stop" << uvc_strerror(res);
    uvc_stream_close(strmh);

    frameReference.invalidate();
    return true;
}

QList<QCameraViewfinderSettings> UVCCameraSession::supportedViewfinderSettings()
{
    return m_supportedViewfinderSettings;
}

QCameraViewfinderSettings UVCCameraSession::viewfinderSettings()
{
    return QCameraViewfinderSettings();
}

void UVCCameraSession::setViewfinderSettings(const QCameraViewfinderSettings &settings)
{
    this->settings = settings;
    interval = 1 / settings.maximumFrameRate();
}

bool UVCCameraSession::isPupilFieldCamera()
{
    bool ret = false;
    uvc_device_descriptor *desc;
    uvc_get_device_descriptor(dev, &desc);
    QString name(desc->product);
    if ( name.contains("Pupil Cam1 ID2") )
        ret = true;
    uvc_free_device_descriptor(desc);
    return ret;
}

bool UVCCameraSession::isPupilEyeCamera()
{
    bool ret = false;
    uvc_device_descriptor *desc;
    uvc_get_device_descriptor(dev, &desc);
    QString name(desc->product);
    if ( name.contains("Pupil Cam1 ID0") || name.contains("Pupil Cam1 ID1") )
        ret = true;
    uvc_free_device_descriptor(desc);
    return ret;
}
