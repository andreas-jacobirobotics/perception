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

        # Set up camera intrinsics for the sensor
        width, height = 2064, 1544
        focal_x, focal_y = 2244.0, 2244.0
        center_x, center_y = 1023.0, 768.0
        if size == "small":
            width = 1032
            height = 772
            focal_x = focal_x / 2
            focal_y = focal_y / 2
            center_x = center_x / 2
            center_y = center_y / 2

            if str(device_name) == "1703005":
                focal_x = focal_y = 1105.0

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
        depth_map = np.array(self.get_depth_map())
        depth_image = DepthImage(depth_map)
        # color_map = (depth_map / depth_map.max() * 255).astype(np.uint8)
        # color_map = np.dstack((color_map,)*3)
        # color_image = ColorImage(color_map)

        texture = np.array(self.get_texture())
        texture = (texture / texture.max() * 255).astype(np.uint8)
        texture = np.dstack((texture,)*3)
        color_image = ColorImage(texture)

        # cloud = np.array(self.get_point_cloud())
        # plt.imshow(cloud)
        # plt.show()

        # normal_map = np.array(self.get_normal_map())
        # plt.imshow(normal_map)
        # plt.show()

        # plt.imshow(color_image.data)
        # plt.show()

        # plt.imshow(depth_image.data)
        # plt.show()

        return color_image, depth_image