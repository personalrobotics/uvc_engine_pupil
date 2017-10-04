#include "uvcserviceplugin.h"

QMediaService* UVCServicePlugin::create(const QString &key)
{
    if (key == Q_MEDIASERVICE_CAMERA)
        return new UVCCameraService();
    return NULL;
}

void UVCServicePlugin::release(QMediaService *service)
{
    delete service;
}

QByteArray UVCServicePlugin::defaultDevice(const QByteArray &service) const
{
    if (service == Q_MEDIASERVICE_CAMERA) {
        const QList<UVCVideoDeviceInfo> &devs = UVCVideoDeviceControl::availableDevices();
        if (!devs.isEmpty())
            return devs.first().first;
    }

    return QByteArray();
}

QList<QByteArray> UVCServicePlugin::devices(const QByteArray &service) const
{
    QList<QByteArray> result;

    if (service == Q_MEDIASERVICE_CAMERA) {
        const QList<UVCVideoDeviceInfo> &devs = UVCVideoDeviceControl::availableDevices();
        for (const UVCVideoDeviceInfo &info : devs)
            result.append(info.first);
    }

    return result;
}

QString UVCServicePlugin::deviceDescription(const QByteArray &service, const QByteArray &device)
{
    if (service == Q_MEDIASERVICE_CAMERA) {
        const QList<UVCVideoDeviceInfo> &devs = UVCVideoDeviceControl::availableDevices();
        for (const UVCVideoDeviceInfo &info : devs)
            if (info.first == device)
                return info.second;
    }

    return QString();
}

QMediaServiceProviderHint::Features UVCServicePlugin::supportedFeatures(
        const QByteArray &service) const
{
    if (service == Q_MEDIASERVICE_MEDIAPLAYER)
        return QMediaServiceProviderHint::StreamPlayback | QMediaServiceProviderHint::VideoSurface;
    else
        return QMediaServiceProviderHint::Features();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(uvcengine, UVCServicePlugin)
#endif // QT_VERSION < 0x050000
