import numpy as np
import matplotlib.pyplot as plt

import sys
sys.path.append('/home/andreas/Software/perception/phoxi/build/')

from phoxi import PhoxiSensor

from autolab_core import CameraIntrinsics, ColorImage, DepthImage


class PhoXiSensor(PhoxiSensor):
    """Class for interfacing with a PhoXi Structured Light Sensor."""

    def __init__(self, frame: str, device_name: str, size: str):
        super().__init__(frame, device_name, size)

        self.scaling_factor = 1000.0

        # Set up camera intrinsics for the sensor
        width, height = 2064, 1544
        focal_x = self.fx
        focal_y = self.fy
        center_x = self.cx
        center_y = self.cy

        self._camera_intr = CameraIntrinsics(
            self.frame,
            focal_x,
            focal_y,
            center_x,
            center_y,
            height=height,
            width=width,
        )

        self.ir_intrinsics = self._camera_intr

    def frames(self):
        super().frames()

        focal_x = self.fx
        focal_y = self.fy
        center_x = self.cx
        center_y = self.cy
        self._camera_intr = CameraIntrinsics(
            self.frame,
            focal_x,
            focal_y,
            center_x,
            center_y,
            height=self._camera_intr.height,
            width=self._camera_intr.width,
        )
        self.ir_intrinsics = self._camera_intr

        depth_map = np.array(self.get_depth_map())
        depth_image = DepthImage(depth_map / self.scaling_factor)

        texture = np.array(self.get_texture())
        texture = (texture / texture.max() * 255).astype(np.uint8)
        texture = np.dstack((texture,)*3)
        color_image = ColorImage(texture)

        plt.imshow(depth_image.data)
        plt.show()

        return color_image, depth_image