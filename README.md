## Berkeley Autolab Perception Module
[![pypi](https://img.shields.io/pypi/v/autolab-perception.svg)](https://pypi.org/project/autolab-perception/) [![python-versions](https://img.shields.io/pypi/pyversions/autolab-perception.svg)](https://pypi.org/project/autolab-perception/) [![status](https://github.com/BerkeleyAutomation/perception/workflows/Release%20Perception/badge.svg)](https://github.com/BerkeleyAutomation/perception/actions) [![style](https://img.shields.io/badge/code%20style-black-000000.svg)](https://github.com/psf/black)

This package provides a wide variety of useful tools for perception tasks.
It directly depends on the [Berkeley Autolab Core
module](https://www.github.com/BerkeleyAutomation/autolab_core), so be sure to install
that first.
View the install guide and API documentation for the perception module
[here](https://BerkeleyAutomation.github.io/perception). Dependencies for each driver are not automatically installed, so please install ROS or camera-specific packages separately before using these wrappers.

NOTE: As of May 4, 2021, this package no longer supports Python versions 3.5 or lower as these versions have reached EOL. In addition, many modules have been moved to `autolab_core` to reduce confusion. This repository now will contain sensor drivers and interfaces only. If you wish to use older Python versions or rely on the old modules, please use the 0.x.x series of tags.

### Photoneo PhoXi 3D Scanner
To run PhoXi Control, use the following command which enforces OpenGL to use the Nvidia driver:
```
__NV_PRIME_RENDER_OFFLOAD_PROVIDER=NVIDIA-G0 __GLX_VENDOR_LIBRARY_NAME=nvidia PhoXiControl
```

In PhoXi control, select the device, e.g., PhoXi3DScan-DKV-164, and click Connect.

### Robot Base to Camera Calibration
The following procedure assumes that the robot is located at the world frame's origin.

1. Place a chessboard so that the camera can see it.
2. Measure the (x,y,z) position of the camera relative to the robot's base frame.
3. Detect the chessboard and get the transform from the camera to the world frame.
```
python tools/register_camera_args.py --config cfg/tools/[FILENAME].yaml --cb_world [X] [Y] [Z]
```


