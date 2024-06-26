#include "phoxi_sensor.h"

int main()
{
    PhoxiSensor sensor("phoxi", "2018-02-020-LC3", "extra-large");
    sensor.start();
    sensor.frames();

    std::vector<std::vector<float>> depth_map = sensor.get_depth_map(); 

    sensor.stop();

    return 0;
}