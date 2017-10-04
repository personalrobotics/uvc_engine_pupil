Qt UVC Engine
=============

Original GitLab link: https://atreus.informatik.uni-tuebingen.de/santini/uvcengine

This plugin provides a backend for UVC camera access through Qt's QCamera on Ubuntu. You **must** install
this plugin before running EyeRecToo with the Pupil Labs cameras. Otherwise, nothing will work and you will be sad.


Notes
------------
**This is beta software at the moment. It seems to work OK, but has only been tested in Ubuntu 14.04.**

Dependencies
------------

1. [Libusb with isochronous support]
2. [Libuvc]
3. [Qt 5.7.0]

**Note:** The last dependency is needed to build the .pro file and run EyeRecToo. 
Qt is a "make-makefile" system similar to CMake. Google around to figure out how to use it here, or ask sniyaz.
