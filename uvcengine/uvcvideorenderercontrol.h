#ifndef UVCVIDEORENDERERCONTROL_H
#define UVCVIDEORENDERERCONTROL_H

#include <qvideorenderercontrol.h>
#include "uvccamerasession.h"

class CameraFormatConverter;

QT_BEGIN_NAMESPACE


class UVCVideoRendererControl : public QVideoRendererControl
{
    Q_OBJECT
public:
    UVCVideoRendererControl(UVCCameraSession* session, QObject *parent = 0);
    ~UVCVideoRendererControl();

    QAbstractVideoSurface *surface() const;
    void setSurface(QAbstractVideoSurface *surface);

private:
    QAbstractVideoSurface* m_surface;
    UVCCameraSession* m_session;
    CameraFormatConverter* converter;
};

QT_END_NAMESPACE

#endif // UVCVIDEORENDERERCONTROL_H
