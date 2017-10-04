#ifndef UVCCAMERACONTROL_H
#define UVCCAMERACONTROL_H


#include <QtCore/qobject.h>
#include <QtMultimedia/QCameraControl>

QT_BEGIN_NAMESPACE

class UVCCameraService;
class UVCCameraSession;

class UVCCameraControl : public QCameraControl
{
    Q_OBJECT
public:
    UVCCameraControl(QObject *parent = 0);
    ~UVCCameraControl();

    QCamera::State state() const { return m_state; }

    QCamera::CaptureModes captureMode() const { return m_captureMode; }
    void setCaptureMode(QCamera::CaptureModes mode);

    void setState(QCamera::State state);

    QCamera::Status status() const;
    bool isCaptureModeSupported(QCamera::CaptureModes mode) const;
    bool canChangeProperty(PropertyChangeType /* changeType */, QCamera::Status /* status */) const {return false; }

private:
    UVCCameraSession *m_session;
    UVCCameraService *m_service;
    QCamera::State m_state;
    QCamera::CaptureModes m_captureMode;
};

QT_END_NAMESPACE

#endif // UVCCAMERACONTROL_H
