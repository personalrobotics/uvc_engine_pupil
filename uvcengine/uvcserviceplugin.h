#ifndef UVCSERVICEPLUGIN_H
#define UVCSERVICEPLUGIN_H

#include <QtMultimedia/QMediaServiceProviderPlugin>

#include "libuvc/libuvc.h"

#include "uvccameraservice.h"
#include "uvcvideodevicecontrol.h"

class UVCServicePlugin
    : public QMediaServiceProviderPlugin
    , public QMediaServiceSupportedDevicesInterface
    , public QMediaServiceDefaultDeviceInterface
    , public QMediaServiceFeaturesInterface
{
    Q_OBJECT
    Q_INTERFACES(QMediaServiceSupportedDevicesInterface)
    Q_INTERFACES(QMediaServiceDefaultDeviceInterface)
    Q_INTERFACES(QMediaServiceFeaturesInterface)

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.qt-project.qt.mediaserviceproviderfactory/5.0" FILE "uvcengine.json")
#endif // QT_VERSION >= 0x050000

public:
    QMediaService *create(const QString &key);
    void release(QMediaService *service);

    QMediaServiceProviderHint::Features supportedFeatures(const QByteArray &service) const;

    QByteArray defaultDevice(const QByteArray &service) const;
    QList<QByteArray> devices(const QByteArray &service) const;
    QString deviceDescription(const QByteArray &service, const QByteArray &device);

private:
};

#endif // UVCSERVICEPLUGIN_H
