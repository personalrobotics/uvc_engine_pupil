#ifndef UVCVIDEODEVICECONTROL_H
#define UVCVIDEODEVICECONTROL_H

#include <QVideoDeviceSelectorControl>
#include <QStringList>
#include <QElapsedTimer>
#include <QDebug>

#include "uvccamerasession.h"

#include "libuvc/libuvc.h"

QT_BEGIN_NAMESPACE

typedef QPair<QByteArray, QString> UVCVideoDeviceInfo;

class UVCVideoDeviceControl : public QVideoDeviceSelectorControl
{
    Q_OBJECT
public:
    UVCVideoDeviceControl(QObject *parent = 0);
    ~UVCVideoDeviceControl();

    int deviceCount() const;
    QString deviceName(int index) const;
    QString deviceDescription(int index) const;
    int defaultDevice() const;
    int selectedDevice() const;

    static const QList<UVCVideoDeviceInfo> &availableDevices();

public Q_SLOTS:
    void setSelectedDevice(int index);

private:
    static void updateDevices();

    UVCCameraSession* m_session;
    int selected;
    static uvc_context_t* ctx;
    static QMap<QString,bool> busyDevices;
    QString currentDevice;
};

QT_END_NAMESPACE

#endif // UVCVIDEODEVICECONTROL_H
