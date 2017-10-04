#include "uvcvideodevicecontrol.h"

QT_BEGIN_NAMESPACE

Q_GLOBAL_STATIC(QList<UVCVideoDeviceInfo>, deviceList)

// this context may never get deleted. Is that a problem?
uvc_context_t* UVCVideoDeviceControl::ctx = NULL;
QMap<QString,bool> UVCVideoDeviceControl::busyDevices;

UVCVideoDeviceControl::UVCVideoDeviceControl(QObject *parent)
    : QVideoDeviceSelectorControl(parent),
      currentDevice(""),
      selected(0)
{
    m_session = qobject_cast<UVCCameraSession*>(parent);
    updateDevices();
}

UVCVideoDeviceControl::~UVCVideoDeviceControl()
{
    busyDevices.remove(currentDevice);
    currentDevice.clear();
}

int UVCVideoDeviceControl::deviceCount() const
{
    updateDevices();
    return deviceList->count();
}

QString UVCVideoDeviceControl::deviceName(int index) const
{
    updateDevices();

    if (index >= 0 && index <= deviceList->count())
        return QString::fromUtf8(deviceList->at(index).first.constData());

    return QString();
}

QString UVCVideoDeviceControl::deviceDescription(int index) const
{
    updateDevices();

    if (index >= 0 && index <= deviceList->count())
        return deviceList->at(index).second;

    return QString();
}

int UVCVideoDeviceControl::defaultDevice() const
{
    return 0;
}

int UVCVideoDeviceControl::selectedDevice() const
{
    return selected;
}

void UVCVideoDeviceControl::setSelectedDevice(int index)
{
    updateDevices();

    if (index >= 0 && index < deviceList->count()) {
        if (m_session) {
            QString device = deviceList->at(index).first;

            uvc_device_t **devList;
            uvc_error_t res;
            res = uvc_get_device_list(ctx, &devList);
            if (res < 0) {
                qWarning() << "uvc_get_device_list" << uvc_strerror(res);
                return;
            }

            int idx = 0;
            while (1) {
                uvc_device_t *dev = devList[idx];
                idx++;
                if (dev == NULL)
                    break;
                uvc_device_descriptor_t *desc;
                res = uvc_get_device_descriptor(dev, &desc);
                if (res < 0) {
                    qWarning() << "uvc_get_device_descriptor" << uvc_strerror(res);
                    return;
                }
                bool selectedDevice = QString(desc->product).compare(device) == 0;
                uvc_free_device_descriptor(desc);

                if (selectedDevice) {
                    if (!busyDevices.contains(device)) {
                        busyDevices[device] = true;
                        currentDevice = device;
                        m_session->setDevice(dev);
                    } else {
                        qWarning() << "Device is busy.";
                    }
                    break;
                }
            }
            uvc_free_device_list(devList, 0);
        }
        selected = index;
    }
}

const QList<UVCVideoDeviceInfo> &UVCVideoDeviceControl::availableDevices()
{
    updateDevices();
    return *deviceList;
}

void UVCVideoDeviceControl::updateDevices()
{
    static QElapsedTimer timer;
    if (timer.isValid() && timer.elapsed() < 500) // ms
        return;

    deviceList->clear();

    uvc_error_t res;

    if (!ctx) {
        // Init libuvc
        qInfo() << "Initializing libuvc context...";
        res = uvc_init(&ctx, NULL);
        if (res < 0) {
            qWarning() << "uvc_init" << uvc_strerror(res);
            return;
        }
    }

    uvc_device_t **devList;
    res = uvc_get_device_list(ctx, &devList);
    if (res < 0) {
        qWarning() << "uvc_get_device_list" << uvc_strerror(res);
        return;
    }

    int idx = 0;
    while (1) {
        uvc_device_t *dev = devList[idx];
        idx++;
        if (dev == NULL)
            break;
        uvc_device_descriptor_t *desc;
        res = uvc_get_device_descriptor(dev, &desc);
        if (res < 0) {
            qWarning() << "uvc_get_device_descriptor" << uvc_strerror(res);
            break;
        }

        if (QString(desc->product).isEmpty())
            continue;

        UVCVideoDeviceInfo devInfo;

        devInfo.first = QString(desc->product).toUtf8();
        devInfo.second = QString(desc->product).toUtf8();

        deviceList->append(devInfo);

        uvc_free_device_descriptor(desc);
    }

    uvc_free_device_list(devList, 0);

    timer.restart();
}
