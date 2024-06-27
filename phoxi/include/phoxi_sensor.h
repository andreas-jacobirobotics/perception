#pragma once

#include "PhoXi.h"

class PhoxiSensor {
public:
    std::string frame;
    std::string device_name;
    std::string size;
    bool running;
    pho::api::PFrame Frame;
    float fx;
    float fy;
    float cx;
    float cy;

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
    std::vector<std::vector<float>> get_depth_map();

    // Get the texture.
    std::vector<std::vector<float>> get_texture();

    // // Get the point cloud.
    // std::vector<std::vector<float>> get_point_cloud();

    // // Get the normal map.
    // std::vector<std::vector<float>> get_normal_map();

    // std::vector<float> get_depth_map_1d();

private:
    pho::api::PPhoXi PhoXiDevice;

    void printDeviceInfoList(const std::vector<pho::api::PhoXiDeviceInformation> &DeviceList);

    void printDeviceInfo(const pho::api::PhoXiDeviceInformation &DeviceInfo);

    void printFrameData(const pho::api::PFrame &Frame);

    void printFrameInfo(const pho::api::PFrame &Frame);
};