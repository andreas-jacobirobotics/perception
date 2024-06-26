#pragma once

#include "PhoXi.h"

class PhoxiSensor {
public:
    std::string frame;
    std::string device_name;
    std::string size;
    bool running;
    pho::api::PFrame Frame;

    PhoxiSensor(const std::string& frame, const std::string& device_name, const std::string& size);

    // Start the sensor.
    bool start();

    // Stop the sensor.
    void stop();

    // Connect the sensor.
    bool connect();

    // Retrieve a frame from the sensor.
    void frames();

    // Get the depth map.
    std::vector<float> get_depth_map();

    const int depth_map_height();
    const int depth_map_width();

private:
    pho::api::PPhoXi PhoXiDevice;
    int depth_map_height_;
    int depth_map_width_;

    void printDeviceInfoList(const std::vector<pho::api::PhoXiDeviceInformation> &DeviceList);

    void printDeviceInfo(const pho::api::PhoXiDeviceInformation &DeviceInfo);

    void printFrameData(const pho::api::PFrame &Frame);

    void printFrameInfo(const pho::api::PFrame &Frame);
};