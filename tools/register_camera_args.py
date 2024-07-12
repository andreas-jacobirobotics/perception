"""
Script to register sensors to a chessboard
"""
import argparse
import logging
import os
import time
import traceback
import yaml
from pathlib import Path

import cv2
import numpy as np
from autolab_core import (
    CameraChessboardRegistration,
    RigidTransform,
)

from perception import RgbdSensorFactory


if __name__ == "__main__":
    # parse args
    parser = argparse.ArgumentParser(
        description="Register a camera to a robot"
    )
    parser.add_argument(
        "--config",
        type=str,
        default="cfg/tools/register_camera.yaml",
        help="filepath of a YAML configuration for registration",
    )
    parser.add_argument('--image', required=False, default='')
    parser.add_argument('--cb_world', nargs=3, required=False, default=[], type=float, help='position of chessboard in world frame (x, y, z)')
    args = parser.parse_args()
    config_filename = args.config
    
    with open(config_filename, 'r') as fh:
        config = yaml.safe_load(fh)
    
    # get known tf from chessboard to world
    if len(args.cb_world) > 0:
        T_cb_world = RigidTransform(rotation=np.eye(3), translation=np.array(args.cb_world), from_frame='cb', to_frame='world')
        print(f'T_cb_world: {T_cb_world}')
    else:
        T_cb_world = RigidTransform(rotation=np.eye(3), translation=np.array([0.279, 0.779, 1.13]), from_frame='cb', to_frame='world')
        
    # get camera sensor object
    for sensor_frame, sensor_data in config["sensors"].items():
        print("Registering %s" % (sensor_frame))
        sensor_config = sensor_data["sensor_config"]
        registration_config = sensor_data["registration_config"].copy()
        registration_config.update(config["chessboard_registration"])

        # open sensor
        try:
            sensor_type = sensor_config["type"]
            sensor_config["frame"] = sensor_frame
            print("Creating sensor")
            sensor = RgbdSensorFactory.sensor(sensor_type, sensor_config)
            print("Starting sensor")
            sensor.start()
            print("Sensor initialized")

            if sensor_type == 'realsense':
                sensor.ir_intrinsics = sensor.color_intrinsics
                sensor.ir_frame = 'realsense'

            # register camera using chessboard
            reg_result = CameraChessboardRegistration.register(
                sensor, registration_config
            )
            T_camera_world = T_cb_world * reg_result.T_camera_cb

            print("Final Result for sensor %s" % (sensor_frame))
            print("Rotation: ")
            print(T_camera_world.rotation)
            print("Quaternion: ")
            print(T_camera_world.quaternion)
            print("Translation: ")
            print(T_camera_world.translation)

        except Exception:
            logging.error("Failed to register sensor {}".format(sensor_frame))
            traceback.print_exc()
            continue
