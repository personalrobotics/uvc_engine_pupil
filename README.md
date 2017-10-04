Qt UVC Engine
=============

This plugin provides a backend for UVC camera access through Qt's QCamera on Windows,
which usually is provided through DirectShow or Microsoft Media Foundation.

It was mainly developed (and tested!) to allow for an uniform interface to Pupil Labs' eye tracker,
thus allowing [EyeRecToo] (http://www.ti.uni-tuebingen.de/EyeRecToo.1907.0.html) to make use of these devices.
For up-to-date instructions on driver installation for Pupil eye trackers, check the [Pupil Labs' guide] (https://github.com/pupil-labs/pupil/wiki/Windows-Driver-Setup).


Notes
------------
**This is beta software at the moment, in particular because of the poor libuvc/libusb support on Windows.**

Dependencies
------------

1. [Libusb with isochronous support] (http://github.com/pupil-labs/libusb)
2. [Libuvc] (http://github.com/pupil-labs/libuvc)
3. MSVS 2015
4. Developed with Qt 5.7.0 [(here for possible compatibility issues)]  (http://doc.qt.io/qt-5/deployment-plugins.html)

