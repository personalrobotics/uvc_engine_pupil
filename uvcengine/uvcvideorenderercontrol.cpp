#include "uvcvideorenderercontrol.h"

QT_BEGIN_NAMESPACE

UVCVideoRendererControl::UVCVideoRendererControl(UVCCameraSession* session, QObject *parent)
    :QVideoRendererControl(parent),
    m_surface(NULL),
    m_session(session)
{
}

UVCVideoRendererControl::~UVCVideoRendererControl()
{
}

QAbstractVideoSurface* UVCVideoRendererControl::surface() const
{
    return m_surface;
}

void UVCVideoRendererControl::setSurface(QAbstractVideoSurface *surface)
{
    m_surface = surface;
    if(m_session)
        m_session->setSurface(m_surface);
}

QT_END_NAMESPACE
