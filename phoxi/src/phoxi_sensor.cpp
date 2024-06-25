#include "phoxi_sensor.h"

PhoxiSensor::PhoxiSensor(const std::string& frame, const std::string& device_name, const std::string& size) : frame(frame), device_name(device_name), size(size), running(false) {

}

void PhoxiSensor::start() {

}

bool PhoxiSensor::connect() {
    return false;
}