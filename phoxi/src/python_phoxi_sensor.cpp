#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <phoxi_sensor.h>


namespace py = pybind11;
using namespace pybind11::literals; // to bring in the `_a` literal

int add(int i, int j) {
    return i + j;
}

PYBIND11_MODULE(phoxi, m) {
    m.doc() = "Photoneo PhoXi driver";

    py::class_<PhoxiSensor>(m, "PhoxiSensor")
        .def(py::init<py::str, py::str, py::str>(), "frame"_a, "device_name"_a, "size"_a)
        .def("start", &PhoxiSensor::start)
        .def("stop", &PhoxiSensor::stop)
        .def("frames", &PhoxiSensor::frames)
        .def("get_depth_map", &PhoxiSensor::get_depth_map)
        .def_readonly("frame", &PhoxiSensor::frame)
        .def_readonly("ir_frame", &PhoxiSensor::frame);
    
    m.def("add", &add, "A function that adds two numbers");
}
