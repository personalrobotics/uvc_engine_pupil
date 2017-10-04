#ifndef UVCCAMERASESSION_H
#define UVCCAMERASESSION_H

#include <QObject>
#include <QTime>
#include <QUrl>
#include <QMutex>
#include <QThread>

#include <qcamera.h>
#include <QtMultimedia/qvideoframe.h>
#include <QtMultimedia/qabstractvideosurface.h>
#include <QtMultimedia/qvideosurfaceformat.h>
#include <QtMultimedia/qcameraimageprocessingcontrol.h>
#include <QtMultimedia/qabstractvideobuffer.h>

#include <QElapsedTimer>

#include <vector>
#include <atomic>

#include "libuvc/libuvc.h"

QT_BEGIN_NAMESPACE

// Maximum acceptable latency for calling the surface present method
#define MAX_LATENCY_MS 1.0e3/25

class UVCCameraSession : public QObject
{
    Q_OBJECT
public:
    UVCCameraSession(QObject *parent = 0);
    ~UVCCameraSession();

    QCamera::Status status() const { return m_status; }

    void setSurface(QAbstractVideoSurface* surface);

    QList<QCameraViewfinderSettings> supportedViewfinderSettings();
    QCameraViewfinderSettings viewfinderSettings();
    void setViewfinderSettings(const QCameraViewfinderSettings &settings);

    void setDevice(uvc_device_t *device);

    bool load();
    bool unload();
    bool startPreview();
    bool stopPreview();
    void callback(uvc_frame_t *frame);

    bool streaming;
    uvc_stream_handle_t *strmh;
    QMutex getFrameMutex;
    double interval;

private Q_SLOTS:
    void presentFrame(QVideoFrame frame, const qreal t);

private:
    float bandwidthFactor;
    QCamera::Status m_status;
    QAbstractVideoSurface *m_surface;
    QMutex surfaceMutex;

    // Source (camera)
    uvc_device_t *dev;
    uvc_device_handle_t *devh;
    uvc_stream_ctrl_t ctrl;

    // Frame
    QVideoFrame currentFrame;
    unsigned char* yuvBuffer;
    long unsigned int yuvBufferSize;
    QElapsedTimer frameReference;

    // capabilities
    QList<QCameraViewfinderSettings> m_supportedViewfinderSettings;
    void updateSourceCapabilities();
    QCameraViewfinderSettings settings;
    bool qPixelFormat2UVCFrameFormat(const QVideoFrame::PixelFormat &qFormat, uvc_frame_format &uvcFormat);

    static QMutex sessionMutex;

    bool isPupilFieldCamera();
    bool isPupilEyeCamera();

};

QT_END_NAMESPACE


#endif // UVCCAMERASESSION_H
