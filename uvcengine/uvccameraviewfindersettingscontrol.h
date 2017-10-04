#ifndef UVCCAMERAVIEWFINDERSETTINGSCONTROL_H
#define UVCCAMERAVIEWFINDERSETTINGSCONTROL_H

#include <qcameraviewfindersettingscontrol.h>

QT_BEGIN_NAMESPACE

class UVCCameraSession;

class UVCCameraViewfinderSettingsControl : public QCameraViewfinderSettingsControl2
{
public:
    UVCCameraViewfinderSettingsControl(UVCCameraSession *session);

    QList<QCameraViewfinderSettings> supportedViewfinderSettings() const;

    QCameraViewfinderSettings viewfinderSettings() const;
    void setViewfinderSettings(const QCameraViewfinderSettings &settings);

private:
    UVCCameraSession *m_session;
};

QT_END_NAMESPACE

#endif // UVCCAMERAVIEWFINDERSETTINGSCONTROL_H
