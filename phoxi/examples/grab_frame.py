import numpy as np
import matplotlib.pyplot as plt

import sys
sys.path.append('/home/andreas/Software/perception/phoxi/build/')

from phoxi import PhoxiSensor

# import ipdb; ipdb.set_trace()

sensor = PhoxiSensor('', '', '')
sensor.start()
sensor.frames()
depth_map = sensor.get_depth_map()
depth_map = np.array(depth_map)

plt.imshow(depth_map)
plt.show()
