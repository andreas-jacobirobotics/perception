#include "phoxi_sensor.h"

int main()
{
    PhoxiSensor sensor("phoxi", "2018-02-020-LC3", "extra-large");
    sensor.start();
    sensor.stop();

    return 0;
}