#pragma once

#include "PhoXi.h"

class PhoxiSensor {
    std::string frame;
    std::string device_name;
    std::string size;
    bool running;

    PhoxiSensor(const std::string& frame, const std::string& device_name, const std::string& size);

    // Start the sensor.
    void start();

    // Stop the sensor.
    void stop();

    // Connect the sensor.
    bool connect();

    // Retrieve a frame from the sensor.
    void frames();
};