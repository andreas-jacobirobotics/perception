import numpy as np
import matplotlib.pyplot as plt

import sys
sys.path.append('/home/andreas/Software/perception/phoxi/build/')

from phoxi import PhoxiSensor

from autolab_core import ColorImage, DepthImage


class PhoXiSensor(PhoxiSensor):
    """Class for interfacing with a PhoXi Structured Light Sensor."""

    def frames(self):
        super().frames()
        depth_map = np.array(self.get_depth_map())
        depth_image = DepthImage(depth_map)
        color_map = (depth_map / depth_map.max() * 255).astype(np.uint8)
        color_map = np.dstack((color_map,)*3)
        color_image = ColorImage(color_map)

        # plt.imshow(color_image.data)
        # plt.show()

        # plt.imshow(depth_image.data)
        # plt.show()

        return color_image, depth_image