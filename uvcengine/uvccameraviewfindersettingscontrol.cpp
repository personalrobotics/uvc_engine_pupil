#include "uvccameraviewfindersettingscontrol.h"
#include "uvccamerasession.h"

QT_BEGIN_NAMESPACE

UVCCameraViewfinderSettingsControl::UVCCameraViewfinderSettingsControl(UVCCameraSession *session)
    : QCameraViewfinderSettingsControl2(session)
    , m_session(session)
{
}

QList<QCameraViewfinderSettings> UVCCameraViewfinderSettingsControl::supportedViewfinderSettings() const
{
    return m_session->supportedViewfinderSettings();
}

QCameraViewfinderSettings UVCCameraViewfinderSettingsControl::viewfinderSettings() const
{
    return m_session->viewfinderSettings();
}

void UVCCameraViewfinderSettingsControl::setViewfinderSettings(const QCameraViewfinderSettings &settings)
{
    m_session->setViewfinderSettings(settings);
}

QT_END_NAMESPACE
