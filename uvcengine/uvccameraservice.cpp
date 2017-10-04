/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtCore/qvariant.h>
#include <QtCore/qdebug.h>

#include "uvccameraservice.h"
#include "uvccameracontrol.h"
#include "uvccamerasession.h"
#include "uvcvideodevicecontrol.h"
#include "uvcvideorenderercontrol.h"
#include "uvccameraviewfindersettingscontrol.h"

QT_BEGIN_NAMESPACE

UVCCameraService::UVCCameraService(QObject *parent):
    QMediaService(parent)
  , m_videoRenderer(NULL)
{
    m_session = new UVCCameraSession(this);
    m_control = new UVCCameraControl(m_session);
    m_videoDevice = new UVCVideoDeviceControl(m_session);
    m_viewfinderSettings = new UVCCameraViewfinderSettingsControl(m_session);
}

UVCCameraService::~UVCCameraService()
{
    delete m_control;
    delete m_viewfinderSettings;
    delete m_videoDevice;
    delete m_videoRenderer;
    delete m_session;
}

QMediaControl* UVCCameraService::requestControl(const char *name)
{
    if(qstrcmp(name,QCameraControl_iid) == 0)
        return m_control;

    if (qstrcmp(name,QVideoRendererControl_iid) == 0) {
        if (!m_videoRenderer) {
            m_videoRenderer = new UVCVideoRendererControl(m_session, this);
            return m_videoRenderer;
        }
    }

    if (qstrcmp(name,QVideoDeviceSelectorControl_iid) == 0)
        return m_videoDevice;

    if (qstrcmp(name, QCameraViewfinderSettingsControl2_iid) == 0)
        return m_viewfinderSettings;

//    if (qstrcmp(name, QCameraImageProcessingControl_iid) == 0)
//        return m_imageProcessingControl;
//
    return NULL;
}

void UVCCameraService::releaseControl(QMediaControl *control)
{
    if (control == m_videoRenderer) {
        delete m_videoRenderer;
        m_videoRenderer = NULL;
        return;
    }
}

QT_END_NAMESPACE
